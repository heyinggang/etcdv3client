/* Author: lipixun
 * Created Time : 2018-05-20 17:42:20
 *
 * File Name: lease.h
 * Description:
 *
 *    The lease implementation
 *
 */


#ifndef GITHUB_BRAINAAS_ETCDV3Client_LEASE_H_
#define GITHUB_BRAINAAS_ETCDV3Client_LEASE_H_

#include <string>
#include <memory>

#include "grpcpp/grpcpp.h"

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "common.h"

namespace etcdv3client {

class LeaseGrantOptions : public Options<etcdserverpb::LeaseGrantRequest> {
 public:
  auto WithID(int64_t id) -> LeaseGrantOptions& {
    setfuncs.push_back([id](etcdserverpb::LeaseGrantRequest& req) {
      req.set_id(id);
    });
    return *this;
  }
};

class LeaseTimeToLiveOptions : public Options<etcdserverpb::LeaseTimeToLiveRequest> {
 public:
  auto WithKeys() -> LeaseTimeToLiveOptions& {
    setfuncs.push_back([](etcdserverpb::LeaseTimeToLiveRequest& req) {
      req.set_keys(true);
    });
    return *this;
  }
};

// The lease interface
class LeaseInterface {
 public:
  // Lease grant
  auto virtual LeaseGrant(grpc::ClientContext* context,
                          int64_t ttl,
                          etcdserverpb::LeaseGrantResponse* response,
                          const LeaseGrantOptions& options = LeaseGrantOptions()) -> grpc::Status = 0;
  // Lease revoke
  auto virtual LeaseRevoke(grpc::ClientContext* context,
                          int64_t id,
                          etcdserverpb::LeaseRevokeResponse* response) -> grpc::Status = 0;
  // Lease keep alive
  auto virtual LeaseKeepAlive(grpc::ClientContext* context)
    -> std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::LeaseKeepAliveRequest, etcdserverpb::LeaseKeepAliveResponse>> = 0;
  // Lease time to live
  auto virtual LeaseTimeToLive(grpc::ClientContext* context,
                               int64_t id,
                               etcdserverpb::LeaseTimeToLiveResponse* response,
                               const LeaseTimeToLiveOptions& options = LeaseTimeToLiveOptions()) -> grpc::Status = 0;
  // List leases
  auto virtual LeaseLeases(grpc::ClientContext* context, etcdserverpb::LeaseLeasesResponse* response) -> grpc::Status = 0;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3ClientLEASEH_H_
