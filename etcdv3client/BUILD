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
        "@com_github_grpc_grpc//:grpc++",
        "//proto:rpc_grpc_cc_proto",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "client_base",
    hdrs = [
        "client.h",
        "kv.h",
        "watch.h",
        "lease.h",
    ],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "//proto:rpc_grpc_cc_proto",
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
        ":client_base",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "watch",
    srcs = ["watch.cc"],
    deps = [
        ":client_base",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "lease",
    srcs = ["lease.cc"],
    deps = [
        ":client_base",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        ":client_base",
        ":kv",
        ":watch",
        ":lease",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__", "//:__pkg__"],
)

cc_library(
    name = "session",
    srcs = ["session.cc"],
    hdrs = ["session.h"],
    deps = [
        "@com_github_brainaas_cbase//cbase/time:backoff",
        ":client",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__", "//:__pkg__"],
)