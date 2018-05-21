/* Author: lipixun
 * Created Time : 2018-05-20 17:48:25
 *
 * File Name: kv.h
 * Description:
 *
 *    The kv implementation
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_KV_H_
#define GITHUB_BRAINAAS_ETCDV3Client_KV_H_

#include <string>
#include <vector>

#include "grpcpp/grpcpp.h"

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "common.h"

namespace etcdv3client {

// RangeOptions
class RangeOptions : public Options<etcdserverpb::RangeRequest> {

};

// PutOptions
class PutOptions : public Options<etcdserverpb::PutRequest> {
 public:
  auto WithLease(int64_t lease) -> PutOptions& {
    setfuncs.push_back([lease](etcdserverpb::PutRequest& req) {
      req.set_lease(lease);
    });
    return *this;
  }

  auto WithPrevKV() -> PutOptions& {
    setfuncs.push_back([](etcdserverpb::PutRequest& req) {
      req.set_prev_kv(true);
    });
    return *this;
  }
};

// The kv interface
class KVInterface {
 public:
  virtual ~KVInterface() = default;

  // Get key and values
  auto virtual Range(
    grpc::ClientContext* context,
    const std::string& key,
    etcdserverpb::RangeResponse* response,
    const RangeOptions& options = RangeOptions()
  ) -> grpc::Status = 0;

  // Put a key and a value
  auto virtual Put(
    grpc::ClientContext* context,
    const std::string& key,
    const std::string& value,
    etcdserverpb::PutResponse* response,
    const PutOptions& options = PutOptions()
    ) -> grpc::Status = 0;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_KV_H_
