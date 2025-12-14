#ifndef LAMPORT_H
#define LAMPORT_H

#include <vector>
#include <cstdint>
#include <mutex>

class Lamport
{
public:
    Lamport();

    int64_t updateTimestamp();
    int64_t updateTimestamp(int64_t);

    int64_t curTimestamp();

private:
    int _curTimestamp;
};

#endif // LAMPORT_H
