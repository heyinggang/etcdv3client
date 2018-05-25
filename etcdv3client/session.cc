/* Author: lipixun
 * Created Time : 2018-05-20 17:44:12
 *
 * File Name: session.cc
 * Description:
 *
 *    The session implementation, this is a high-level encapsulation of lease
 *
 */

#include "cbase/time/backoff.h"

#include "session.h"

using namespace brainaas::etcdv3client;

// Grant the session
auto Session::Grant(grpc::ClientContext *context, const SessionOptions& options) -> grpc::Status {
  // Grant the lease
  proto::LeaseGrantResponse response;
  auto status = client_->LeaseGrant(context, options.get_ttl(), &response, LeaseGrantOptions().WithID(options.get_lease()));
  if (!status.ok()) {
    return status;
  }

  lease_ = response.id();
  lease_ttl_ = response.ttl(); // In seconds

  // Start the keep alive thread,
  keep_alive_thread_ = std::thread(&Session::KeepAlive, this, GetKeepAliveIntervalByTTL(lease_ttl_), options);

  return status;
}

auto Session::Close() -> void {
  {
    std::unique_lock<std::mutex> lk(close_cv_m_);
    if (closed_) {
      return;
    }
    closed_ = true;
  }
  close_cv_.notify_all();
  if (keep_alive_thread_.joinable()) {
    keep_alive_thread_.join();
  }
}

auto Session::KeepAlive(std::chrono::seconds interval_secs, const SessionOptions& options) -> void {
  brainaas::cbase::time::BackOffStrategy backoff(
    std::chrono::seconds(1),
    interval_secs,
    interval_secs
    );

  auto set_client_context_func = options.get_set_context_function();

  while (!closed_) {
    grpc::ClientContext context;
    if (set_client_context_func != nullptr) {
      set_client_context_func(&context);
    }
    auto keep_alive_rw = client_->LeaseKeepAlive(&context);
    while (true) {
      {
        std::unique_lock<std::mutex> lk(close_cv_m_);
        if (close_cv_.wait_for(lk, backoff.Next(), [this]{ return closed_; })) {
          // Closed
          break;
        }
      }
      // Write request and wait for response
      proto::LeaseKeepAliveRequest req;
      proto::LeaseKeepAliveResponse rsp;
      req.set_id(lease_);
      if (keep_alive_rw->Write(req) && keep_alive_rw->Read(&rsp)) {
        // Good
        lease_ = rsp.id();
        lease_ttl_ = rsp.ttl();
        backoff.Reset(true);
      } else {
        // Bad, retry with new request
        keep_alive_rw->Finish();
        backoff.Reset(false);
        break;
      }
    }
  }

  // Lease revoke
  int revoke_max_retry_times = options.get_revoke_max_rety_times();
  if (!revoke_max_retry_times) {
    revoke_max_retry_times = revoke_default_max_retry_times;
  }

  int retry_times = 0;

  backoff.Reset(true);
  while (++retry_times <= revoke_max_retry_times) {
    grpc::ClientContext context;
    if (set_client_context_func != nullptr) {
      set_client_context_func(&context);
    }
    proto::LeaseRevokeResponse rsp;
    auto status = client_->LeaseRevoke(&context, lease_, &rsp);
    if (status.ok()) {
      return;
    }
    backoff.Reset(false);
    std::this_thread::sleep_for(backoff.Next());
  }

  // Failed to revoke. This is acceptable since the lease will expire.
}

auto brainaas::etcdv3client::NewSession(std::shared_ptr<Client> client) -> std::shared_ptr<Session> {
  return std::make_shared<Session>(client);
}
