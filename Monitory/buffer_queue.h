#ifndef BUFFER_QUEUE_H
#define BUFFER_QUEUE_H

#include <string>
#include <vector>

#include "monitor.h"

#define A_TOOL 65
#define B_TOOL 66
#define QUEUE_SIZE 10

class BufferQueue: public Monitor {
private:
    const std::string name;
    std::vector<int> queue;
    Condition full;
    Condition empty;
public:
    BufferQueue(std::string queueName);

    void push(int value, bool withPriority=false);
    void pop(int *value);

    friend std::ostream& operator<<(std::ostream &os, const BufferQueue &buffer);
};

#endif // BUFFER_QUEUE_H
