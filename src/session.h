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

#include "client.h"

namespace etcdv3client {

class Session {
 public:
  // Create a new Session
  Session(const std::shared_ptr<Client>& client): client_(client) {};

  //
  // Session is neither copyable nor movable.
  //
  Session(const Session&) = delete;
  Session(Session&&) = delete;
  auto operator=(const Session&) -> Session& = delete;

 private:
  std::shared_ptr<Client> client_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_SESSION_H_
