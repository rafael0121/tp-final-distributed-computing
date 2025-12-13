#include "rpc_compressor.hpp"

// Protocols
#include <ringElection/ringElection.grpc.pb.h>
#include <ringElection/ringElection.pb.h>

// GRPC
#include <grpcpp/server_builder.h>

// STD
#include <atomic>
#include <algorithm>
#include <thread>

// Builder.
RingNodeServiceImpl::RingNodeServiceImpl(int id, const std::string& successor_addr)
: my_energy(1000), my_id(id), successor(successor_addr), coordinator_id(-1) {}

/*
#################################################################
# Public
#################################################################
*/

grpc::Status RingNodeServiceImpl::ReceiveElection(grpc::ServerContext* ctx, const ring::ElectionMsg* req, ring::Ack* resp) {
    int max_id = std::max(req->best_id(), my_id);
    int best_energy = std::max(req->best_energy(), my_energy);
    if (req->origin_id() == my_id) {
        // election completed: announce coordinator
        sendCoordinator(max_id);
    } else {
        // forward to successor
        forwardElection(req->origin_id(), max_id, best_energy);
    }
    resp->set_ok(true);
        return grpc::Status::OK;
}

grpc::Status RingNodeServiceImpl::ReceiveCoordinator(grpc::ServerContext* ctx, const ring::CoordinatorMsg* req, ring::Ack* resp) {
    coordinator_id = req->leader_id();
    resp->set_ok(true);
    return grpc::Status::OK;
}

void RingNodeServiceImpl::startServer(const std::string& listen_addr) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(listen_addr, grpc::InsecureServerCredentials());
    builder.RegisterService(this);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}

/*
#################################################################
# Private
#################################################################
*/

void RingNodeServiceImpl::forwardElection(int origin, int current_max_id, int current_max_energy) {
    // create stub to successor and call ReceiveElection (handle retries/timeouts)
}
void RingNodeServiceImpl::sendCoordinator(int leader) {
    // call ReceiveCoordinator on successor until full ring notified
}