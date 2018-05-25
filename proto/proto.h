/* Author: lipixun
 * Created Time : 2018-05-25 18:22:38
 *
 * File Name: proto.h
 * Description:
 *
 *    Include all proto & grpc headers and rename namespaces
 *
 */

#include "proto/rpc.pb.h"
#include "proto/rpc.grpc.pb.h"
#include "proto/kv.pb.h"
#include "proto/auth.pb.h"

#ifndef GITHUB_BRAINAAS_ETCDV3Client_PROTO_PROTO_H_
#define GITHUB_BRAINAAS_ETCDV3Client_PROTO_PROTO_H_

namespace brainaas::etcdv3client {
  namespace proto = etcdserverpb;
}

#endif // GITHUB_BRAINAAS_ETCDV3Client_PROTO_PROTO_H_
