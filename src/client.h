/* Author: lipixun
 * Created Time : 2018-05-20 17:45:02
 *
 * File Name: client.h
 * Description:
 *
 *    The client
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_
#define GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_

#include <string>
#include <memory>

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "transport.h"
#include "kv.h"

namespace etcdv3client {

// The etcdv3 client class
class Client final : public KVInterface {
 public:

  // Create a new Client by grpc URI
  Client(const std::string& target):
    Client(std::unique_ptr<TransportInterface>(new Transport(target))) {}

  // Create a new Client by transport interface
  // NOTE:
  //  After this ctor, `Client` will take the ownership of `transport`
  Client(std::unique_ptr<TransportInterface>&& transport):
    transport_(std::move(transport)) {
      // Create stubs
      kv_ = etcdserverpb::KV::NewStub(transport_->GetChannel());
      watch_ = etcdserverpb::Watch::NewStub(transport_->GetChannel());
      lease_ = etcdserverpb::Lease::NewStub(transport_->GetChannel());
      cluster_ = etcdserverpb::Cluster::NewStub(transport_->GetChannel());
      maintenance_ = etcdserverpb::Maintenance::NewStub(transport_->GetChannel());
      auth_ = etcdserverpb::Auth::NewStub(transport_->GetChannel());
    }

  //
  // Client is neither copyable nor movable.
  //
  Client(const Client&) = delete;
  Client(Client&&) = delete;
  auto operator=(const Client&) -> Client& = delete;

  //
  // The KV interface
  //

  // Get key and values
  auto Range(
    grpc::ClientContext* context,
    const std::string& key,
    etcdserverpb::RangeResponse* response,
    const RangeOptions& options = RangeOptions()
  ) -> grpc::Status override;

  // Put
  auto Put(
    grpc::ClientContext* context,
    const std::string& key,
    const std::string& value,
    etcdserverpb::PutResponse* response,
    const PutOptions& options = PutOptions()
    ) -> grpc::Status override;

  // Delete
  auto DeleteRange(
    grpc::ClientContext* context,
    const std::string& key,
    etcdserverpb::DeleteRangeResponse* response,
    const DeleteRangeOptions& options = DeleteRangeOptions()
  ) -> grpc::Status override;

 private:

  // The transport object. `Client` has the ownership of this object.
  std::unique_ptr<TransportInterface> transport_;

  // The stubs
  std::unique_ptr<etcdserverpb::KV::Stub> kv_;
  std::unique_ptr<etcdserverpb::Watch::Stub> watch_;
  std::unique_ptr<etcdserverpb::Lease::Stub> lease_;
  std::unique_ptr<etcdserverpb::Cluster::Stub> cluster_;
  std::unique_ptr<etcdserverpb::Maintenance::Stub> maintenance_;
  std::unique_ptr<etcdserverpb::Auth::Stub> auth_;

};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_
