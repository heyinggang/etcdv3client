/* Author: lipixun
 * Created Time : 2018-05-20 17:44:51
 *
 * File Name: election.cc
 * Description:
 *
 *    The election implementation based on session
 *
 */

#include "election.h"
#include "client.h"

#include <mutex>
#include <algorithm>
#include <functional>
#include <condition_variable>

#include "glog/logging.h"

#include "maybecommon/status.h"

#include "cbase/time/backoff.h"

using namespace brainaas::etcdv3client;


// Get current leader
auto Client::ElectionImpl::GetLeader(grpc::ClientContext* context,
                                     const std::string& key_prefix,
                                     ElectionNode *node) -> maybecommon::Status {
  proto::RangeResponse response;
  auto status = client_->Range(context, key_prefix, &response, RangeOptions().WithFirstCreate());
  if (!status.ok()) {
    return Status(StatusCode::GRPCError).WithValue(status);
  }

  node->set_current_revision(response.header().revision());

  if (response.kvs().size() != 1) {
    return Status().WithValue(status);
  }
  node->set_bykv(response.kvs()[0]);

  return Status().WithValue(status);
}

// Watch watchs the election change
auto Client::ElectionImpl::Watch(const std::shared_ptr<context::Context>& ctx,
                                 const std::string& key_prefix,
                                 ElectionNode* out_leader_node,
                                 const ElectionWatchOptions& options) -> maybecommon::Status {
  brainaas::cbase::time::BackOffStrategy backoff(
    std::chrono::seconds(1),
    std::chrono::seconds(10)
  );

  auto revision = options.get_revision();

  while (true) { // Watch leader loop
    //
    // Get current leader until context done
    //
    while (true) {
      auto context = GetGRPCClientContext(options.get_get_grpc_client_context_function());
      auto status = GetLeader(context.get(), key_prefix, out_leader_node);
      if (!status.ok()) {
        // Retry
        backoff.Reset(false);
        if (ctx->Wait(backoff.Next())) {
          // Done (either cancelled or timeout)
          return Status(maybecommon::StatusCode::Cancelled);
        }
        continue;
      }
      // Good
      break;
    }
    if (out_leader_node->IsValid() && (revision == 0 || revision < out_leader_node->get_create_revision())) {
      return Status();
    }
    //
    // Watch for new leader
    //
    backoff.Reset();
    // Set the revision to watch new leader
    if (out_leader_node->IsValid()) {
      revision = std::max(out_leader_node->get_watch_revision() + 1, revision);
    }
    auto watch_options = WatchOptions().WithPrefix().WithStartRevision(revision);
    auto watch_event = proto::Event_EventType_PUT;
    if (out_leader_node->IsValid()) {
      // Wait for new leader, means watching DELETE event
      watch_options = watch_options.WithNoPut();
      watch_event = proto::Event_EventType_DELETE;
    } else {
      // Wait for first leader, means watching PUT event
      watch_options = watch_options.WithNoDelete();
    }
    // Watch
    bool found_event = false;
    while (true) { // Continue watching event
      auto context = GetGRPCClientContext(options.get_get_grpc_client_context_function());
      auto watch_rw = client_->WatchSingle(context.get(), key_prefix, watch_options);
      while (true) { // Watching in single request
        // NOTE: We donot support timeout until grpc provides a better async mechanism
        proto::WatchResponse watch_rsp;
        if (watch_rw->Read(&watch_rsp)) {
          for (const auto& event : watch_rsp.events()) {
            if (event.type() == watch_event) {
              revision = event.kv().create_revision() - 1;
              found_event = true;
              break;
            }
          }
          if (found_event) {
            watch_rw->Finish(); // Complete the watch request
            break;
          }
          // Event not found, continue watching
        } else {
          // Error happened. Close the current watch request
          auto status = watch_rw->Finish();
          // TODO: Log the status
          // Wait to start another watch request
          backoff.Reset(false);
          if (ctx->Wait(backoff.Next())) {
            // Done (either cancelled or timeout)
            return Status(maybecommon::StatusCode::Cancelled);
          }
          // Start another watch request
          break;
        }
      } // Watching in single request
      if (found_event) {
        break;
      }
    } // Continue watching event
  } // Watch leader loop
}
