#ifndef RPC_ELECTION_HPP_
#define RPC_ELECTION_HPP_

// Protocols
#include <ringElection.grpc.pb.h>
#include <ringElection.pb.h>

// Common
#include <peer_status.hpp>

class RingNodeServiceImpl final : public ring::RingNode::Service {
public:
  RingNodeServiceImpl();

  grpc::Status ReceiveElection(grpc::ServerContext* ctx, const ring::ElectionMsg* req, ring::Ack* resp);

  grpc::Status ReceiveCoordinator(grpc::ServerContext* ctx, const ring::CoordinatorMsg* req, ring::Ack* resp);

  void startServer(const std::string& listen_addr);

private:

  Peer findSuccessor();

  void forwardElection(int origin, int current_best_id, int current_best_energy, std::string current_best_address);
  void sendCoordinator(int leader_id, std::string leader_address);
};

#endif