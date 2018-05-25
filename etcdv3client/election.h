/* Author: lipixun
 * Created Time : 2018-05-20 17:44:33
 *
 * File Name: election.h
 * Description:
 *
 *    The election implementation based on session
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_ELECTION_H_
#define GITHUB_BRAINAAS_ETCDV3Client_ELECTION_H_

#include <memory>
#include <string>
#include <chrono>
#include <functional>

#include "grpcpp/grpcpp.h"

#include "cbase/context/context.h"

#include "session.h"

namespace brainaas::etcdv3client {

// The node of election
class ElectionNode {
 public:
  ElectionNode(
    const std::string key,
    const std::string value,
    const int64_t lease,
    const int64_t version,
    const int64_t create_revision,
    const int64_t mod_revision,
    const int64_t current_revision)
      : key_(key),
        value_(value),
        lease_(lease),
        version_(version),
        create_revision_(create_revision),
        mod_revision_(mod_revision),
        current_revision_(current_revision) {}

  auto get_key() const -> const std::string { return key_; }
  auto get_value() const -> const std::string { return value_; }
  auto get_lease() const -> int64_t { return lease_; }
  auto get_version() const -> int64_t { return version_; }
  auto get_create_revision() const -> int64_t { return create_revision_; }
  auto get_mod_revision() const -> int64_t { return mod_revision_; }
  auto get_current_revision() const -> int64_t { return current_revision_; }

 private:
  std::string key_;
  std::string value_;
  int64_t lease_;
  int64_t version_;
  int64_t create_revision_;
  int64_t mod_revision_;
  int64_t current_revision_;
};

typedef std::function<void(const ElectionNode& prev_leader, const ElectionNode& current_leader)> LeaderChangeCallback;

// Check election info and watch election
class Election {
 public:
  Election(const std::string& key_prefix, std::shared_ptr<Client> client, LeaderChangeCallback leader_change_callback = nullptr)
      : key_prefix_(key_prefix),
        client_(client),
        leader_change_callback_(leader_change_callback) {}

  // Get current leader
  auto GetLeader(grpc::ClientContext* context, ElectionNode *node) const -> grpc::Status;
  // Watch the leader changed
  // Returns:
  //  bool: True if changed, otherwise timeout
  auto Watch(grpc::ClientContext* context, ElectionNode *node, std::chrono::seconds timeout = std::chrono::seconds(0)) -> bool;

  // Get the key prefix
  auto get_key_prefix() const -> const std::string& { return key_prefix_; }

 private:
  std::string key_prefix_;
  std::shared_ptr<Client> client_;
  LeaderChangeCallback leader_change_callback_;
};

// Elect
class LeaderElection : public Election {
 public:
  LeaderElection(const std::string& key_prefix, std::shared_ptr<Session> session)
    : Election(key_prefix, session->get_client()),
      session_(session) {}

  // Check if is leader or not
  auto IsLeader() const -> bool;
  // Campaign puts a value as eligible for the election
  // This is a blocking method which will return if:
  //  * Elected as a leader
  //  * Failed to join the election
  // Args:
  //  value: The value to claim
  //  leader_ctx: A context object that will be done if current node lose the leader identity.
  // Returns:
  //  grpc::Status: Will if ok if succeed, otherwise contains the rpc error
  auto Campaign(grpc::ClientContext* context,
                const std::string& value,
                std::shared_ptr<brainaas::cbase::context::Context>* leader_ctx) -> grpc::Status;
  // Proclaim lets the leader announce a new value without another election
  // The `value` this class holds will change after the value is successfully changed in etcd
  auto Proclaim(grpc::ClientContext* context, const std::string& value) -> grpc::Status;
  // Resign lets a leader start a new election
  auto Resign(grpc::ClientContext* context) -> grpc::Status;

  // Get election value
  auto get_value() const -> const std::string& { return value_; }

 private:
  std::shared_ptr<Session> session_;

  std::string value_;
};

// Create a new election
auto NewElection(const std::string& key_prefix, std::shared_ptr<Client> client) -> std::shared_ptr<Election>;
// Create a new leader election
auto NewLeaderElection(const std::string& key_prefix, const std::shared_ptr<Session> session) -> std::shared_ptr<LeaderElection>;

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_ELECTION_H_
