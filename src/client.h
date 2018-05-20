/* Author: lipixun
 * Created Time : 2018-05-20 17:45:02
 *
 * File Name: client.h
 * Description:
 *
 *    The client
 *
 */

#ifndef GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_
#define GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_

namespace etcdv3client {

// The etcdv3 client class
class Client {
 public:

  // Client is neither copyable nor movable.
  Client(const Client&) = delete;
  Client(Client&&) = delete;
  Client& operator=(const Client&) = delete;
};

}

#endif // GITHUB_BRAINAAS_ETCDV3Client_CLIENT_H_
