workspace(name = "com_github_brainaas_etcdv3client")

http_archive(
    name = "com_github_google_glog",
    urls = ["https://github.com/google/glog/archive/master.zip"],
    strip_prefix = "glog-master",
)
http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "6e16c8bc91b1310a44f3965e616383dbda48f83e8c1eaa2370a215057b00cabe",
    strip_prefix = "gflags-77592648e3f3be87d6c7123eb81cbad75f9aef5a",
    urls = [
        "https://mirror.bazel.build/github.com/gflags/gflags/archive/77592648e3f3be87d6c7123eb81cbad75f9aef5a.tar.gz",
        "https://github.com/gflags/gflags/archive/77592648e3f3be87d6c7123eb81cbad75f9aef5a.tar.gz",
    ],
)

http_archive(
    name = "com_github_grpc_grpc",
    urls = ["https://github.com/grpc/grpc/archive/master.zip"],
    strip_prefix = "grpc-master",
)

load("@com_github_grpc_grpc//:bazel/grpc_deps.bzl", "grpc_deps")
grpc_deps()

http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/master.zip"],
    strip_prefix = "googletest-master",
)

local_repository(
    name = "com_github_brainaas_cbase",
    path = "/Users/lipixun/workspace/src/github.com/brainaas/cbase",
)

local_repository(
    name = "com_github_cppcontext_context",
    path = "/Users/lipixun/workspace/src/github.com/cppcontext/context",
)

local_repository(
    name = "com_github_cppcontext_maybecommon",
    path = "/Users/lipixun/workspace/src/github.com/cppcontext/maybecommon",
)
