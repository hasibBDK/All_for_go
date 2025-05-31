#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

bool flag[2] = {false, false};  // interested flags
int turn = 0;                   // whose turn

void enter_region(int process) {
    int other = 1 - process;
    flag[process] = true;       // this process is interested
    turn = other;               // give turn to the other
    while (flag[other] && turn == other); // busy wait
}

void leave_region(int process) {
    flag[process] = false;      // done with critical section
}

void peterson(int id) {
    while (true) {
        enter_region(id);

        // Critical Section
        cout << "Process " << id << " is using the printer..." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));

        leave_region(id);

        // Remainder Section
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {
    thread P0(peterson, 0);
    thread P1(peterson, 1);

    P0.join();
    P1.join();

    return 0;
}
