#ifndef RPC_DISCOVER_HPP_
#define RPC_DISCOVER_HPP_

// Protocols
#include <discoverNodes.grpc.pb.h>
#include <discoverNodes.pb.h>

#include <peer_status.hpp>

extern PeerStatus myStatus;

class DiscoveryServiceImpl final : public disc::DiscoveryService::Service {
public:
    DiscoveryServiceImpl();

    grpc::Status Hello(
        grpc::ServerContext* context,
        const disc::HelloRequest* request, 
        disc::HelloReply* reply
    );

    void syncNodes(std::string dest_address);
};

#endif