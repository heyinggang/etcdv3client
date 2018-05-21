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
  auto virtual GetAuthService() -> std::shared_ptr<etcdserverpb::Auth::StubInterface> = 0;
};

// The default `Transport` implementation by gRPC
class Transport : public TransportInterface {
 public:
  Transport(const std::string& target):
    Transport(CreateChannelByTarget(target)) {}
  Transport(const std::shared_ptr<grpc::ChannelInterface>& channel):
    channel_(channel),
    kv_(std::make_shared<etcdserverpb::KV::Stub>(channel)),
    watch_(std::make_shared<etcdserverpb::Watch::Stub>(channel)),
    lease_(std::make_shared<etcdserverpb::Lease::Stub>(channel)),
    cluster_(std::make_shared<etcdserverpb::Cluster::Stub>(channel)),
    maintenance_(std::make_shared<etcdserverpb::Maintenance::Stub>(channel)),
    auth_(std::make_shared<etcdserverpb::Auth::Stub>(channel)) {}

  // Get the channel of this transport
  auto Channel() noexcept -> const std::shared_ptr<grpc::ChannelInterface>& {
    return channel_;
  }

  auto GetKVService() -> std::shared_ptr<etcdserverpb::KV::StubInterface> override {
    return kv_;
  }

  auto GetWatchService() -> std::shared_ptr<etcdserverpb::Watch::StubInterface> override {
    return watch_;
  }

  auto GetLeaseService() -> std::shared_ptr<etcdserverpb::Lease::StubInterface> override {
    return lease_;
  }

  auto GetClusterService() -> std::shared_ptr<etcdserverpb::Cluster::StubInterface> override {
    return cluster_;
  }

  auto GetMaintenanceService() -> std::shared_ptr<etcdserverpb::Maintenance::StubInterface> override {
    return maintenance_;
  }
  auto GetAuthService() -> std::shared_ptr<etcdserverpb::Auth::StubInterface> override {
    return auth_;
  }

 protected:
  auto virtual CreateChannelByTarget(const std::string& target) -> std::shared_ptr<grpc::ChannelInterface>&;

 private:
  const std::shared_ptr<grpc::ChannelInterface>& channel_;

  std::shared_ptr<etcdserverpb::KV::Stub> kv_;
  std::shared_ptr<etcdserverpb::Watch::Stub> watch_;
  std::shared_ptr<etcdserverpb::Lease::Stub> lease_;
  std::shared_ptr<etcdserverpb::Cluster::Stub> cluster_;
  std::shared_ptr<etcdserverpb::Maintenance::Stub> maintenance_;
  std::shared_ptr<etcdserverpb::Auth::Stub> auth_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_TRANSPORT_H_
