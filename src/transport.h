/* Author: lipixun
 * Created Time : 2018-05-20 17:58:10
 *
 * File Name: transport.h
 * Description:
 *
 *    The transport implementation
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_TRANSPORT_H_
#define GITHUB_BRAINAAS_ETCDV3Client_TRANSPORT_H_

#include <memory>

#include "grpcpp/grpcpp.h"

#include "proto/rpc.grpc.pb.h"

namespace etcdv3client {

// Define the interface of a `Transport`
class TransportInterface {
 public:
  virtual ~TransportInterface() = default;

  auto virtual GetKVService() -> std::shared_ptr<etcdserverpb::KV::StubInterface> = 0;
  auto virtual GetWatchService() -> std::shared_ptr<etcdserverpb::Watch::StubInterface> = 0;
  auto virtual GetLeaseService() -> std::shared_ptr<etcdserverpb::Lease::StubInterface> = 0;
  auto virtual GetClusterService() -> std::shared_ptr<etcdserverpb::Cluster::StubInterface> = 0;
  auto virtual GetMaintenanceService() -> std::shared_ptr<etcdserverpb::Maintenance::StubInterface> = 0;

  auto virtual Close() -> void = 0;
};

// The default `Transport` implementation by gRPC
class Transport : public TransportInterface {
 public:
  Transport(const std::string& target):
    Transport(CreateChannelByTarget(target)) {}
  Transport(const std::shared_ptr<grpc::ChannelInterface>& channel):
    channel_(channel) {}

  // Get the channel of this transport
  auto Channel() noexcept -> const std::shared_ptr<grpc::ChannelInterface>& {
    return channel_;
  }

 protected:
  auto virtual CreateChannelByTarget(const std::string& target) -> std::shared_ptr<grpc::ChannelInterface>&;

 private:
  const std::shared_ptr<grpc::ChannelInterface>& channel_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_TRANSPORT_H_
