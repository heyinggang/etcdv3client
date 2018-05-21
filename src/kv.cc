/* Author: lipixun
 * Created Time : 2018-05-20 17:48:37
 *
 * File Name: kv.cc
 * Description:
 *
 *    The kv implementation
 *
 */

#include "kv.h"
#include "client.h"

using namespace etcdv3client;

// Get key and values
auto Client::Range(
  grpc::ClientContext* context,
  const std::string& key,
  etcdserverpb::RangeResponse* response,
  const RangeOptions& options
  ) -> grpc::Status {

    etcdserverpb::RangeRequest req;
    req.set_key(key);

    options.Apply(req);

    return kv_->Range(context, req, response);
  }

// Put
auto Client::Put(
  grpc::ClientContext* context,
  const std::string& key,
  const std::string& value,
  etcdserverpb::PutResponse* response,
  const PutOptions& options
  ) -> grpc::Status {

    etcdserverpb::PutRequest req;
    req.set_key(key);
    req.set_value(value);

    options.Apply(req);

    return kv_->Put(context, req, response);
  }

auto Client::DeleteRange(
  grpc::ClientContext* context,
  const std::string& key,
  etcdserverpb::DeleteRangeResponse* response,
  const DeleteRangeOptions& options
  ) -> grpc::Status {

    etcdserverpb::DeleteRangeRequest req;
    req.set_key(key);

    options.Apply(req);

    return kv_->DeleteRange(context, req, response);
  }
