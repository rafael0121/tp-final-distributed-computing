#include "rpc_discover.hpp"
#include <peer_status.hpp>

// Protocols
#include <discoverNodes.grpc.pb.h>
#include <discoverNodes.pb.h>

extern PeerStatus myStatus;

DiscoveryServiceImpl::DiscoveryServiceImpl(){}

grpc::Status DiscoveryServiceImpl::Hello(
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