/* Author: lipixun
 * Created Time : 2018-05-20 17:43:12
 *
 * File Name: session.h
 * Description:
 *
 *    The session implementation, this is a high-level encapsulation of lease
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_SESSION_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_SESSION_H_

#include <memory>
#include <thread>
#include <chrono>
#include <exception>
#include <functional>
#include <condition_variable>

#include "grpcpp/grpcpp.h"

#include "context/context.h"

#include "status.h"
#include "common.h"
#include "proto/proto.h"

namespace brainaas::etcdv3client {

class Client;

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
  auto WithMaxRevokeRetry(int times) -> SessionOptions& {
    revoke_max_retry_times_ = times;
    return *this;
  }
  auto WithRetryWaitTime(std::chrono::seconds min, std::chrono::seconds max) -> SessionOptions& {
    retry_min_wait_time_ = min;
    retry_max_wait_time_ = max;
    return *this;
  }
  auto WithGetGRPCClientContextFunc(GetGRPCClientContextFunction f) -> SessionOptions& {
    get_grpc_client_context_func_ = f;
    return *this;
  }

  auto get_lease() const -> int64_t { return lease_; }
  auto set_lease(int64_t lease) -> void { lease_ = lease; }
  auto get_ttl() const -> int64_t { return ttl_; }
  auto set_ttl(int64_t ttl) -> void { ttl_ = ttl; }
  auto get_revoke_max_rety_times() const -> int { return revoke_max_retry_times_; }
  auto get_retry_min_wait_time() const -> std::chrono::seconds { return retry_min_wait_time_; }
  auto get_retry_max_wait_time() const -> std::chrono::seconds { return retry_max_wait_time_; }
  auto get_get_grpc_client_context_func() const -> GetGRPCClientContextFunction { return get_grpc_client_context_func_; }

 private:
  int64_t lease_;
  int64_t ttl_ = 60;
  int revoke_max_retry_times_;
  std::chrono::seconds retry_min_wait_time_ = std::chrono::seconds(1);
  std::chrono::seconds retry_max_wait_time_ = std::chrono::seconds(60);
  GetGRPCClientContextFunction get_grpc_client_context_func_;
};

class Session {
 public:
  // Create a new Session
  Session(const std::shared_ptr<Client>& client)
      : client_(client),
        alive_(std::make_shared<context::Event>()),
        expired_(std::make_shared<context::Event>(false, true)) {}
  virtual ~Session() { Close(); }
  //
  // Session is neither copyable nor movable.
  //
  Session(const Session&) = delete;
  Session(Session&&) = delete;
  auto operator=(const Session&) -> Session& = delete;

  // Grant the session
  // Args:
  //  ctx(Context): The context object which will cause the session to be closed if done
  // Returns:
  //  Status: The grant result
  auto Grant(const std::shared_ptr<context::Context>& ctx) -> maybecommon::Status {
    return Grant(ctx, SessionOptions());
  }
  // Grant the session
  // Args:
  //  ctx(Context): The context object which will cause the session to be closed if done
  //  options(SessionOptions): The session options
  // Returns:
  //  Status: The grant result
  auto Grant(const std::shared_ptr<context::Context>& ctx,
             const SessionOptions& options) -> maybecommon::Status;
  // Check if this session is alive
  auto IsAlive() const -> bool { return alive_->IsSet(); }
  // Close this session, and also will revoke the lease
  auto Close() -> void;

  // Get the alive event
  auto get_alive_event() const -> const std::shared_ptr<context::Event>& { return alive_; }
  // Get the expired event
  auto get_expired_event() const -> const std::shared_ptr<context::Event>& { return expired_; }
  // Get the client of this session
  auto get_client() const -> const std::shared_ptr<Client>& { return client_; }
  // Get the lease id
  auto get_lease() const -> int64_t { return lease_; }
  // Get if granted
  auto get_granted() const -> bool { return granted_; }
  // Get if closed
  auto get_closed() const -> bool { return closed_.IsSet(); }

  // Create a new session
  auto static New(std::shared_ptr<Client> client) -> std::shared_ptr<Session> {
    return std::make_shared<Session>(client);
  }

 protected:
  // The background worker
  // NOTE: Here we copy session options
  auto virtual GrantAndKeepAlive(std::shared_ptr<context::Context> ctx, SessionOptions options) -> void;
  // Try to grant
  auto virtual TryGrant(const std::shared_ptr<context::Waitable>& close_event, const SessionOptions& options) -> void;
  // Try to keep alive
  auto virtual TryKeepAlive(const std::shared_ptr<context::Waitable>& close_event, const SessionOptions& options) -> void;
  // Try to revoke
  auto virtual TryRevoke(const SessionOptions& options) -> void;
  // Get interval between keep alive requests by lease ttl. TTL is in seconds
  auto virtual GetKeepAliveIntervalByTTL(int64_t ttl) -> std::chrono::seconds {
    auto interval_secs = ttl / 2;
    if (interval_secs <= 0) {
      interval_secs = 1;
    }
    return std::chrono::seconds(interval_secs);
  }

private:

  int64_t lease_;
  int64_t lease_ttl_;

  bool granted_ = false;
  context::Event closed_;

  std::mutex m_;
  std::shared_ptr<Client> client_;
  std::thread grant_and_keep_alive_thread_;
  std::shared_ptr<context::Event> alive_;
  std::shared_ptr<context::Event> expired_;

  const int revoke_default_max_retry_times = 3;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_SESSION_H_
