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
