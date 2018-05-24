workspace(name = "com_github_brainaas_etcdv3client")

http_archive(
    name = "com_google_googletest",
    urls = ["https://github.com/google/googletest/archive/master.zip"],
    strip_prefix = "googletest-master",
)

http_archive(
    name = "com_github_grpc_grpc",
    urls = ["https://github.com/grpc/grpc/archive/master.zip"],
    strip_prefix = "grpc-master",
)

load("@com_github_grpc_grpc//:bazel/grpc_deps.bzl", "grpc_deps")
grpc_deps()

local_repository(
    name = "com_github_brainaas_cbase",
    path = "/Users/lipixun/workspace/src/github.com/brainaas/cbase",
)
