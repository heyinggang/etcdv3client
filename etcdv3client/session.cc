/* Author: lipixun
 * Created Time : 2018-05-20 17:44:12
 *
 * File Name: session.cc
 * Description:
 *
 *    The session implementation, this is a high-level encapsulation of lease
 *
 */

#include "session.h"

#include "glog/logging.h"

#include "cbase/time/backoff.h"

#include "client.h"

using namespace brainaas::etcdv3client;

// Grant the session
auto Session::Grant(const std::shared_ptr<context::Context>& ctx,
                    const SessionOptions& options) -> maybecommon::Status {
  std::unique_lock<std::mutex> lk(m_);
  if (closed_.IsSet()) {
    // A closed session cannot be granted again
    return Status(StatusCode::SessionAlreadyClosed, "Session is already closed");
  }
  if (granted_) {
    // Session is already granted
    return Status(StatusCode::SessionAlreadyGranted, "Session is already granted");
  }
  // Start the keep alive thread,
  grant_and_keep_alive_thread_ = std::thread(&Session::GrantAndKeepAlive,
                                             this,
                                             ctx,
                                             options);
  granted_ = true;
  return Status();
}

auto Session::Close() -> void {
  std::unique_lock<std::mutex> lk(m_);
  // Set close
  closed_.Set();
  // Wait thread
  if (grant_and_keep_alive_thread_.joinable()) {
    grant_and_keep_alive_thread_.join();
  }
}

auto Session::GrantAndKeepAlive(std::shared_ptr<context::Context> ctx, SessionOptions options) -> void {
  auto close_event = context::WaitAny({ctx.get(), &closed_});
  while (!close_event->Finished()) {
    // Try grant
    VLOG(1) << LogPrefix << "::Session - Try grant lease " << lease_;
    TryGrant(close_event, options);
    options.set_lease(lease_);
    options.set_ttl(lease_ttl_);
    VLOG(1) << LogPrefix << "::Session - Complete grant lease " << lease_;
    if (!close_event->Finished()) {
      // Set alive
      expired_->Reset();
      alive_->Set();
      // Try keepalive
      VLOG(1) << LogPrefix << "::Session - Try keep alive for " << lease_;
      TryKeepAlive(close_event, options);
      VLOG(1) << LogPrefix << "::Session - Stop keep alive for " << lease_;
      // Set expired
      alive_->Reset();
      expired_->Set();
    }
  }
  // Revoke
  TryRevoke(options);
  // Close
  closed_.Set();
}

auto Session::TryGrant(const std::shared_ptr<context::Waitable>& close_event, const SessionOptions& options) -> void {
  brainaas::cbase::time::BackOffStrategy backoff(options.get_retry_min_wait_time(), options.get_retry_max_wait_time());

  while (!close_event->Finished()) {
    VLOG(1) << LogPrefix << "::Session - Start grant with lease " << options.get_lease() << " ttl " << options.get_ttl();
    proto::LeaseGrantResponse response;
    auto context = GetGRPCClientContext(options.get_get_grpc_client_context_func());
    auto status = client_->LeaseGrant(context.get(),
                                      options.get_ttl(),
                                      &response,
                                      LeaseGrantOptions().WithID(options.get_lease()));
    if (status.ok()) {
      // Good
      lease_ = response.id();
      lease_ttl_ = response.ttl(); // In seconds
      return;
    }
    // Bad
    LOG_EVERY_N(ERROR, 10) << LogPrefix << "::Session - Failed to grant with error: #" << status.error_code() << " " << status.error_message();
    // Wait with backoff
    backoff.Reset(false);
    if (close_event->Wait(backoff.Next())) {
      // Close event
      return;
    }
  }
}

auto Session::TryKeepAlive(const std::shared_ptr<context::Waitable>& close_event, const SessionOptions& options) -> void {
  auto ttl = std::chrono::seconds(lease_ttl_);
  brainaas::cbase::time::BackOffStrategy backoff(std::min(options.get_retry_min_wait_time(), ttl),
                                                 std::min(options.get_retry_max_wait_time(), ttl));
  while (!close_event->Finished()) {
    VLOG(1) << LogPrefix << "::Session - Start session keep alive with lease " << lease_ << " ttl " << options.get_ttl();
    auto context = GetGRPCClientContext(options.get_get_grpc_client_context_func());
    auto keep_alive_rw = client_->LeaseKeepAlive(context.get());
    while (true) { // Inner loop to keep alive
      if (close_event->Wait(backoff.Next())) {
        // Close event
        VLOG(1) << LogPrefix << "::Session - Close event is set for " << lease_;
        context->TryCancel();
        keep_alive_rw->Finish();
        return;
      }
      // Write request and wait for response
      proto::LeaseKeepAliveRequest req;
      proto::LeaseKeepAliveResponse rsp;
      req.set_id(lease_);
      VLOG(3) << LogPrefix << "::Session - Keep alive start for " << lease_;
      if (keep_alive_rw->Write(req) && keep_alive_rw->Read(&rsp)) {
        // Good
        VLOG(3) << LogPrefix << "::Session - Keep alive succeed for " << lease_;
        lease_ = rsp.id();
        lease_ttl_ = rsp.ttl();
        backoff.Reset();
      } else {
        // Bad
        auto status = keep_alive_rw->Finish();
        if (status.error_code() == grpc::StatusCode::NOT_FOUND) {
          // Lease not found, close the session
          LOG(ERROR) << LogPrefix << "::Session - Lost lease " << lease_;
          return;
        }
        LOG(ERROR) << LogPrefix << "::Session - Failed to keep alive for " << lease_ << " with error: #" << status.error_code() << " " << status.error_message();
        backoff.Reset(false);
        std::this_thread::sleep_for(backoff.Next());
        break;
      }
    } // Inner loop
  }
}

auto Session::TryRevoke(const SessionOptions& options) -> void {
  brainaas::cbase::time::BackOffStrategy backoff(options.get_retry_min_wait_time(), options.get_retry_max_wait_time());

  int revoke_max_retry_times = options.get_revoke_max_rety_times();
  if (!revoke_max_retry_times) {
    revoke_max_retry_times = revoke_default_max_retry_times;
  }

  int retry_times = 0;
  while (++retry_times <= revoke_max_retry_times) {
    VLOG(1) << LogPrefix << "::Session - Start revoke lease " << lease_;
    proto::LeaseRevokeResponse rsp;
    auto context = GetGRPCClientContext(options.get_get_grpc_client_context_func());
    auto status = client_->LeaseRevoke(context.get(), lease_, &rsp);
    if (status.ok() || status.error_code() == grpc::StatusCode::NOT_FOUND) {
      // Revoked or lease not found both are ok
      VLOG(3) << LogPrefix << "::Session - Revoke succeed for " << lease_;
      return;
    }
    LOG(ERROR) << LogPrefix << "::Session - Failed to revoke lease " << lease_ << " with error: #" << status.error_code() << " " << status.error_message();
    backoff.Reset(false);
    std::this_thread::sleep_for(backoff.Next());
  }
}
