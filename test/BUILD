cc_library(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
        "@com_google_googletest//:gtest",
        "//etcdv3client:client",
    ],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
)

cc_test(
    name = "kv-test",
    srcs = ["kv-test.cc"],
    deps = [
        "//etcdv3client:client",
        ":main",
    ],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    linkstatic = True,
)

cc_test(
    name = "session-test",
    srcs = ["session-test.cc"],
    deps = [
        "//etcdv3client:client",
        "//etcdv3client:session",
        ":main",
    ],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    linkstatic = True,
)
