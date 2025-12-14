// STD
#include <iostream>

// GRPC
#include <grpcpp/server_builder.h>

// RPC Servies
#include "rpc_compressor.hpp"
#include "rpc_discover.hpp"
#include "rpc_election.hpp"

#include <lamport.hpp>

#include <peer_status.hpp>

// Services
RingNodeServiceImpl ringNode;
CompressorNodeImpl compressNode;
DiscoveryServiceImpl discoverNodes;

// Initialize node.
PeerStatus myStatus = PeerStatus::getInstance();

void start_rpc_server(std::string address, int port){
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&ringNode);
    builder.RegisterService(&compressNode);
    builder.RegisterService(&discoverNodes);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}

// --knownnode
std::string known_node;

int parse_args(int argc, char *argv[]){
    bool check_address_arg = false;

    // --address
    std::string address;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--address" && i + 1 < argc) {
            address = argv[++i];
            myStatus.setAddress(address);
            check_address_arg = true;
        } else 
        if (arg == "--knownode" && i + 1 < argc) {
            known_node = argv[++i];
        }
    }

    return check_address_arg;
}

// Extract IP part
std::string getIp(const std::string& addr) {
    auto pos = addr.find_last_of(':');
    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid address format, expected ip:port");
    }
    return addr.substr(0, pos);
}

// Extract Port part
int getPort(const std::string& addr) {
    auto pos = addr.find_last_of(':');
    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid address format, expected ip:port");
    }
    return std::stoi(addr.substr(pos + 1));
}

int main(int argc, char *argv[]){
    if(!parse_args(argc, argv)){
        std::cerr << "Address not informed: use --adress <ip:port>";
    }

    start_rpc_server(getIp(myStatus.getAddress()), getPort(myStatus.getAddress()));

    if(!known_node.empty()) {
        
    }


    return 0;
}