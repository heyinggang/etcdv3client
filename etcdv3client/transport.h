/* Author: lipixun
 * Created Time : 2018-05-20 17:58:10
 *
 * File Name: transport.h
 * Description:
 *
 *    The transport implementation
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_TRANSPORT_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_TRANSPORT_H_

#include <memory>

#include "grpcpp/grpcpp.h"

namespace brainaas::etcdv3client {

// Define the interface of a `Transport`.
// The transport is used to create the underlying grpc channel
class TransportInterface {
 public:
  TransportInterface() = default;

  //
  // TransportInterface is neither copyable nor movable.
  //
  TransportInterface(const TransportInterface&) = delete;
  TransportInterface(TransportInterface&&) = delete;
  auto operator=(const TransportInterface&) -> TransportInterface& = delete;

  virtual ~TransportInterface() = default;

  auto virtual GetChannel() -> std::shared_ptr<grpc::ChannelInterface> = 0;
};

// The default `Transport` implementation which either receivces:
// * A grpc target uri
// * A grpc channel
class Transport : public TransportInterface {
 public:
  // Create a new Transport by grpc target uri
  Transport(const std::string& target):
    Transport(grpc::CreateChannel(target, grpc::InsecureChannelCredentials())) {}

  // Create a new Transport by channel ptr
  Transport(const std::shared_ptr<grpc::ChannelInterface>& channel):
    channel_(channel) {}

  auto GetChannel() -> std::shared_ptr<grpc::ChannelInterface> override {
    return channel_;
  }

 private:
  const std::shared_ptr<grpc::ChannelInterface> channel_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_TRANSPORT_H_
