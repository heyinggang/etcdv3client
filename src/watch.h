/* Author: lipixun
 * Created Time : 2018-05-20 17:46:52
 *
 * File Name: watch.h
 * Description:
 *
 *    The watcher implementation
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_WATCH_H_
#define GITHUB_BRAINAAS_ETCDV3Client_WATCH_H_

#include <memory>

#include "grpcpp/grpcpp.h"

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "common.h"

namespace etcdv3client {

// The watch interface
class WatchInterface {
 public:
  // Watch
  auto virtual Watch(grpc::ClientContext* context) ->
    std::unique_ptr<grpc::ClientReaderWriterInterface<etcdserverpb::WatchRequest, etcdserverpb::WatchResponse>> = 0;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_WATCH_H_
