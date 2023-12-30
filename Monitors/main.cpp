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
    bool isPriority;
    while (true) {
        bufferTools.pop(&tool);
        if (tool == A_TOOL)
            bufferTools.push(A_TOOL);
        else
            bufferTools.push(B_TOOL);

        int secondOrderTime = bufferOrders.getSecondElementValue();
        bufferOrders.pop(&orderTime);

        if (secondOrderTime != -1 && orderTime > secondOrderTime)
            isPriority = true;
        else
            isPriority = false;

        if (tool == A_TOOL)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        else
            std::this_thread::sleep_for(std::chrono::seconds(2));
        
        bufferWarehouse.push(orderTime, isPriority);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void processDelivery() {
    std::ofstream file("delivery_info.txt", std::ios::out);
    int counter=0;
    int orderTime;
    bool isPriority;
    while(true) {
        std::this_thread::sleep_for(std::chrono::microseconds(250'000));

        int waitingOrderTime = bufferOrders.getSecondElementValue(); 
        bufferWarehouse.pop(&orderTime);

        if (waitingOrderTime != -1 && orderTime > waitingOrderTime) 
            isPriority = true;
        else
            isPriority = false;

        // Not writing the first measurment
        std::string additionalInfo = "";
        if (++counter)
            if (isPriority)
                additionalInfo = "[VIP 100%] ";
            file << additionalInfo << "Od przyjecia zlecenia do wydania go kurierowi minelo " << (time(NULL) - orderTime) << " sekund\n" << std::endl;
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
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <client_sleep_time> <vip_client_sleep_time>\n";
        exit(1);
    }

    float X;
    float X_VIP;
    try {
        X = std::stof(argv[1]);
        X_VIP = std::stof(argv[2]);
    }
    catch(const std::exception &e) {
        std::cerr << "Nieudana konwersja arguemntu na floata: " << argv[1] << std::endl;
        exit(2);
    }

    bufferTools.push(A_TOOL);
    bufferTools.push(B_TOOL);

    std::vector<std::thread> threadsToRun;

    for(int i=0; i<3; i++) {
        threadsToRun.push_back(std::thread(processClient, X, false));
        threadsToRun.push_back(std::thread(processCreation));
    }
    threadsToRun.push_back(std::thread(processClient, X_VIP, true)); // Priority client
    threadsToRun.push_back(std::thread(processDelivery));
    threadsToRun.push_back(std::thread(showQueues));

    for(std::thread &th: threadsToRun)
        th.join();
}
