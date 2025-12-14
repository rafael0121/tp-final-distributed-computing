#include "rpc_election.hpp"
#include <peer_status.hpp>

// Protocols
#include <ringElection.grpc.pb.h>
#include <ringElection.pb.h>

// GRPC
#include <grpcpp/grpcpp.h>

// STD
#include <atomic>
#include <algorithm>
#include <thread>

// Builder.
RingNodeServiceImpl::RingNodeServiceImpl(){};

extern PeerStatus myStatus;

std::string getIpPort(std::string peer){
    std::string s = peer;
    auto pos = s.find(':');
    if (pos != std::string::npos) {
        s = s.substr(pos + 1);
    }
    return s;
}

/*
#################################################################
# Public
#################################################################
*/

grpc::Status RingNodeServiceImpl::ReceiveElection(grpc::ServerContext* ctx, const ring::ElectionMsg* req, ring::Ack* resp) {
    int best_energy = -1;
    int best_id = -1;
    std::string best_address;

    if(req->best_energy() > myStatus.getEnergyLevel()){
        // Req energy is bigger than my.
        best_energy = req->best_energy();
        best_id = req->best_id();
        best_address = req->best_address();
    } else if (best_energy == myStatus.getEnergyLevel()){
        // Req energy is equal my.
        if(req->best_id() > myStatus.getId()){
            // Req id is bigger than my.
            best_energy = req->best_energy();
            best_id = req->best_id();
            best_address = req->best_address();
        } else {
            // Req id is smaller than my.
            best_energy = myStatus.getEnergyLevel();
            best_id = myStatus.getId();
            best_address = myStatus.getAddress();
        }
    } else {
        // Req energy is smaller than my.
        best_energy = myStatus.getEnergyLevel();
        best_id = myStatus.getId();
        best_address = myStatus.getAddress();
    }

    if (req->origin_id() == myStatus.getId()) {
        // Election Completed announce coordinator
        sendCoordinator(best_id, best_address);
    } else {
        // Forward to successor
        forwardElection(req->origin_id(), best_id, best_energy, best_address);
    }
    resp->set_ok(true);
        return grpc::Status::OK;
}

grpc::Status RingNodeServiceImpl::ReceiveCoordinator(grpc::ServerContext* ctx, const ring::CoordinatorMsg* req, ring::Ack* resp) {
    myStatus.setCoordinator({req->leader_id(), req->leader_address()});
    return grpc::Status::OK;
}

/*
#################################################################
# Private
#################################################################
*/

Peer RingNodeServiceImpl::findSuccessor(){
    /* Find Sucessor */
    Peer successor;
    int myId = myStatus.getId();

    // Sort nodes list by id.
    std::list<Peer> nodes = myStatus.copyKnownNodes();
    nodes.sort([](const Peer& a, const Peer& b){
                    return a.id < b.id;
                });

    // Find the biggest id than my.
    for(const auto& node : nodes){
        if(node.id > myId){
            successor = node;
            return successor;
        }
    }

    // If haven't find the biggest return the first one.
    successor = nodes.front();
    return successor;
}

void RingNodeServiceImpl::forwardElection(int origin, int current_best_id, int current_best_energy, std::string current_best_address) {
    int myId = myStatus.getId();
    std::string myAddress = myStatus.getAddress();
    Peer successor;

    while(true){
        // If there isn't other live node i am the coordinator.
        if(myStatus.copyKnownNodes().empty()){
            successor = {myId, myAddress};
            myStatus.setCoordinator(successor);
            return;
        }

        successor = findSuccessor();

        // Create channel to the successor.
        auto channel = grpc::CreateChannel(successor.address, grpc::InsecureChannelCredentials());

        std::unique_ptr<ring::RingNode::Stub> stub = ring::RingNode::NewStub(channel);

        ring::ElectionMsg msg;
        grpc::ClientContext context;
        ring::Ack reply;

        msg.set_origin_id(origin);
        msg.set_best_energy(current_best_energy);
        msg.set_best_energy(current_best_id);
        msg.set_best_address(current_best_address);

        grpc::Status status = stub->ReceiveElection(&context, msg, &reply);

        if (status.ok() && reply.ok()) {
            return;
        } else {
            std::cerr << "Failed to forward election to successor "
                    << successor.id << ": "
                    << status.error_message() << std::endl;

            // If origin is dead i am the new "origin".
            if(successor.id == origin) {
                sendCoordinator(current_best_id, current_best_address);
                myStatus.setCoordinator({current_best_id, current_best_address});
            }

            // Remove failed node.
            myStatus.removeKnownNodes(successor);
        }
        // Continue looking for a live successor.
    }
}
void RingNodeServiceImpl::sendCoordinator(int leader_id, std::string leader_address) {
    // call ReceiveCoordinator on successor until full ring notified

    std::list<Peer> nodes = myStatus.copyKnownNodes();

    for(const Peer& node : nodes){
        // Create channel to notifie the node.
        auto channel = grpc::CreateChannel(node.address, grpc::InsecureChannelCredentials());

        std::unique_ptr<ring::RingNode::Stub> stub = ring::RingNode::NewStub(channel);

        ring::CoordinatorMsg msg;
        msg.set_leader_id(leader_id);
        msg.set_leader_address(leader_address);

        grpc::ClientContext context;
        ring::Ack reply;

        grpc::Status status = stub->ReceiveCoordinator(&context, msg, &reply);

        if (status.ok() && reply.ok()) {
            return;
        } else {
            std::cerr << "Failed to notifie node "
                    << node.id << ": "
                    << status.error_message() << std::endl;

            // Remove failed node.
            myStatus.removeKnownNodes(node);
        }
        // Continue looking for a live node to notify.
    }
}