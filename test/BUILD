cc_test(
    name = "kv-test",
    srcs = ["kv-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//etcdv3client:client",
    ],
    linkstatic = True,
)

cc_test(
    name = "session-test",
    srcs = ["session-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//etcdv3client:client",
        "//etcdv3client:session",
    ],
    linkstatic = True,
)
