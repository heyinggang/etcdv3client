/* Author: lipixun
 * Created Time : 2018-05-20 17:43:12
 *
 * File Name: session.h
 * Description:
 *
 *    The session implementation, this is a high-level encapsulation of lease
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_SESSION_H_
#define GITHUB_BRAINAAS_ETCDV3Client_SESSION_H_

#include <memory>
#include <thread>
#include <chrono>
#include <functional>
#include <condition_variable>

#include "grpcpp/grpcpp.h"

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "common.h"
#include "client.h"

namespace brainaas::etcdv3client {

// The session options
class SessionOptions {
 public:
  auto WithTTL(int64_t ttl) -> SessionOptions& {
    ttl_ = ttl;
    return *this;
  }

  auto WithLease(int64_t id) -> SessionOptions& {
    lease_ = id;
    return *this;
  }

  auto WithMaxRevokeRetry(int times) -> SessionOptions {
    revoke_max_retry_times_ = times;
    return *this;
  }

  auto get_lease() const -> int64_t { return lease_; }
  auto get_ttl() const -> int64_t { return ttl_; }
  auto get_revoke_max_rety_times() const -> int { return revoke_max_retry_times_; }

 private:
  int64_t lease_;
  int64_t ttl_;
  int revoke_max_retry_times_;
};

class Session {
 public:
  // Create a new Session
  Session(const std::shared_ptr<Client>& client) : client_(client) {}
  Session(Session&&) = default;
  virtual ~Session() { Close(); }
  //
  // Session can not be copied
  //
  Session(const Session&) = delete;
  auto operator=(const Session&) -> Session& = delete;

  // Grant the session
  auto Grant(grpc::ClientContext *context, const SessionOptions& options = SessionOptions()) -> grpc::Status;
  // Close this session, and also will revoke the lease
  auto Close() -> void;
  // Get the lease id
  auto get_lease() -> int64_t {
    return lease_;
  }

 private:

  // Keep alive thread worker
  auto KeepAlive(grpc::ClientContext *context, std::chrono::seconds interval_secs, const SessionOptions& options) -> void;
  // Get interval between keep alive requests by lease ttl. TTL is in seconds
  auto GetKeepAliveIntervalByTTL(int64_t ttl) -> std::chrono::seconds {
    auto interval_secs = ttl / 2;
    if (interval_secs <= 0) {
      interval_secs = 1;
    }
    return std::chrono::seconds(interval_secs);
  }

  std::shared_ptr<Client> client_;

  int64_t lease_;
  int64_t lease_ttl_;

  std::thread keep_alive_thread_;

  std::mutex close_cv_m_;
  std::condition_variable close_cv_;
  const int revoke_default_max_retry_times = 3;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_SESSION_H_
