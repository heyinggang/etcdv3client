/* Author: lipixun
 * Created Time : 2018-05-20 17:47:14
 *
 * File Name: watch.cc
 * Description:
 *
 *    The watch implementation
 *
 */

#include "watch.h"
#include "client.h"

using namespace brainaas::etcdv3client;

auto Client::Watch(grpc::ClientContext* context) ->
  std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::WatchRequest, etcdserverpb::WatchResponse>> {
  return watch_->Watch(context);
}

auto Client::WatchSingle(grpc::ClientContext* context,
                         const std::string& key,
                         const WatchOptions& options) -> std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::WatchRequest, etcdserverpb::WatchResponse>> {
  auto return_ = watch_->Watch(context);

  etcdserverpb::WatchRequest req;
  req.mutable_create_request()->set_key(key);
  options.Apply(*req.mutable_create_request());

  return_->WriteLast(req, grpc::WriteOptions());
  return return_;
}
