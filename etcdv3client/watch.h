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

#include <string>
#include <memory>

#include "grpcpp/grpcpp.h"

#include "proto/proto.h"

#include "common.h"

namespace brainaas::etcdv3client {

class WatchOptions : public Options<proto::WatchCreateRequest> {
 public:
  auto WithRangeEnd(const std::string& end) -> WatchOptions& {
    setfuncs.push_back([end](proto::WatchCreateRequest& req) {
      req.set_range_end(end);
    });
    return *this;
  }

  auto WithStartRevision(int64_t revision) -> WatchOptions& {
    setfuncs.push_back([revision](proto::WatchCreateRequest& req) {
      req.set_start_revision(revision);
    });
    return *this;
  }

  auto WithProgressNotify() -> WatchOptions& {
    setfuncs.push_back([](proto::WatchCreateRequest& req) {
      req.set_progress_notify(true);
    });
    return *this;
  }

  auto WithNoPut() -> WatchOptions& {
    setfuncs.push_back([](proto::WatchCreateRequest& req) {
      req.add_filters(proto::WatchCreateRequest_FilterType_NOPUT);
    });
    return *this;
  }

  auto WithNoDelete() -> WatchOptions& {
    setfuncs.push_back([](proto::WatchCreateRequest& req) {
      req.add_filters(proto::WatchCreateRequest_FilterType_NODELETE);
    });
    return *this;
  }

  auto WithPrevKV() -> WatchOptions& {
    setfuncs.push_back([](proto::WatchCreateRequest& req) {
      req.set_prev_kv(true);
    });
    return *this;
  }

  auto WithWatchID(int64_t id) -> WatchOptions& {
    setfuncs.push_back([id](proto::WatchCreateRequest& req) {
      req.set_watch_id(id);
    });
    return *this;
  }

  auto WithFragment() -> WatchOptions& {
    setfuncs.push_back([](proto::WatchCreateRequest& req) {
      req.set_fragment(true);
    });
    return *this;
  }
};

// The watch interface
class WatchInterface {
 public:
  // Watch
  auto virtual Watch(grpc::ClientContext* context) ->
    std::unique_ptr<grpc::ClientReaderWriterInterface<proto::WatchRequest, proto::WatchResponse>> = 0;

  // Watch a single key (range)
  auto virtual WatchSingle(grpc::ClientContext* context,
                           const std::string& key,
                           const WatchOptions& options = WatchOptions()) -> std::unique_ptr<grpc::ClientReaderWriterInterface<proto::WatchRequest, proto::WatchResponse>> = 0;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_WATCH_H_
