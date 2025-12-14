#include "node_status.hpp"

#include<mutex>
#include <list>
#include <algorithm>
#include <string>

std::mutex builder;
std::mutex list;

NodeStatus& NodeStatus::getInstance(int id, int energy){
    std::lock_guard<std::mutex> guard(builder);

    static NodeStatus nodeStatus = NodeStatus(id, energy);

    return nodeStatus;
 }

 void NodeStatus::addKnownNodes(Peer node){
    std::lock_guard<std::mutex> guard(list);
    knownNodes.push_back(node);
 };

 void NodeStatus::removeKnownNodes(Peer node){
    std::lock_guard<std::mutex> guard(list);
    int node_id = node.id;

    auto deleteNode = std::find_if(knownNodes.begin(), knownNodes.end(),
                                [node_id](const Peer& n) {
                                    return n.id == node_id;
                                });
    if(deleteNode != knownNodes.end()){
        knownNodes.erase(deleteNode);
    }
 };

 void NodeStatus::addKnownSensors(Peer sensor){
    std::lock_guard<std::mutex> guard(list);
    knownSensors.push_back(sensor);
 };

 void NodeStatus::RemoveKnownSensors(Peer sensor){
    std::lock_guard<std::mutex> guard(list);
    int sensor_id = sensor.id;

    auto deleteSensor = std::find_if(knownNodes.begin(), knownNodes.end(),
                                [sensor_id](const Peer& s) {
                                    return s.id == sensor_id;
                                });
    if(deleteSensor != knownNodes.end()){
        knownNodes.erase(deleteSensor);
    }
 };