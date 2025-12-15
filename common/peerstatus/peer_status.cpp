#include "peer_status.hpp"

#include<mutex>
#include <list>
#include <algorithm>
#include <string>
#include <random>
#include <iostream>

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

void PeerStatus::addKnownNode(Peer node){
    std::list<Peer> list_of_one; 

    list_of_one.push_back(node);
    this->updateKnownNodes(list_of_one);
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
                                [node, this](const Peer& n){
                                    return (n.id == node.id || node.id == this->id);
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

void PeerStatus::addKnownSensor(Peer sensor){
    std::list<Peer> list_of_one; 

    list_of_one.push_back(sensor);
    this->updateKnownSensors(list_of_one);
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
                                [sensor, this](const Peer& s){
                                    return (s.id == sensor.id || s.id == this->id);
                                });
        if(it == knownNodes.end()) {
            // Not found the new node in the known nodes list.
            knownNodes.push_back(sensor);
        }
    }
}

std::list<Peer> PeerStatus::copyKnownSensors(){
    return knownSensors;
}

/*
    LOGs
*/

void PeerStatus::printStatus(){
    std::lock_guard<std::mutex> guard(list);
    std::cout 
    << std::endl
    << "===================================================================" << std::endl
    << "                    Id: " << id << std::endl
    << "               Address: " << address << std::endl
    << " Coordinator<id, addr>: " << coordinator.id << ", "<< coordinator.address << std::endl
    << "          Energy Level: " << energy_level << std::endl
    << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl
    << " Known Nodes: " << std::endl;
    for(const auto& node : knownNodes){
        std::cout << node.id << ", " << node.address  << std::endl;
    }
    std::cout 
    << "-------------------------------------------------------------------" << std::endl
    << " Known sensors: " << std::endl;
    for(const auto& sensor : knownSensors){
        std::cout << sensor.id << ", " << sensor.address  << std::endl;
    }
    std::cout 
    << "###################################################################" << std::endl
    << std::endl;
}

void PeerStatus::setElectionOrigin(bool set) {
    ElectionOrigin = set;
}

bool PeerStatus::isElectionOrigin() {
    return ElectionOrigin;
}