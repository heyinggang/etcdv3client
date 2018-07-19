/* Author: lipixun
 * Created Time : 2018-05-20 17:42:20
 *
 * File Name: lease.h
 * Description:
 *
 *    The lease implementation
 *
 */


#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_LEASE_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_LEASE_H_

#include <string>
#include <memory>

#include "grpcpp/grpcpp.h"

#include "proto/proto.h"

#include "common.h"

namespace brainaas::etcdv3client {

const int64_t InvalidLeaseID = 0;

class LeaseGrantOptions : public Options<proto::LeaseGrantRequest> {
 public:
  auto WithID(int64_t id) -> LeaseGrantOptions& {
    setfuncs.push_back([id](proto::LeaseGrantRequest& req) {
      req.set_id(id);
    });
    return *this;
  }
};

class LeaseTimeToLiveOptions : public Options<proto::LeaseTimeToLiveRequest> {
 public:
  auto WithKeys() -> LeaseTimeToLiveOptions& {
    setfuncs.push_back([](proto::LeaseTimeToLiveRequest& req) {
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
                          proto::LeaseGrantResponse* response,
                          const LeaseGrantOptions& options = LeaseGrantOptions()) -> grpc::Status = 0;
  // Lease revoke
  auto virtual LeaseRevoke(grpc::ClientContext* context,
                          int64_t id,
                          proto::LeaseRevokeResponse* response) -> grpc::Status = 0;
  // Lease keep alive
  auto virtual LeaseKeepAlive(grpc::ClientContext* context)
    -> std::unique_ptr<grpc::ClientReaderWriterInterface<proto::LeaseKeepAliveRequest, proto::LeaseKeepAliveResponse>> = 0;
  // Lease time to live
  auto virtual LeaseTimeToLive(grpc::ClientContext* context,
                               int64_t id,
                               proto::LeaseTimeToLiveResponse* response,
                               const LeaseTimeToLiveOptions& options = LeaseTimeToLiveOptions()) -> grpc::Status = 0;
  // List leases
  auto virtual LeaseLeases(grpc::ClientContext* context, proto::LeaseLeasesResponse* response) -> grpc::Status = 0;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_LEASE_H_
