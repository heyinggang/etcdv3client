/* Author: lipixun
 * Created Time : 2018-05-20 17:42:32
 *
 * File Name: lease.cc
 * Description:
 *
 *    The lease implementation
 *
 */

#include "lease.h"
#include "client.h"

using namespace etcdv3client;

// Lease grant
auto Client::LeaseGrant(grpc::ClientContext* context,
                        int64_t ttl,
                        etcdserverpb::LeaseGrantResponse* response,
                        const LeaseGrantOptions& options) -> grpc::Status {
  etcdserverpb::LeaseGrantRequest req;
  req.set_ttl(ttl);
  options.Apply(req);
  return lease_->LeaseGrant(context, req, response);
}

// Lease revoke
auto Client::LeaseRevoke(grpc::ClientContext* context,
                         int64_t id,
                         etcdserverpb::LeaseRevokeResponse* response) -> grpc::Status {
  etcdserverpb::LeaseRevokeRequest req;
  req.set_id(id);
  return lease_->LeaseRevoke(context, req, response);
}

// Lease keep alive
auto Client::LeaseKeepAlive(grpc::ClientContext* context)
  -> std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::LeaseKeepAliveRequest, etcdserverpb::LeaseKeepAliveResponse>> {
  return lease_->LeaseKeepAlive(context);
}

// Lease time to live
auto Client::LeaseTimeToLive(grpc::ClientContext* context,
                             int64_t id,
                             etcdserverpb::LeaseTimeToLiveResponse* response,
                             const LeaseTimeToLiveOptions& options) -> grpc::Status {
  etcdserverpb::LeaseTimeToLiveRequest req;
  req.set_id(id);
  options.Apply(req);
  return lease_->LeaseTimeToLive(context, req, response);
}

// List leases
auto Client::LeaseLeases(grpc::ClientContext* context, etcdserverpb::LeaseLeasesResponse* response) -> grpc::Status {
  return lease_->LeaseLeases(context, etcdserverpb::LeaseLeasesRequest(), response);
}
