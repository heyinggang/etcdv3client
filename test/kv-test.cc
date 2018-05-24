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

TEST(KVTest, Put) {
  etcdv3client::Client client("123");
  //client.Put(nullptr, "123", "1234", nullptr, etcdv3client::PutOptions().WithLease(123).WithPrevKV());
}
