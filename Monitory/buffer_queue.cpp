#include <iostream>

#include "buffer_queue.h"
#include "monitor.h"


BufferQueue::BufferQueue(std::string queueName) :full(Condition()), empty(Condition()), 
name(queueName) {}

void BufferQueue::push(int value, bool withPriority) {
    this->enter();

    if (queue.size() == QUEUE_SIZE)
        this->wait(full);
    
    if (withPriority)
        queue.insert(queue.begin(), value);
    else 
        queue.push_back(value);
    
    if (queue.size() == 1)
        this->signal(empty);
    
    this->leave();
}


void BufferQueue::pop(int *value) {
    this->enter();

    if (queue.empty())
        this->wait(empty);

    *value = queue.front();
    queue.erase(queue.begin());

    if (queue.size() == QUEUE_SIZE - 1)
        this->signal(full);

    this->leave();
}

std::ostream& operator<<(std::ostream &os, const BufferQueue &buffer) {
    os << "BufferQueue - " << buffer.name << "': [";
    for (const int &value : buffer.queue) {
        os << value << " ";
    }
    os << "]";
    return os;
}
