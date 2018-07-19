/* Author: lipixun
 * Created Time : 2018-06-01 15:44:36
 *
 * File Name: main.cc
 * Description:
 *
 *    The test main
 *
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

int main(int argc, char** argv) {
  FLAGS_v = 3;
  FLAGS_logtostderr = true;
  testing::InitGoogleMock(&argc, argv);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  return RUN_ALL_TESTS();
}
