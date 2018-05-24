cc_library(
    name = "main",
    deps = [
        "//etcdv3client:client",
        "//etcdv3client:session",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "libetcdv3client.so",
    deps = [":main"],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
    linkshared = True,
)
