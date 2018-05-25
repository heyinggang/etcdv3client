# etcdv3client

The etcdv3 client in cpp

## Build

* Mac OS

  bazel test --copt "-D GRPC_BAZEL_BUILD" //test:* && bazel build --copt "-D GRPC_BAZEL_BUILD" libetcdv3client.so

* Linux

  bazel test //test:* && bazel build libetcdv3client.so

## Question

* Q: Why build or test on mac os x failed?

  A: There're many issues when building and testing on mac ox x, the walk around is to build & test with: `--copt "-D GRPC_BAZEL_BUILD"`

* Q: Why tests are all `linkstatic`?

  A: There's a bug when testing on mac with grpc if the library is not linkstatic. Please see: [https://github.com/bazelbuild/bazel/issues/507](https://github.com/bazelbuild/bazel/issues/507)
