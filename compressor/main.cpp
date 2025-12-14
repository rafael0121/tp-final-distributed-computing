// STD
#include <iostream>

// GRPC
#include <grpcpp/server_builder.h>

// RPC Servies
#include "rpc_compressor.hpp"
#include "rpc_discover.hpp"
#include "rpc_election.hpp"

#include <lamport.hpp>

// Services
RingNodeServiceImpl ringNode;
CompressorNodeImpl compressNode;
DiscoveryServiceImpl discoverNodes;

void start_rpc_server(std::string ip, int port){
    grpc::ServerBuilder builder;
    builder.AddListeningPort(ip, grpc::InsecureServerCredentials());
    builder.RegisterService(&ringNode);
    builder.RegisterService(&compressNode);
    builder.RegisterService(&discoverNodes);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}

int main(){
    start_rpc_server("0.0.0.0", 50000);

    return 0;
}