#include <iostream>

#include "buffer_queue.h"
#include "monitor.h"


BufferQueue::BufferQueue(std::string queueName) :full(Condition()), empty(Condition()), priority(Condition()),
name(queueName) {}

void BufferQueue::push(int value, bool withPriority) {
    this->enter();

    if (queue.size() == QUEUE_SIZE)
        this->wait(full);
    
    if (withPriority) {
        if (queue.size() >= 2 && queue.at(0) > queue.at(1))
            this->wait(priority);

        queue.insert(queue.begin(), value);
    }
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

    this->signal(priority);

    if (queue.size() == QUEUE_SIZE - 1)
        this->signal(full);

    this->leave();
}

int BufferQueue::getSecondElementValue() {
    if (queue.size() >= 2)
        return queue.at(1);
    return -1;
}

int BufferQueue::getFirstElementValue() {
    if (queue.size())
        return queue.at(0);
    return -1;
}

std::ostream& operator<<(std::ostream &os, const BufferQueue &buffer) {
    os << "BufferQueue - " << buffer.name << "': [";
    for (const int &value : buffer.queue) {
        os << value << " ";
    }
    os << "]";
    return os;
}
