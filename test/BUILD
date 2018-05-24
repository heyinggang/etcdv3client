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
