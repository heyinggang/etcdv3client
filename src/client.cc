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

using namespace etcdv3client;

Client::Client(const std::string& target):
  Client(std::unique_ptr<TransportInterface>(new Transport(target))) {}

Client::Client(std::unique_ptr<TransportInterface>& transport):
  Client(std::move(transport)) {}

Client::Client(std::unique_ptr<TransportInterface>&& transport):
  transport_(std::move(transport)) {
  // Create stubs
  kv_ = etcdserverpb::KV::NewStub(transport_->GetChannel());
  watch_ = etcdserverpb::Watch::NewStub(transport_->GetChannel());
  lease_ = etcdserverpb::Lease::NewStub(transport_->GetChannel());
  cluster_ = etcdserverpb::Cluster::NewStub(transport_->GetChannel());
  maintenance_ = etcdserverpb::Maintenance::NewStub(transport_->GetChannel());
  auth_ = etcdserverpb::Auth::NewStub(transport_->GetChannel());
}

auto New(const std::string& target) -> std::shared_ptr<Client> {
  return std::make_shared<Client>(target);
}

auto New(std::unique_ptr<TransportInterface>& transport) -> std::shared_ptr<Client> {
  return std::make_shared<Client>(transport);
}
