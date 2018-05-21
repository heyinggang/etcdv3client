/* Author: lipixun
 * Created Time : 2018-05-21 17:42:29
 *
 * File Name: common.h
 * Description:
 *
 *    The common data type definition
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_COMMON_H_
#define GITHUB_BRAINAAS_ETCDV3Client_COMMON_H_

#include <vector>
#include <functional>

namespace etcdv3client {

template<class TargetType>
class Options {
 public:
  typedef std::function<void(TargetType&)> SetFunctionType;

  auto virtual Apply(TargetType& target) const noexcept -> void {
    for (auto const& f : setfuncs) {
      f(target);
    }
  }

 protected:
  std::vector<SetFunctionType> setfuncs;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_COMMON_H_
