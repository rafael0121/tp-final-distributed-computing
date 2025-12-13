#ifndef RPC_ELECTION_HPP_
#define RPC_ELECTION_HPP_

// Protocols
#include <ringElection.grpc.pb.h>
#include <ringElection.pb.h>


class RingNodeServiceImpl final : public ring::RingNode::Service {
public:
  RingNodeServiceImpl(int id, const std::string& successor_addr);

  grpc::Status ReceiveElection(grpc::ServerContext* ctx, const ring::ElectionMsg* req, ring::Ack* resp);

  grpc::Status ReceiveCoordinator(grpc::ServerContext* ctx, const ring::CoordinatorMsg* req, ring::Ack* resp);

  void startServer(const std::string& listen_addr);

private:
  int my_id;
  int my_energy;
  std::string successor;
  std::atomic<int> coordinator_id;

  void forwardElection(int origin, int current_max_id, int current_max_energy);
  void sendCoordinator(int leader);
};

#endif