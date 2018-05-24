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
  etcdserverpb::LeaseGrantResponse response;
  auto status = client_->LeaseGrant(context, options.get_ttl(), &response, LeaseGrantOptions().WithID(options.get_lease()));
  if (!status.ok()) {
    return status;
  }

  lease_ = response.id();
  lease_ttl_ = response.ttl(); // In seconds

  // Start the keep alive thread
  keep_alive_thread_ = std::thread(&Session::KeepAlive, this, context, GetKeepAliveIntervalByTTL(lease_ttl_), options);

  return status;
}

auto Session::Close() -> void {
  close_cv_.notify_all();
  keep_alive_thread_.join();
}

auto Session::KeepAlive(grpc::ClientContext *context, std::chrono::seconds interval_secs, const SessionOptions& options) -> void {
  // Lease keep alive
  brainaas::cbase::time::BackOffStrategy backoff(
    std::chrono::seconds(1),
    interval_secs,
    interval_secs
    );
  auto keep_alive_rw = client_->LeaseKeepAlive(context);
  while (true) {
    std::unique_lock<std::mutex> lk(close_cv_m_);
    if (close_cv_.wait_for(lk, backoff.Next()) == std::cv_status::no_timeout) {
      // Closed
      break;
    }
    // Write request and wait for response
    etcdserverpb::LeaseKeepAliveRequest req;
    etcdserverpb::LeaseKeepAliveResponse rsp;
    req.set_id(lease_);
    if (keep_alive_rw->Write(req) && keep_alive_rw->Read(&rsp)) {
      // Good
      lease_ = rsp.id();
      lease_ttl_ = rsp.ttl();
      backoff.Reset(GetKeepAliveIntervalByTTL(lease_ttl_));
    } else {
      // Bad, try
      backoff.Reset();
    }
  }

  // Lease revoke
  int revoke_max_retry_times = options.get_revoke_max_rety_times();
  if (!revoke_max_retry_times) {
    revoke_max_retry_times = revoke_default_max_retry_times;
  }

  int retry_times = 0;

  backoff.Reset();
  while (++retry_times < revoke_max_retry_times) {
    etcdserverpb::LeaseRevokeResponse rsp;
    auto status = client_->LeaseRevoke(context, lease_, &rsp);
    if (status.ok()) {
      return;
    }
    std::this_thread::sleep_for(backoff.Next());
  }

  // Failed to revoke. This is acceptable since the lease will expire.
}
