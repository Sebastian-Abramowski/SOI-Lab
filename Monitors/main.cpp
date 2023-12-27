#include <cstdio>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>

#include "buffer_queue.h"


BufferQueue bufferOrders("Orders");
BufferQueue bufferWarehouse("Warehouse");
BufferQueue bufferTools("Tools");

void processClient(float secondsToSleep, bool withPriority) {
    while (true) {
        bufferOrders.push((int)(time(NULL)), withPriority);
        float multiplier = 0.5 + ((float)rand() / ((float)RAND_MAX));
        int microsecondsToSleep = (int)(secondsToSleep * multiplier * 1000000);
        std::this_thread::sleep_for(std::chrono::microseconds(microsecondsToSleep));
    }
}

void processCreation() {
    int tool, orderTime;
    while (true) {
        bufferTools.pop(&tool);
        if (tool == A_TOOL)
            bufferTools.push(A_TOOL);
        else
            bufferTools.push(B_TOOL);
        bufferOrders.pop(&orderTime);

        if (tool == A_TOOL)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        else
            std::this_thread::sleep_for(std::chrono::seconds(2));
        
        bufferWarehouse.push(orderTime);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void processDelivery() {
    std::ofstream file("delivery_info.txt", std::ios::out);
    int counter=0;
    while(true) {
        std::this_thread::sleep_for(std::chrono::microseconds(250'000));  
        int orderTime;
        bufferWarehouse.pop(&orderTime);

        // Not writing the first measurment
        if (counter++)
            file << "Od przyjecia zlecenia do wydania go kurierowi minelo " << (time(NULL) - orderTime) << " sekund\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(250'000));    
    }
}

void showQueues() {
    while(true) {
        std::cout<< "-----------------" << std::endl;
        std::cout<<bufferTools<<std::endl;
        std::cout<<bufferOrders<<std::endl;
        std::cout<<bufferWarehouse<<std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(500'000));
        fflush(stdout);       
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <client_sleep_time>\n";
        exit(1);
    }

    float clientSleepTime;
    try {
        clientSleepTime = std::stof(argv[1]);
    }
    catch(const std::exception &e) {
        std::cerr << "Nieudana konwersja arguemntu na floata: " << argv[1] << std::endl;
        exit(2);
    }

    bufferTools.push(A_TOOL);
    bufferTools.push(B_TOOL);

    std::vector<std::thread> threadsToRun;

    for(int i=0; i<3; i++) {
        threadsToRun.push_back(std::thread(processClient, clientSleepTime, false));
        threadsToRun.push_back(std::thread(processCreation));
    }
    threadsToRun.push_back(std::thread(processClient, clientSleepTime, true)); // Priority client
    threadsToRun.push_back(std::thread(processDelivery));
    threadsToRun.push_back(std::thread(showQueues));

    for(std::thread &th: threadsToRun)
        th.join();
}
