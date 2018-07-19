/* Author: lipixun
 * Created Time : 2018-05-21 17:42:29
 *
 * File Name: common.h
 * Description:
 *
 *    The common data type definition
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_COMMON_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_COMMON_H_

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "grpcpp/grpcpp.h"

namespace brainaas::etcdv3client {

const std::string LogPrefix = "brainaas::etcdv3client";

typedef std::function<std::unique_ptr<grpc::ClientContext>(void)> GetGRPCClientContextFunction;

auto inline GetGRPCClientContext(GetGRPCClientContextFunction f = nullptr) -> std::unique_ptr<grpc::ClientContext> {
  if (f != nullptr) {
    return f();
  }
  return std::make_unique<grpc::ClientContext>();
}

template<class TargetType>
class Options {
 public:
  typedef std::function<void(TargetType&)> SetFunction;

  auto virtual Apply(TargetType& target) const noexcept -> void {
    for (const auto& f : setfuncs) {
      f(target);
    }
  }

 protected:
  std::vector<SetFunction> setfuncs;
};

auto GetPrefixRangeEnd(const std::string& key) -> std::string;

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_COMMON_H_
