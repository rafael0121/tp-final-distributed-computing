// STD
#include <iostream>
#include <thread>

// GRPC
#include <grpcpp/server_builder.h>

// RPC Servies
#include "rpc_compressor.hpp"
#include "rpc_discover.hpp"
#include "rpc_election.hpp"

#include <lamport.hpp>
#include <logger.hpp>

#include <peer_status.hpp>

// Services
RingNodeServiceImpl ringNode;
CompressorNodeImpl compressNode;
DiscoveryServiceImpl discoverNodes;

// Initialize node.
PeerStatus myStatus = PeerStatus::getInstance();

void start_rpc_server(std::string address){
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&ringNode);
    builder.RegisterService(&compressNode);
    builder.RegisterService(&discoverNodes);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}

// --knownnode
std::string known_node_address;

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
            known_node_address = argv[++i];
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

// Thread
void printStatus(){
    while(true){
        sleep(10);
        myStatus.printStatus();
    }
}

void mainBehavior(){
    // The first things to do.
    if(!known_node_address.empty()){
        if(!discoverNodes.syncNodes(known_node_address)){
            LOG_ERRO("Known node isn't alive");
            exit(-1);
        }
    } else {
        // Set me as the coordinator.
        myStatus.setCoordinator({myStatus.getId(), myStatus.getAddress()});
    }

    while(true){

        // Check if i am the coordinator.
        if(myStatus.getCoordinator().id == myStatus.getId()){
            LOG_INFO("The coordinator is me.");
            sleep(30);
        } else {
            // Sync with the coordinator.
            if(!discoverNodes.syncNodes(myStatus.getCoordinator().address)){
                // If sync failed is because coordinator is dead.
                myStatus.removeKnownNodes(myStatus.getCoordinator());
                ringNode.callElection();
                sleep(30); // Election timeout
            };
            sleep(5);// sync time.
        }
    }
}

int main(int argc, char *argv[]){
    if(!parse_args(argc, argv)){
        std::cerr << "Address not informed: use --adress <ip:port>";
        return -1;
    }

    std::string myAddr =  myStatus.getAddress();

    /*
        Initialize threads.
    */ 

    // Print status thread.
    std::thread t_prints(printStatus);
    t_prints.detach();

    // Main Node thread
    std::thread t_main(mainBehavior);
    t_main.detach();

    // Server thread.
    std::thread t_server(start_rpc_server, myAddr);
    t_server.join();

    return 0;
}