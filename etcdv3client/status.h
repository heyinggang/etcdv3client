/* Author: lipixun
 * Created Time : 2018-05-28 11:11:39
 *
 * File Name: status.h
 * Description:
 *
 *    The status definition
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3CLIENT_STATUS_H_
#define GITHUB_BRAINAAS_ETCDV3CLIENT_STATUS_H_

#include <string>

#include "grpcpp/grpcpp.h"

#include "maybecommon/status.h"

namespace brainaas::etcdv3client {

// StatusScope defines the scope of all status that is returned by this module
const std::string StatusScopeName = "brainaas::etcdv3client";

// The status code
enum StatusCode : int {
  OK = 0,
  // GRPC error
  GRPCError = 1000,
  // Session error
  SessionAlreadyGranted = 1000,
  SessionAlreadyClosed = 1001,
  // Election error
  ElectionAlreadyCampaigned = 2000,
  ElectionCampaignOnClosedSession = 2001,
};

auto inline Status() -> maybecommon::Status {
  return maybecommon::Status().WithScope(::brainaas::etcdv3client::StatusScopeName);
}

auto inline Status(int code) -> maybecommon::Status {
  return maybecommon::Status(code).WithScope(::brainaas::etcdv3client::StatusScopeName);
}

auto inline Status(int code, const std::string& message) -> maybecommon::Status {
  return maybecommon::Status(code, message).WithScope(::brainaas::etcdv3client::StatusScopeName);
}

}

#endif // GITHUB_BRAINAAS_ETCDV3CLIENT_STATUS_H_
