/* Author: lipixun
 * Created Time : 2018-06-27 23:29:46
 *
 * File Name: leader_session.h
 * Description:
 *
 *    The leader session
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_LEADER_SESSION_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_LEADER_SESSION_H_

#include <memory>

#include "glog/logging.h"

#include "context/context.h"

#include "status.h"
#include "common.h"
#include "election.h"

namespace brainaas::etcdv3client {

// The leader session
class LeaderSession {
 public:
  LeaderSession(const std::shared_ptr<Session>& session, const std::string& key_prefix, const std::string& value)
      : session_(session),
        resign_event_(context::Event(true, false)),
        stop_event_(context::Event(true, false)),
        elected_event_(std::make_shared<context::Event>()),
        lost_event_(std::make_shared<context::Event>(false, true)),
        key_prefix_(key_prefix),
        leader_value_(value) {}
  // Deconstruct the LeaderSession
  virtual ~LeaderSession() {
    // Resign
    grpc::ClientContext context;
    auto status = Stop(&context);
    if (!status.ok()) {
      LOG(ERROR) << LogPrefix << "::LeaderSession - Failed to stop when finializing with error:" << status.String();
    }
  }

  // Start campaign. This method will not block the thread.
  // This method need not be called in most cases, except when `Resign` is called.
  auto StartCampaign(const std::shared_ptr<context::Context>& ctx) -> void {
    return StartCampaign(ctx, CampaignOptions());
  }
  auto StartCampaign(const std::shared_ptr<context::Context>& ctx, const CampaignOptions& options) -> void;
  // NewLeaderContext creates a new Context object which will be finished when leader identity is lost
  // NOTE:
  //  The Context is `Finished` when the leader identity has not been assigned yet.
  auto NewLeaderContext() const -> std::shared_ptr<context::Context> {
    return NewLeaderContext(nullptr);
  }
  auto NewLeaderContext(const std::shared_ptr<context::Context>& ctx) const -> std::shared_ptr<context::Context> {
    return context::Context::WithWaitable(get_lost_event().get(), ctx);
  }
  // Is leader
  auto IsLeader() const -> bool { return elected_event_->IsSet(); }
  // Proclaim lets the leader announce a new value without another election
  // The `value` this class holds will change after the value is successfully changed in etcd
  auto Proclaim(grpc::ClientContext* context, const std::string& value) -> maybecommon::Status;
  // Resign lets a leader start a new election
  auto Resign(grpc::ClientContext* context) -> maybecommon::Status;
  // Stop this session (Stop campaign and resign if could)
  auto Stop(grpc::ClientContext* context) -> maybecommon::Status;

  // Get the session
  auto get_session() -> const std::shared_ptr<Session>& { return session_; }
  // Get key prefix
  auto get_key_prefix() const -> const std::string& { return key_prefix_; }
  // Get election value
  auto get_value() const -> const std::string& { return leader_value_; }
  // Wait for elected event
  auto get_elected_event() const -> const std::shared_ptr<context::Event>& { return elected_event_; }
  // Wait for lost event
  auto get_lost_event() const -> const std::shared_ptr<context::Event>& { return lost_event_; }

 protected:
  // Cmpaign and watch for the changes. This is main `working` thread
  auto CampaignAndWatch(std::shared_ptr<context::Context> ctx);

 private:
  std::mutex m_;
  std::thread campaign_and_watch_thread_;
  std::shared_ptr<Session> session_;
  context::Event resign_event_;
  context::Event stop_event_;
  std::shared_ptr<context::Event> elected_event_;
  std::shared_ptr<context::Event> lost_event_;
  std::string key_prefix_;
  std::string leader_key_;
  std::string leader_value_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_LEADER_SESSION_H_
