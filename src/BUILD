cc_library(
    name = "common",
    srcs = ["common.cc"],
    hdrs = ["common.h"],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "transport",
    srcs = ["transport.cc"],
    hdrs = ["transport.h"],
    deps = [
        "//proto:rpc_grpc_cc_proto",
        "@com_github_grpc_grpc//:grpc++",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "client",
    srcs = ["client.cc"],
    hdrs = ["client.h", "kv.h"],
    deps = [
        ":common",
        ":transport",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "kv",
    srcs = ["kv.cc"],
    deps = [
        ":client",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)
