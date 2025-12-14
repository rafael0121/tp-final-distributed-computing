#include "peer_status.hpp"

#include<mutex>
#include <list>
#include <algorithm>
#include <string>
#include <random>

std::mutex builder;
std::mutex list;

/* Private builder */

PeerStatus::PeerStatus(){
    // Random engine (seeded with system clock)
    std::random_device rd;
    std::mt19937 gen(rd());

    // Distribution: from 1 up to INT32_MAX
    std::uniform_int_distribution<int> dist(1, INT32_MAX);

    // Generate peer id;
    id = dist(gen);

    address = "";
}

/* Set */

void PeerStatus::setAddress(std::string new_address){
    address = new_address;
}

/* Gets */

PeerStatus& PeerStatus::getInstance(){
    std::lock_guard<std::mutex> guard(builder);

    static PeerStatus nodeStatus = PeerStatus();

    return nodeStatus;
}

int PeerStatus::getEnergyLevel(){
    return energy_level;
}

int PeerStatus::getId(){
    return id;
}

std::string PeerStatus::getAddress(){
    return address;
}

/*
    Coordinator
*/

Peer PeerStatus::getCoordinator(){
    return coordinator;
}

void PeerStatus::setCoordinator(Peer newCoordinator){
    coordinator = newCoordinator;
}

/*
    KnownNodes
*/

void PeerStatus::addKnownNodes(Peer node){
    std::lock_guard<std::mutex> guard(list);
    knownNodes.push_back(node);
}

void PeerStatus::removeKnownNodes(Peer node){
    std::lock_guard<std::mutex> guard(list);
    int node_id = node.id;

    auto deleteNode = std::find_if(knownNodes.begin(), knownNodes.end(),
                                [node_id](const Peer& n) {
                                    return n.id == node_id;
                                });
    if(deleteNode != knownNodes.end()){
        knownNodes.erase(deleteNode);
    }
}

void PeerStatus::updateKnownNodes(const std::list<Peer>& nodes){
    std::lock_guard<std::mutex> guard(list);

    for(const auto& node : nodes) {
        // Search for existing node with same id.
        auto it = std::find_if(knownNodes.begin(), knownNodes.end(),
                                [node](const Peer& n){
                                    return n.id == node.id;
                                });

        // New node.
        if(it == knownNodes.end()) {
            knownNodes.push_back(node);
        }
    }
}

std::list<Peer> PeerStatus::copyKnownNodes(){
    return knownNodes;
}

/*
    KnownSensors
*/

void PeerStatus::addKnownSensors(Peer sensor){
    std::lock_guard<std::mutex> guard(list);
    knownSensors.push_back(sensor);
}

void PeerStatus::removeKnownSensors(Peer sensor){
    std::lock_guard<std::mutex> guard(list);
    int sensor_id = sensor.id;

    auto deleteSensor = std::find_if(knownNodes.begin(), knownNodes.end(),
                                [sensor_id](const Peer& s) {
                                    return s.id == sensor_id;
                                });
    if(deleteSensor != knownNodes.end()){
        knownNodes.erase(deleteSensor);
    }
}

void PeerStatus::updateKnownSensors(const std::list<Peer>& sensors){
    std::lock_guard<std::mutex> guard(list);

    for(const auto& sensor : sensors) {
        // Search for existing sensor with same id.
        auto it = std::find_if(knownNodes.begin(), knownNodes.end(),
                                [sensor](const Peer& s){
                                    return s.id == sensor.id;
                                });
        // New node.
        if(it == knownNodes.end()) {
            knownNodes.push_back(sensor);
        }
    }
}

std::list<Peer> PeerStatus::copyKnownSensors(){
    return knownSensors;
}