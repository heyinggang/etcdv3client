/* Author: lipixun
 * Created Time : 2018-05-20 17:45:10
 *
 * File Name: client.cc
 * Description:
 *
 *    The client
 *
 */

#include "client.h"

using namespace brainaas::etcdv3client;

Client::Client(const std::string& target):
  Client(std::unique_ptr<TransportInterface>(new Transport(target))) {}

Client::Client(std::unique_ptr<TransportInterface>& transport):
  Client(std::move(transport)) {}

Client::Client(std::unique_ptr<TransportInterface>&& transport):
  transport_(std::move(transport)) {
  // Create stubs
  kv_ = proto::KV::NewStub(transport_->GetChannel());
  watch_ = proto::Watch::NewStub(transport_->GetChannel());
  lease_ = proto::Lease::NewStub(transport_->GetChannel());
  cluster_ = proto::Cluster::NewStub(transport_->GetChannel());
  maintenance_ = proto::Maintenance::NewStub(transport_->GetChannel());
  auth_ = proto::Auth::NewStub(transport_->GetChannel());
}

auto brainaas::etcdv3client::New(const std::string& target) -> std::shared_ptr<Client> {
  return std::make_shared<Client>(target);
}

auto brainaas::etcdv3client::New(std::unique_ptr<TransportInterface>& transport) -> std::shared_ptr<Client> {
  return std::make_shared<Client>(transport);
}
