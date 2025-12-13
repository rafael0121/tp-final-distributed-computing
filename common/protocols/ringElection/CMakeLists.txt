find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

#
# Protobuf/Grpc source files
#
set(PROTO_FILES
    ringElection.proto
)

#
# Add Library target with protobuf sources
#
add_library(ringElection ${PROTO_FILES})

target_link_libraries(ringElection
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

target_include_directories(ringElection PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

#
# Compile protobuf and grpc files in ringElection target to cpp
#
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET ringElection LANGUAGE cpp)
protobuf_generate(TARGET ringElection LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")