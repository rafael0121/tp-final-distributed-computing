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
    static NodeStatus& getInstance(int id, int energy);

    void addKnownNodes(Peer node);
    void removeKnownNodes(Peer node);
    void updateKnownNodes(std::list<Peer> nodes);

    void addKnownSensors(Peer sensors);
    void removeKnownSensors(Peer sensors);
    void updateKnownSensors(const std::list<Peer>& sensors);

private:
    const int id;
    int energy_level;

    std::list<Peer> knownNodes;
    std::list<Peer> knownSensors;

    NodeStatus(int id, int energy);
};

#endif