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

using namespace etcdv3client;

auto Client::Watch(grpc::ClientContext* context) ->
  std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::WatchRequest, etcdserverpb::WatchResponse>> {
  return watch_->Watch(context);
}
