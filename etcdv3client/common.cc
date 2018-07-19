/* Author: lipixun
 * Created Time : 2018-05-21 17:42:42
 *
 * File Name: common.cc
 * Description:
 *
 *    The common data type definition
 *
 */

#include "common.h"

auto brainaas::etcdv3client::GetPrefixRangeEnd(const std::string& key) -> std::string {
  for (size_t i = 0; i < key.length(); ++i) {
    if (key[key.length()-i-1] < char(0xff)) {
      auto range_end = key.substr(0, i+1);
      ++range_end[i];
      return range_end;
    }
  }

  return std::string("\0", 1);
}
