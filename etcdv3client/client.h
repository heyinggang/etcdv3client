/* Author: lipixun
 * Created Time : 2018-05-20 17:45:02
 *
 * File Name: client.h
 * Description:
 *
 *    The client interface
 *
 *    NOTE: This file MUST not provide any implementation except inline
 *          outside class in order to avoid linking errors.
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
#include "watch.h"
#include "lease.h"

namespace brainaas::etcdv3client {

// The etcdv3 client class
class Client final : public KVInterface, public WatchInterface, public LeaseInterface {
 public:

  // Create a new Client by grpc URI
  Client(const std::string& target);
  // Create a new Client by transport interface
  // NOTE:
  //  After this ctor, `Client` will take the ownership of `transport`
  Client(std::unique_ptr<TransportInterface>& transport);
  // Create a new Client by transport interface
  // NOTE:
  //  After this ctor, `Client` will take the ownership of `transport`
  Client(std::unique_ptr<TransportInterface>&& transport);

  //
  // Client is neither copyable nor movable.
  //
  Client(const Client&) = delete;
  Client(Client&&) = delete;
  auto operator=(const Client&) -> Client& = delete;

  //
  //
  // The KV interface
  //
  //

  // Get key and values
  auto Range(grpc::ClientContext* context,
             const std::string& key,
             etcdserverpb::RangeResponse* response,
             const RangeOptions& options = RangeOptions()) -> grpc::Status override;

  // Put key and value
  auto Put(grpc::ClientContext* context,
           const std::string& key,
           const std::string& value,
           etcdserverpb::PutResponse* response,
           const PutOptions& options = PutOptions()) -> grpc::Status override;

  // Delete key(s)
  auto DeleteRange(grpc::ClientContext* context,
                   const std::string& key,
                   etcdserverpb::DeleteRangeResponse* response,
                   const DeleteRangeOptions& options = DeleteRangeOptions()) -> grpc::Status override;

  // Txn
  auto Txn(grpc::ClientContext* context,
           const ::brainaas::etcdv3client::Txn& txn,
           etcdserverpb::TxnResponse* response) -> grpc::Status override;

  // Compact
  auto Compact(grpc::ClientContext* context,
               int64_t revision,
               etcdserverpb::CompactionResponse* response,
               const CompactOptions& options = CompactOptions()) -> grpc::Status override;

  //
  //
  // The watch interface
  //
  //

  // Watch
  auto Watch(grpc::ClientContext* context) ->
    std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::WatchRequest, etcdserverpb::WatchResponse>> override;

  // Watch a single key (range)
  auto WatchSingle(grpc::ClientContext* context,
                   const std::string& key,
                   const WatchOptions& options = WatchOptions()) -> std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::WatchRequest, etcdserverpb::WatchResponse>> override;

  //
  //
  // The lease interface
  //
  //

  // Lease grant
  auto LeaseGrant(grpc::ClientContext* context,
                  int64_t ttl,
                  etcdserverpb::LeaseGrantResponse* response,
                  const LeaseGrantOptions& options = LeaseGrantOptions()) -> grpc::Status override;
  // Lease revoke
  auto LeaseRevoke(grpc::ClientContext* context,
                   int64_t id,
                   etcdserverpb::LeaseRevokeResponse* response) -> grpc::Status override;
  // Lease keep alive
  auto LeaseKeepAlive(grpc::ClientContext* context)
    -> std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::LeaseKeepAliveRequest, etcdserverpb::LeaseKeepAliveResponse>> override;
  // Lease time to live
  auto LeaseTimeToLive(grpc::ClientContext* context,
                       int64_t id,
                       etcdserverpb::LeaseTimeToLiveResponse* response,
                       const LeaseTimeToLiveOptions& options = LeaseTimeToLiveOptions()) -> grpc::Status override;
  // List leases
  auto LeaseLeases(grpc::ClientContext* context, etcdserverpb::LeaseLeasesResponse* response) -> grpc::Status override;

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

auto New(const std::string& target) -> std::shared_ptr<Client>;

auto New(std::unique_ptr<TransportInterface>& transport) -> std::shared_ptr<Client>;

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_