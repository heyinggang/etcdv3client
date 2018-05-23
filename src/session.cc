/* Author: lipixun
 * Created Time : 2018-05-20 17:44:12
 *
 * File Name: session.cc
 * Description:
 *
 *    The session implementation, this is a high-level encapsulation of lease
 *
 */

#include "session.h"

using namespace etcdv3client;

Session::Session(const std::shared_ptr<Client>& client, const SessionOptions& options = SessionOptions()) :
  client_(client),
  options_(options) {
  // Schedule to call lease grant
}

auto Session::Close() -> void {

}

auto Session::KeepAlive() -> void {
  // Lease grant
  // Lease keep alive
  // Lease revoke
}
