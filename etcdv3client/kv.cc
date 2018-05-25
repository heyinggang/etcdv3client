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

using namespace brainaas::etcdv3client;

auto Client::Range(grpc::ClientContext* context,
                   const std::string& key,
                   proto::RangeResponse* response,
                   const RangeOptions& options) -> grpc::Status {
  proto::RangeRequest req;
  req.set_key(key);
  options.Apply(req);
  return kv_->Range(context, req, response);
}

auto Client::Put(grpc::ClientContext* context,
                 const std::string& key,
                 const std::string& value,
                 proto::PutResponse* response,
                 const PutOptions& options) -> grpc::Status {
  proto::PutRequest req;
  req.set_key(key);
  req.set_value(value);
  options.Apply(req);
  return kv_->Put(context, req, response);
}

auto Client::DeleteRange(grpc::ClientContext* context,
                         const std::string& key,
                         proto::DeleteRangeResponse* response,
                         const DeleteRangeOptions& options) -> grpc::Status {
  proto::DeleteRangeRequest req;
  req.set_key(key);
  options.Apply(req);
  return kv_->DeleteRange(context, req, response);
}

// Txn
auto Client::Txn(grpc::ClientContext* context,
                 const ::brainaas::etcdv3client::Txn& txn,
                 proto::TxnResponse* response) -> grpc::Status {
  return kv_->Txn(context, txn.GetRequest(), response);
}

auto Client::Compact(grpc::ClientContext* context,
                    int64_t revision,
                    proto::CompactionResponse* response,
                    const CompactOptions& options) -> grpc::Status {
  proto::CompactionRequest req;
  req.set_revision(revision);
  options.Apply(req);
  return kv_->Compact(context, req, response);
}
