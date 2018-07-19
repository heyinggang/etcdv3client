/* Author: lipixun
 * Created Time : 2018-05-20 17:44:33
 *
 * File Name: election.h
 * Description:
 *
 *    The election implementation based on session
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_ELECTION_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_ELECTION_H_

#include <tuple>
#include <mutex>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <functional>

#include "grpcpp/grpcpp.h"

#include "context/context.h"

#include "status.h"
#include "common.h"
#include "session.h"
#include "lease.h"

namespace brainaas::etcdv3client {

// The node of election
class ElectionNode {
 public:
  ElectionNode() {}
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

  auto IsValid() -> bool { return lease_ != InvalidLeaseID; }

  auto get_key() const -> const std::string { return key_; }
  auto get_value() const -> const std::string { return value_; }
  auto get_lease() const -> int64_t { return lease_; }
  auto get_version() const -> int64_t { return version_; }
  auto get_create_revision() const -> int64_t { return create_revision_; }
  auto get_mod_revision() const -> int64_t { return mod_revision_; }

  // NOTE: This is a request-level parameter.
  // The value of this field is the key-value store revision when the request was applied.
  auto get_current_revision() const -> int64_t { return current_revision_; }
  // Get the revision for watch leader change, this is actually the current revision
  auto get_watch_revision() const -> int64_t { return current_revision_; }

  //
  // Set methods
  //

  auto set_bykv(const proto::KeyValue& kv) -> void {
    set_key(kv.key());
    set_value(kv.value());
    set_lease(kv.lease());
    set_version(kv.version());
    set_create_revision(kv.create_revision());
    set_mod_revision(kv.mod_revision());
  }
  auto set_key(const std::string& key) -> void { key_ = key; }
  auto set_value(const std::string& value) -> void { value_ = value; }
  auto set_lease(int64_t lease) -> void { lease_ = lease; }
  auto set_version(int64_t version) -> void { version_ = version; }
  auto set_create_revision(int64_t revision) -> void { create_revision_ = revision; }
  auto set_mod_revision(int64_t revision) -> void { mod_revision_ = revision; }
  auto set_current_revision(int64_t revision) -> void { current_revision_ = revision; }

 private:
  std::string key_;
  std::string value_;
  int64_t lease_;
  int64_t version_;
  int64_t create_revision_;
  int64_t mod_revision_;
  int64_t current_revision_;
};

class ElectionWatchOptions {
 public:
  auto WithRevision(int64_t revision) -> ElectionWatchOptions& {
    revision_ = revision;
    return *this;
  }

  auto WithGetGRPCClientContextFunc(GetGRPCClientContextFunction f) -> ElectionWatchOptions& {
    get_grpc_client_context_func_ = f;
    return *this;
  }

  auto get_revision() const -> int64_t { return revision_; }
  auto get_get_grpc_client_context_function() const -> GetGRPCClientContextFunction { return get_grpc_client_context_func_; }

 private:
  int64_t revision_;
  GetGRPCClientContextFunction get_grpc_client_context_func_;
};

// CampaignOptions defines the options
class CampaignOptions {
 public:
  // WithDonotAutoStart set options to not auto start campaign
  auto WithDonotAutoStart() -> CampaignOptions& {
    donot_auto_start_ = true;
    return *this;
  }
  // WithClientContext set options to custom client context
  auto WithClientContext(GetGRPCClientContextFunction f) -> CampaignOptions& {
    get_grpc_client_context_func_ = f;
    return *this;
  }

  auto get_donot_auto_start() const -> bool { return donot_auto_start_; }
  auto get_get_grpc_client_context_func() const -> GetGRPCClientContextFunction { return get_grpc_client_context_func_; }

 private:
  bool donot_auto_start_;
  GetGRPCClientContextFunction get_grpc_client_context_func_;
};

class LeaderSession;

// The interface of election
class ElectionInterface {
 public:
  virtual ~ElectionInterface() = default;

  // GetLeader gets the leader node
  auto virtual GetLeader(grpc::ClientContext* context,
                         const std::string& key_prefix,
                         ElectionNode* out_leader_node) -> maybecommon::Status = 0;

  // Watch watchs the election change
  auto virtual Watch(const std::shared_ptr<context::Context>& ctx,
                     const std::string& key_prefix,
                     ElectionNode* out_leader_node) -> maybecommon::Status = 0;
  auto virtual Watch(const std::shared_ptr<context::Context>& ctx,
                     const std::string& key_prefix,
                     ElectionNode* out_leader_node,
                     const ElectionWatchOptions& options) -> maybecommon::Status = 0;

  // Campaign puts a value as eligible for the election
  // This is a blocking method which will wait for the leadership or context cancelled
  // Args:
  //  value: The value to claim
  // Returns:
  //  Tuple<shared_ptr<LeaderSession>, maybecommon::Status>
  auto virtual Campaign(const std::shared_ptr<context::Context>& ctx,
                        const std::string& key_prefix,
                        const std::string& value,
                        const std::shared_ptr<Session>& session) -> std::shared_ptr<LeaderSession> = 0;
  auto virtual Campaign(const std::shared_ptr<context::Context>& ctx,
                        const std::string& key_prefix,
                        const std::string& value,
                        const std::shared_ptr<Session>& session,
                        const CampaignOptions& options) -> std::shared_ptr<LeaderSession> = 0;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_ELECTION_H_
