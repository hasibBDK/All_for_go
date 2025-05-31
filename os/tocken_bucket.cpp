#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

using namespace std;

const int bucketCapacity = 15, tokenRate = 3, packets[5] = {1, 5, 6, 2, 7};
int tokens = 0;

mutex mtx;
queue<int> packetQueue;     // Queue for pending packets
bool running = true;        // Flag to stop threads gracefully

void generateTokens() {
    while (running) {
        {
            lock_guard<mutex> lock(mtx);
            tokens = min(bucketCapacity, tokens + tokenRate);
            cout << "[Token Gen] Tokens: +" << tokenRate << ", Total = " << tokens << "\n\n";

            // Try sending packets from the queue
            while (!packetQueue.empty() && tokens >= packetQueue.front()) {
                int pkt = packetQueue.front();
                tokens -= pkt;
                cout << "[Send] Sent packet of size " << pkt << ". Tokens left: " << tokens << "\n\n";
                packetQueue.pop();
            }
        }
        this_thread::sleep_for(chrono::seconds(4));
    }
}

void receivePacket(int packetSize) {
    lock_guard<mutex> lock(mtx);
    if (tokens >= packetSize) {
        tokens -= packetSize;
        cout << "-----[Send] Packet of size " << packetSize << " sent immediately. Tokens left: " << tokens << "\n\n";
    } else {
        packetQueue.push(packetSize);
        cout << "------[Queue] Not enough tokens. Packet of size " << packetSize << " added to queue.\n\n";
    }
}

void inputPackets() {
    while (running) {
        int size;
        cin >> size;

        if (size == -1) {
            running = false;
            break;
        }

        if (size <= 0) {
            cout << "[Error] Packet size must be positive.\n";
            continue;
        } else {
            receivePacket(size);
        }
    }
    // Run this portion to make the packets pre defined.
    // This will send the packets at a random interval
    // between 1 to 5 seconds.
    /*
    srand(time(0));
    for (int i = 0; i < 5; ++i) {
        int delay = rand() % 5 + 1;
        this_thread::sleep_for(chrono::seconds(delay));
        receivePacket(packets[i]);
    }
    while (!packetQueue.empty())
        continue;
    running = false;
    */
}

int main() {
    cout << "---------------Token Bucket Algorithm.---------------------\n\n";
    cout << "Enter new packet of any size whenever you want. Press -1 to end the Program.\n\n";
    thread tokenThread(generateTokens);
    thread inputThread(inputPackets);

    tokenThread.join();
    inputThread.join();

    cout << "[Exit] Program ended.\n";
    return 0;
}