/* Author: lipixun
 * Created Time : 2018-05-25 15:47:34
 *
 * File Name: session-test.cc
 * Description:
 *
 *    The session test
 *
 */

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "etcdv3client/client.h"
#include "etcdv3client/session.h"

namespace brainaas::etcdv3client::test {

TEST(SessionTest, GrantAndRevoke) {
  auto client = brainaas::etcdv3client::New("127.0.0.1:2379");
  auto session = brainaas::etcdv3client::NewSession(client);

  {
    grpc::ClientContext ctx;
    auto status = session->Grant(&ctx);
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
  }

  {
    grpc::ClientContext ctx;
    proto::LeaseLeasesResponse response;
    auto status = client->LeaseLeases(&ctx, &response);
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
    EXPECT_TRUE(
      std::find_if(
        response.leases().begin(),
        response.leases().end(),
        [session](const proto::LeaseStatus& s){ return s.id() == session->get_lease(); }
        ) != response.leases().end()) << "Lease not found:" << session->get_lease();
  }
}

}
