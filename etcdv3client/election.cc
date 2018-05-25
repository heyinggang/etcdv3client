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

using namespace brainaas::etcdv3client;

auto brainaas::etcdv3client::NewElection(const std::string& key_prefix, std::shared_ptr<Client> client) -> std::shared_ptr<Election> {
  return std::make_shared<Election>(key_prefix, client);
}

auto brainaas::etcdv3client::NewLeaderElection(const std::string& key_prefix, const std::shared_ptr<Session> session) -> std::shared_ptr<LeaderElection> {
  return std::make_shared<LeaderElection>(key_prefix, session);
}
