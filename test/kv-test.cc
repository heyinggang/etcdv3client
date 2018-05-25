/* Author: lipixun
 * Created Time : 2018-05-21 18:56:22
 *
 * File Name: kv-test.cc
 * Description:
 *
 *    The key value function tests
 *
 */

#include <string>
#include <iostream>

#include "gtest/gtest.h"

#include "etcdv3client/client.h"

namespace brainaas::etcdv3client::test {

TEST(KVTest, Basic) {
  auto client = brainaas::etcdv3client::New("127.0.0.1:2379");

  std::string test_key("test_key_kvtest_basic");
  std::string test_value("test_value_kvtest_basic");

  // Put a simple key value
  {
    grpc::ClientContext ctx;
    proto::PutResponse response;
    auto status = client->Put(&ctx, test_key, test_value, &response);
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
  }

  // Get the value
  {
    grpc::ClientContext ctx;
    proto::RangeResponse response;
    auto status = client->Range(&ctx, test_key, &response);
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
    EXPECT_EQ(response.kvs().size(), 1) << "Wrong response kv size";
    EXPECT_EQ(response.kvs()[0].key(), test_key) << "Wrong response key";
    EXPECT_EQ(response.kvs()[0].value(), test_value) << "Wrong response value";
  }

  // Delete the value
  {
    grpc::ClientContext ctx;
    proto::DeleteRangeResponse response;
    auto status = client->DeleteRange(&ctx, test_key, &response);
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
    EXPECT_EQ(response.prev_kvs().size(), 0) << "Wrong response prev_kv size";
  }
}

TEST(KVTest, DeleteWithPrevKV) {
  auto client = brainaas::etcdv3client::New("127.0.0.1:2379");

  std::string test_key("test_key_kvtest_deletewithprevkv");
  std::string test_value("test_value_kvtest_deletewithprevkv");

  // Put a simple key value
  {
    grpc::ClientContext ctx;
    proto::PutResponse response;
    auto status = client->Put(&ctx, test_key, test_value, &response);
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
  }

  // Delete the value
  {
    grpc::ClientContext ctx;
    proto::DeleteRangeResponse response;
    auto status = client->DeleteRange(&ctx, test_key, &response, DeleteRangeOptions().WithPrevKV());
    EXPECT_TRUE(status.ok()) << "GRPC:" << status.error_message();
    EXPECT_EQ(response.prev_kvs().size(), 1) << "Wrong response prev_kv size";
    EXPECT_EQ(response.prev_kvs()[0].key(), test_key) << "Wrong response key";
    EXPECT_EQ(response.prev_kvs()[0].value(), test_value) << "Wrong response value";
  }
}

}
