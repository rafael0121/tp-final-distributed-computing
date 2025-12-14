#include "rpc_discover.hpp"
#include <peer_status.hpp>

// GRPC
#include <grpcpp/grpcpp.h>

// Protocols
#include <discoverNodes.grpc.pb.h>
#include <discoverNodes.pb.h>

extern PeerStatus myStatus;

DiscoveryServiceImpl::DiscoveryServiceImpl(){}

grpc::Status DiscoveryServiceImpl::hello(
    grpc::ServerContext* context, 
    const disc::HelloRequest* request,
    disc::HelloReply* reply
){
    std::list<Peer> nodes = myStatus.copyKnownNodes();
    std::list<Peer> sensors = myStatus.copyKnownSensors();

    reply->set_energy(myStatus.getEnergyLevel());
    reply->set_id(myStatus.getId());

    for (const auto& n : nodes) {
        disc::Peer* newNode = reply->add_nodes();
        newNode->set_peer_id(n.id);
        newNode->set_peer_ip(n.address);
    }

    for (const auto& s : sensors) {
        disc::Peer* newSensor = reply->add_sensors();
        newSensor->set_peer_id(s.id);
        newSensor->set_peer_ip(s.address);
    }

    return grpc::Status::OK;
}

void DiscoveryServiceImpl::syncNodes(std::string dest_address){
    int myId = myStatus.getId();
    std::list<Peer> nodes;
    std::list<Peer> sensors;

    // RPC.
    disc::HelloRequest request;
    grpc::ClientContext context;
    disc::HelloReply reply;


    // Create channel to the successor.
    auto channel = grpc::CreateChannel(dest_address, grpc::InsecureChannelCredentials());
    std::unique_ptr<disc::DiscoveryService::Stub> stub 
                    = disc::DiscoveryService::NewStub(channel);

    request.set_sender_id(myId);

    grpc::Status status = stub->Hello(&context, request, &reply);

    if(status.ok()){
        Peer dest_node = {reply.id(), dest_address};
        nodes.push_back(dest_node);

        for(const auto& s : reply.sensors()){
            Peer sensor = {s.peer_id(), s.peer_ip()};
            sensors.push_back(sensor);
        }

        for(const auto& n : reply.nodes()){
            Peer node = {n.peer_id(), n.peer_ip()};
            nodes.push_back(node);
        }

        myStatus.updateKnownNodes(nodes);
        myStatus.updateKnownSensors(nodes);
    }
}