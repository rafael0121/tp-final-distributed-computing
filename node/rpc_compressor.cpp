#include "rpc_compressor.hpp"

// Protocols
#include <compressorNode.grpc.pb.h>
#include <compressorNode.pb.h>

grpc::Status CompressorNodeImpl::SendData(
    grpc::ServerContext* context,
    const compn::SensorBatch* request,
    compn::Ack* reply
) {
    for (const auto& r : request->readings()) {
      std::cout << "Valor=" << r.value() << " Timestamp=" << r.timestamp() << "\n";
    }
    reply->set_ok(true);
    return grpc::Status::OK;
};

grpc::Status CompressorNodeImpl::SendCompressData(
    grpc::ServerContext* context,
    const compn::SensorBatch* request,
    compn::Ack* reply
) {
    for (const auto& r : request->readings()) {
      std::cout << "Valor=" << r.value() << " Timestamp=" << r.timestamp() << "\n";
    }
    reply->set_ok(true);
    return grpc::Status::OK;
};