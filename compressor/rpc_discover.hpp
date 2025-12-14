#ifndef RPC_DISCOVER_HPP_
#define RPC_DISCOVER_HPP_

// Protocols
#include <discoverNodes.grpc.pb.h>
#include <discoverNodes.pb.h>

class DiscoveryServiceImpl final : public disc::DiscoveryService::Service {
public:
    DiscoveryServiceImpl(int id, int energy);

    grpc::Status Hello(
        grpc::ServerContext* context,
        const disc::HelloRequest* request, 
        disc::HelloReply* reply
    );

private:
    
};

#endif