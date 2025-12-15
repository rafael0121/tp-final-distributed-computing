#include "rpc_discover.hpp"
#include <peer_status.hpp>
#include <logger.hpp>
#include <utils.hpp>

// GRPC
#include <grpcpp/grpcpp.h>

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
    LOG_DBUG("Hello called...");

    int sender_clock = request->lamport_timestamp();
    // Update my timeStamp
    myStatus.logic_clock.updateTimestamp(sender_clock);

    // Sender informations
    int sender_id = request->sender_id();
    std::string sender_address = request->sender_address();

    int myId = myStatus.getId();

    // Add the sender to known nodes
    Peer sender_node = {sender_id, sender_address};
    myStatus.addKnownNode(sender_node);

    LOG_DBUG("Requested sync by ", sender_address);

    // Nodes and sensors to send.
    std::list<Peer> nodes = myStatus.copyKnownNodes();
    std::list<Peer> sensors = myStatus.copyKnownSensors();

    // Leader node.
    Peer leader = myStatus.getCoordinator();


    reply->set_node_id(myId);
    reply->set_leader_address(leader.address);
    reply->set_leader_id(leader.id);
    reply->set_lamport_timestamp(myStatus.logic_clock.curTimestamp());

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

    LOG_DBUG("Replying sync...");
    return grpc::Status::OK;
}

bool DiscoveryServiceImpl::syncNodes(std::string dest_address){
    LOG_DBUG("Start syncNodes");

    int myId = myStatus.getId();
    std::string myAddress = myStatus.getAddress();

    // Leader node.
    Peer leader_node;

    // Nodes and sensors synced.
    std::list<Peer> nodes;
    std::list<Peer> sensors;

    // RPC.
    disc::HelloRequest request;
    grpc::ClientContext context;
    disc::HelloReply reply;

    request.set_sender_id(myId);
    request.set_sender_address(myAddress);

    // Create channel to the successor.
    LOG_DBUG("Sync with ", dest_address, "...");
    auto channel = grpc::CreateChannel(dest_address, grpc::InsecureChannelCredentials());
    std::unique_ptr<disc::DiscoveryService::Stub> stub 
                    = disc::DiscoveryService::NewStub(channel);

    LOG_DBUG("Sending Hello...");
    grpc::Status status = stub->Hello(&context, request, &reply);

    if(status.ok()){
        LOG_DBUG("Hello reply: OK");
        Peer dest_node = {reply.node_id(), dest_address};
        nodes.push_back(dest_node);

        for(const auto& s : reply.sensors()){
            Peer sensor = {s.peer_id(), s.peer_ip()};
            sensors.push_back(sensor);
        }

        for(const auto& n : reply.nodes()){
            Peer node = {n.peer_id(), n.peer_ip()};
            nodes.push_back(node);
        }
        
        leader_node.id = reply.leader_id();
        leader_node.address = reply.leader_address();

        myStatus.setCoordinator(leader_node);

        myStatus.updateKnownNodes(nodes);
        myStatus.updateKnownSensors(sensors);

        myStatus.logic_clock.updateTimestamp(reply.lamport_timestamp());

        LOG_DBUG("Nodes synced");
        return true;
    } else {
        LOG_ERRO("Hello reply: ", status.error_message());

        return false;
    }
}