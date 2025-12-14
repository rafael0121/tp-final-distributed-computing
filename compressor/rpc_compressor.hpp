#ifndef RPC_COMPRESSOR_HPP_
#define RPC_COMPRESSOR_HPP_

// Protocols
#include <compressorNode.grpc.pb.h>
#include <compressorNode.pb.h>

class CompressorNodeImpl final : public compn::CompressorNode::Service {
public:
    grpc::Status SendData(
        grpc::ServerContext* context,
        const compn::SensorBatch* request,
        compn::Ack* reply);

    grpc::Status SendCompressData(
        grpc::ServerContext* context,
        const compn::SensorBatch* request,
        compn::Ack* reply);
};

#endif