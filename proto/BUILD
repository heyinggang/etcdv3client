load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "etcdserver_proto",
    srcs = ["etcdserver.proto"],
)

proto_library(
    name = "_auth_proto_only",
    srcs = ["auth.proto"],
)

proto_library(
    name = "_kv_proto_only",
    srcs = ["kv.proto"],
)

cc_proto_library(
    name = "etcdserver_cc_proto",
    deps = [":etcdserver_proto"],
)

cc_proto_library(
    name = "auth_proto",
    deps = [":_auth_proto_only"],
)

cc_proto_library(
    name = "kv_proto",
    deps = [":_kv_proto_only"],
)

cc_grpc_library(
    name = "rpc_grpc_cc_proto",
    srcs = ["rpc.proto"],
    deps = ["auth_proto", "kv_proto"],
    proto_only = False,
    well_known_protos = False,
    use_external = True,
    generate_mocks = False,
    visibility = ["//etcdv3client:__pkg__"],
)
