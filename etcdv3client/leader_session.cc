/* Author: lipixun
 * Created Time : 2018-06-27 23:31:08
 *
 * File Name: leader_session.cc
 * Description:
 *
 *    The leader session
 *
 */

#include "leader_session.h"
#include "client.h"

#include "glog/logging.h"

using namespace brainaas::etcdv3client;

auto Client::ElectionImpl::Campaign(const std::shared_ptr<context::Context>& ctx,
                                    const std::string& key_prefix,
                                    const std::string& value,
                                    const std::shared_ptr<Session>& session,
                                    const CampaignOptions& options) -> std::shared_ptr<LeaderSession> {
  auto leader_session = std::make_shared<LeaderSession>(session, key_prefix, value);
  if (!options.get_donot_auto_start()) {
    leader_session->StartCampaign(ctx, options);
  }
  return leader_session;
}

auto LeaderSession::StartCampaign(const std::shared_ptr<context::Context>& ctx, const CampaignOptions& options) -> void {

}

auto LeaderSession::Proclaim(grpc::ClientContext* context, const std::string& value) -> maybecommon::Status {

}

auto LeaderSession::Resign(grpc::ClientContext* context) -> maybecommon::Status {

}

auto LeaderSession::Stop(grpc::ClientContext* context) -> maybecommon::Status {

}

auto LeaderSession::CampaignAndWatch(std::shared_ptr<context::Context> ctx) {

}
