#ifndef PEER_STATUS_HPP
#define PEER_STATUS_HPP

#include<list>
#include<string>

#include <lamport.hpp>

struct Peer {
    int id;
    std::string address;
};

class PeerStatus {
public:
    static PeerStatus& getInstance();

    // KnownNodes
    void addKnownNode(Peer node);
    void removeKnownNodes(Peer node);
    void updateKnownNodes(const std::list<Peer>& nodes);
    std::list<Peer> copyKnownNodes();

    // KnownSensors
    void addKnownSensor(Peer sensor);
    void removeKnownSensors(Peer sensors);
    void updateKnownSensors(const std::list<Peer>& sensors);
    std::list<Peer> copyKnownSensors();

    // Get Values
    int getEnergyLevel();
    int getId();
    std::string getAddress();

    // Set address.
    void setAddress(std::string new_address);

    // Coordinator
    Peer getCoordinator();
    void setCoordinator(Peer newCoordinator);

    // lOGS
    void printStatus();

private:
    int id;
    std::string address;

    int energy_level;

    // Coordinator
    Peer coordinator;

    std::list<Peer> knownNodes;
    std::list<Peer> knownSensors;

    PeerStatus();
};

#endif