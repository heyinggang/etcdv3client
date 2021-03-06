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

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_CLIENT_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_CLIENT_H_

#include <string>
#include <memory>

#include "proto/proto.h"

#include "transport.h"
#include "kv.h"
#include "watch.h"
#include "lease.h"
#include "election.h"

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
             proto::RangeResponse* response,
             const RangeOptions& options = RangeOptions()) -> grpc::Status override;

  // Put key and value
  auto Put(grpc::ClientContext* context,
           const std::string& key,
           const std::string& value,
           proto::PutResponse* response,
           const PutOptions& options = PutOptions()) -> grpc::Status override;

  // Delete key(s)
  auto DeleteRange(grpc::ClientContext* context,
                   const std::string& key,
                   proto::DeleteRangeResponse* response,
                   const DeleteRangeOptions& options = DeleteRangeOptions()) -> grpc::Status override;

  // Txn
  auto Txn(grpc::ClientContext* context,
           const ::brainaas::etcdv3client::Txn& txn,
           proto::TxnResponse* response) -> grpc::Status override;

  // Compact
  auto Compact(grpc::ClientContext* context,
               int64_t revision,
               proto::CompactionResponse* response,
               const CompactOptions& options = CompactOptions()) -> grpc::Status override;

  //
  //
  // The watch interface
  //
  //

  // Watch
  auto Watch(grpc::ClientContext* context) ->
    std::unique_ptr<grpc::ClientReaderWriterInterface<proto::WatchRequest, proto::WatchResponse>> override;

  // Watch a single key (range)
  auto WatchSingle(grpc::ClientContext* context,
                   const std::string& key,
                   const WatchOptions& options = WatchOptions()) -> std::unique_ptr<grpc::ClientReaderWriterInterface<proto::WatchRequest, proto::WatchResponse>> override;

  //
  //
  // The lease interface
  //
  //

  // Lease grant
  auto LeaseGrant(grpc::ClientContext* context,
                  int64_t ttl,
                  proto::LeaseGrantResponse* response,
                  const LeaseGrantOptions& options = LeaseGrantOptions()) -> grpc::Status override;
  // Lease revoke
  auto LeaseRevoke(grpc::ClientContext* context,
                   int64_t id,
                   proto::LeaseRevokeResponse* response) -> grpc::Status override;
  // Lease keep alive
  auto LeaseKeepAlive(grpc::ClientContext* context)
    -> std::unique_ptr<grpc::ClientReaderWriterInterface<proto::LeaseKeepAliveRequest, proto::LeaseKeepAliveResponse>> override;
  // Lease time to live
  auto LeaseTimeToLive(grpc::ClientContext* context,
                       int64_t id,
                       proto::LeaseTimeToLiveResponse* response,
                       const LeaseTimeToLiveOptions& options = LeaseTimeToLiveOptions()) -> grpc::Status override;
  // List leases
  auto LeaseLeases(grpc::ClientContext* context, proto::LeaseLeasesResponse* response) -> grpc::Status override;

  //
  //  Advanced apis
  //
  auto Election() -> ElectionInterface& {
    return *election_;
  }

 protected:

  // Election implements the ElectionInterface
  class ElectionImpl : public ElectionInterface {
   public:
    ElectionImpl(Client* client) : client_(client) {}

    // GetLeader gets the leader node
    auto GetLeader(grpc::ClientContext* context,
                   const std::string& key_prefix,
                   ElectionNode* out_leader_node) -> maybecommon::Status override;

    // Watch watchs the election change
    auto Watch(const std::shared_ptr<context::Context>& ctx,
               const std::string& key_prefix,
               ElectionNode* out_leader_node) -> maybecommon::Status override {
      return Watch(ctx, key_prefix, out_leader_node, ElectionWatchOptions());
    }
    auto Watch(const std::shared_ptr<context::Context>& ctx,
               const std::string& key_prefix,
               ElectionNode* out_leader_node,
               const ElectionWatchOptions& options) -> maybecommon::Status override;

    // Campaign puts a value as eligible for the election
    // This is a blocking method which will wait for the leadership or context cancelled
    // Args:
    //  value: The value to claim
    // Returns:
    //  Tuple<shared_ptr<LeaderSession>, maybecommon::Status>
    auto Campaign(const std::shared_ptr<context::Context>& ctx,
                  const std::string& key_prefix,
                  const std::string& value,
                  const std::shared_ptr<Session>& session) -> std::shared_ptr<LeaderSession> override {
      return Campaign(ctx, key_prefix, value, session, CampaignOptions());
    }
    auto Campaign(const std::shared_ptr<context::Context>& ctx,
                  const std::string& key_prefix,
                  const std::string& value,
                  const std::shared_ptr<Session>& session,
                  const CampaignOptions& options) -> std::shared_ptr<LeaderSession> override;
   private:
    Client* client_;
  };

 private:

  // The transport object. `Client` has the ownership of this object.
  std::unique_ptr<TransportInterface> transport_;

  // The stubs
  std::unique_ptr<proto::KV::Stub> kv_;
  std::unique_ptr<proto::Watch::Stub> watch_;
  std::unique_ptr<proto::Lease::Stub> lease_;
  std::unique_ptr<proto::Cluster::Stub> cluster_;
  std::unique_ptr<proto::Maintenance::Stub> maintenance_;
  std::unique_ptr<proto::Auth::Stub> auth_;

  // Advanced api
  std::unique_ptr<ElectionImpl> election_;
};

auto New(const std::string& target) -> std::shared_ptr<Client>;

auto New(std::unique_ptr<TransportInterface>& transport) -> std::shared_ptr<Client>;

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_CLIENT_H_
