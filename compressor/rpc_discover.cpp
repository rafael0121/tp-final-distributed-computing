#include "rpc_discover.hpp"

// Protocols
#include <discoverNodes.grpc.pb.h>
#include <discoverNodes.pb.h>

// GRPC
#include <grpcpp/server_builder.h>

DiscoveryServiceImpl::DiscoveryServiceImpl(int id, int energy)
    : node_id(id), energy_level(energy) {}

grpc::Status DiscoveryServiceImpl::Hello(
    grpc::ServerContext* context, 
    const disc::HelloRequest* request,
    disc::HelloReply* reply
){
    reply->set_node_id(node_id);
    reply->set_energy(energy_level);
    return grpc::Status::OK;
}