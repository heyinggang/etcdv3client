cc_library(
    name = "common",
    srcs = ["common.cc"],
    hdrs = ["common.h"],
    deps = [
        "@com_github_cppcontext_context//context:context",
        "@com_github_grpc_grpc//:grpc++",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "status",
    hdrs = ["status.h"],
    deps = [
        "@com_github_cppcontext_maybecommon//maybecommon:status",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__"],
)

cc_library(
    name = "transport",
    srcs = ["transport.cc"],
    hdrs = ["transport.h"],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "//proto:proto",
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
        "election.h",
        "session.h",
    ],
    deps = [
        "@com_github_grpc_grpc//:grpc++",
        "//proto:proto",
        ":common",
        ":status",
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
    name = "election",
    srcs = ["election.cc"],
    hdrs = ["election.h"],
    deps = [
        "@com_github_google_glog//:glog",
        "@com_github_cppcontext_context//context:context",
        ":session",
        ":client_base",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__", "//:__pkg__"],
)

cc_library(
    name = "session",
    srcs = ["session.cc"],
    hdrs = ["session.h"],
    deps = [
        "@com_github_google_glog//:glog",
        "@com_github_brainaas_cbase//cbase/time:backoff",
        "@com_github_cppcontext_context//context:context",
        ":status",
        ":client_base",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__", "//:__pkg__"],
)

cc_library(
    name = "leader_session",
    srcs = ["leader_session.cc"],
    hdrs = ["leader_session.h"],
    deps = [
        "@com_github_google_glog//:glog",
        "@com_github_brainaas_cbase//cbase/time:backoff",
        "@com_github_cppcontext_context//context:context",
        ":status",
        ":client_base",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__", "//:__pkg__"],
)

cc_library(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        ":client_base",
        ":kv",
        ":watch",
        ":lease",
        ":election",
        "leader_session",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//test:__pkg__", "//:__pkg__"],
)
