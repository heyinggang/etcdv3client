/* Author: lipixun
 * Created Time : 2018-05-20 17:48:25
 *
 * File Name: kv.h
 * Description:
 *
 *    The kv implementation
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_KV_H_
#define GITHUB_BRAINAAS_ETCDV3Client_KV_H_

#include <string>
#include <vector>

#include "grpcpp/grpcpp.h"

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"

#include "common.h"

namespace brainaas::etcdv3client {

// RangeOptions
class RangeOptions : public Options<etcdserverpb::RangeRequest> {
 public:
  auto WithRangeEnd(const std::string& end) -> RangeOptions& {
    setfuncs.push_back([end](etcdserverpb::RangeRequest& req) {
      req.set_range_end(end);
    });
    return *this;
  }

  auto WithLimit(int64_t limit) -> RangeOptions& {
    setfuncs.push_back([limit](etcdserverpb::RangeRequest& req) {
      req.set_limit(limit);
    });
    return *this;
  }

  auto WithRevision(int64_t revision) -> RangeOptions& {
    setfuncs.push_back([revision](etcdserverpb::RangeRequest& req) {
      req.set_revision(revision);
    });
    return *this;
  }

  auto WithSortOrder(bool ascend = true) -> RangeOptions& {
    setfuncs.push_back([ascend](etcdserverpb::RangeRequest& req) {
      if (ascend) {
        req.set_sort_order(etcdserverpb::RangeRequest_SortOrder_ASCEND);
      } else {
        req.set_sort_order(etcdserverpb::RangeRequest_SortOrder_DESCEND);
      }
    });
    return *this;
  }
  // Sort by key. This is the default one, so there's usually no need to call this
  auto WithSortByKey() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_sort_target(etcdserverpb::RangeRequest_SortTarget_KEY);
    });
    return *this;
  }

  auto WithSortByVersion() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_sort_target(etcdserverpb::RangeRequest_SortTarget_VERSION);
    });
    return *this;
  }

  auto WithSortByCreate() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_sort_target(etcdserverpb::RangeRequest_SortTarget_CREATE);
    });
    return *this;
  }

  auto WithSortByMod() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_sort_target(etcdserverpb::RangeRequest_SortTarget_MOD);
    });
    return *this;
  }

  auto WithSortByValue() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_sort_target(etcdserverpb::RangeRequest_SortTarget_VALUE);
    });
    return *this;
  }

  auto WithSerializable() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_serializable(true);
    });
    return *this;
  }

  auto WithKeysOnly() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_keys_only(true);
    });
    return *this;
  }

  auto WithCountOnly() -> RangeOptions& {
    setfuncs.push_back([](etcdserverpb::RangeRequest& req) {
      req.set_count_only(true);
    });
    return *this;
  }

  auto WithMinModRevision(int64_t revision) -> RangeOptions& {
    setfuncs.push_back([revision](etcdserverpb::RangeRequest& req) {
      req.set_min_mod_revision(revision);
    });
    return *this;
  }

  auto WithMaxModRevision(int64_t revision) -> RangeOptions& {
    setfuncs.push_back([revision](etcdserverpb::RangeRequest& req) {
      req.set_max_mod_revision(revision);
    });
    return *this;
  }

  auto WithMinCreateRevision(int64_t revision) -> RangeOptions& {
    setfuncs.push_back([revision](etcdserverpb::RangeRequest& req) {
      req.set_min_create_revision(revision);
    });
    return *this;
  }

  auto WithMaxCreateRevision(int64_t revision) -> RangeOptions& {
    setfuncs.push_back([revision](etcdserverpb::RangeRequest& req) {
      req.set_max_create_revision(revision);
    });
    return *this;
  }
};

// PutOptions
class PutOptions : public Options<etcdserverpb::PutRequest> {
 public:
  // Set the lease of this key
  auto WithLease(int64_t lease) -> PutOptions& {
    setfuncs.push_back([lease](etcdserverpb::PutRequest& req) {
      req.set_lease(lease);
    });
    return *this;
  }
  // Return the previous key-value data
  auto WithPrevKV() -> PutOptions& {
    setfuncs.push_back([](etcdserverpb::PutRequest& req) {
      req.set_prev_kv(true);
    });
    return *this;
  }
  // Ignore the value
  auto WithIgnoreValue() -> PutOptions& {
    setfuncs.push_back([](etcdserverpb::PutRequest& req) {
      req.set_ignore_value(true);
    });
    return *this;
  }
  // Ignore the lease
  auto WithIgnoreLease() -> PutOptions& {
    setfuncs.push_back([](etcdserverpb::PutRequest& req) {
      req.set_ignore_lease(true);
    });
    return *this;
  }
};

// DeleteRangeOptions
class DeleteRangeOptions : public Options<etcdserverpb::DeleteRangeRequest> {
 public:
  // With range end
  auto WithRangeEnd(const std::string& end) -> DeleteRangeOptions& {
    setfuncs.push_back([end](etcdserverpb::DeleteRangeRequest& req) {
      req.set_range_end(end);
    });
    return *this;
  }
  // With the previous key-value data
  auto WithPrevKV() -> DeleteRangeOptions& {
    setfuncs.push_back([](etcdserverpb::DeleteRangeRequest& req) {
      req.set_prev_kv(true);
    });
    return *this;
  }
};

// CompactOptions
class CompactOptions : public Options<etcdserverpb::CompactionRequest> {
 public:
  // With physical
  auto WithPhysical() -> CompactOptions& {
    setfuncs.push_back([](etcdserverpb::CompactionRequest& req) {
      req.set_physical(true);
    });
    return *this;
  }
};

class Txn;

// The kv interface
class KVInterface {
 public:
  virtual ~KVInterface() = default;

  // Get key and values
  auto virtual Range(grpc::ClientContext* context,
                     const std::string& key,
                     etcdserverpb::RangeResponse* response,
                     const RangeOptions& options = RangeOptions()) -> grpc::Status = 0;

  // Put key and value
  auto virtual Put(grpc::ClientContext* context,
                   const std::string& key,
                   const std::string& value,
                   etcdserverpb::PutResponse* response,
                   const PutOptions& options = PutOptions()) -> grpc::Status = 0;

  // Delete key(s)
  auto virtual DeleteRange(grpc::ClientContext* context,
                           const std::string& key,
                           etcdserverpb::DeleteRangeResponse* response,
                           const DeleteRangeOptions& options = DeleteRangeOptions()) -> grpc::Status = 0;

  // Txn
  auto virtual Txn(grpc::ClientContext* context,
                   const Txn& txn,
                   etcdserverpb::TxnResponse* response) -> grpc::Status = 0;

  // Compact
  auto virtual Compact(grpc::ClientContext* context,
                       int64_t revision,
                       etcdserverpb::CompactionResponse* response,
                       const CompactOptions& options = CompactOptions()) -> grpc::Status = 0;
};

class Txn {
 public:
  // If: The compare condition
  auto If(const etcdserverpb::Compare& compare) -> Txn& {
    request_.add_compare()->CopyFrom(compare);
    return *this;
  }

  // If: The compare condition
  auto If(const etcdserverpb::Compare&& compare) -> Txn& {
    *request_.add_compare() = std::move(compare);
    return *this;
  }

  // Then
  auto Then(const Txn& txn) -> Txn& {
    request_.add_success()->mutable_request_txn()->CopyFrom(txn.GetRequest());
    return *this;
  }

  auto ThenRange(const std::string& key, const RangeOptions& options = RangeOptions()) -> Txn& {
    auto req = request_.add_success()->mutable_request_range();
    req->set_key(key);
    options.Apply(*req);
    return *this;
  }

  auto ThenPut(const std::string& key,
               const std::string& value,
               const PutOptions& options = PutOptions()) -> Txn& {
    auto req = request_.add_success()->mutable_request_put();
    req->set_key(key);
    req->set_value(value);
    options.Apply(*req);
    return *this;
  }

  auto ThenDeleteRange(const std::string& key, const DeleteRangeOptions& options = DeleteRangeOptions()) -> Txn& {
    auto req = request_.add_success()->mutable_request_delete_range();
    req->set_key(key);
    options.Apply(*req);
    return *this;
  }

  // Else
  auto Else(const Txn& txn) -> Txn& {
    request_.add_failure()->mutable_request_txn()->CopyFrom(txn.GetRequest());
    return *this;
  }

  auto ElseRange(const std::string& key, const RangeOptions& options = RangeOptions()) -> Txn& {
    auto req = request_.add_failure()->mutable_request_range();
    req->set_key(key);
    options.Apply(*req);
    return *this;
  }

  auto ElsePut(const std::string& key,
               const std::string& value,
               const PutOptions& options = PutOptions()) -> Txn& {
    auto req = request_.add_failure()->mutable_request_put();
    req->set_key(key);
    req->set_value(value);
    options.Apply(*req);
    return *this;
  }

  auto ElseDeleteRange(const std::string& key, const DeleteRangeOptions& options = DeleteRangeOptions()) -> Txn& {
    auto req = request_.add_failure()->mutable_request_delete_range();
    req->set_key(key);
    options.Apply(*req);
    return *this;
  }

  // Get txn request
  auto GetRequest() const -> const etcdserverpb::TxnRequest& {
    return request_;
  }

 private:
  etcdserverpb::TxnRequest request_;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_KV_H_
