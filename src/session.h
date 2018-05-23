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

#include "grpcpp/grpcpp.h"

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "common.h"
#include "client.h"

namespace etcdv3client {

// The session options
class SessionOptions : Options<etcdserverpb::LeaseGrantRequest> {
 public:
  auto WithTTL(int64_t ttl) -> SessionOptions& {
    setfuncs.push_back([ttl](etcdserverpb::LeaseGrantRequest& req) {
      req.set_ttl(ttl);
    });
    return *this;
  }

  auto WithLease(int64_t id) -> SessionOptions& {
    setfuncs.push_back([id](etcdserverpb::LeaseGrantRequest& req) {
      req.set_id(id);
    });
    return *this;
  }
};

class Session {
 public:
  // Create a new Session
  Session(const std::shared_ptr<Client>& client, const SessionOptions& options = SessionOptions());

  virtual ~Session() {
    Close();
  }

  //
  // Session is neither copyable nor movable.
  //
  Session(const Session&) = delete;
  Session(Session&&) = delete;
  auto operator=(const Session&) -> Session& = delete;

  // Close this session, and also will revoke the lease
  auto Close() -> void;

  // Get the lease id
  auto get_lease_id() -> int64_t {
    return lease_id_;
  }

 private:

  auto KeepAlive() -> void;

  std::shared_ptr<Client> client_;
  const SessionOptions options_;
  int64_t lease_id_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_SESSION_H_
