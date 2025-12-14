#ifndef NODE_STATUS_HPP
#define NODE_STATUS_HPP

#include<list>
#include<string>

struct Peer {
    int id;
    std::string ip;
};

class NodeStatus {
public:
    static NodeStatus& getInstance();

    // KnownNodes
    void addKnownNodes(Peer node);
    void removeKnownNodes(Peer node);
    void updateKnownNodes(const std::list<Peer>& nodes);
    std::list<Peer> copyKnownNodes();

    // KnownSensors
    void addKnownSensors(Peer sensors);
    void removeKnownSensors(Peer sensors);
    void updateKnownSensors(const std::list<Peer>& sensors);
    std::list<Peer> copyKnownSensors();

    // Get Values
    int getEnergyLevel();
    int getId();

private:
    const int id;
    int energy_level;

    std::list<Peer> knownNodes;
    std::list<Peer> knownSensors;

    NodeStatus();
};

#endif