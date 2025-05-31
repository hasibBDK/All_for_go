#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

int shared_counter = 0;
bool in_critical_section = false;

mutex mtx;
condition_variable cv;

bool waiting_high_priority = false; // Flag if high-priority process is waiting

void process(int id, int times, bool is_high_priority) {
    string priority = is_high_priority ? "HIGH" : "LOW";

    for (int i = 0; i < times; ++i) {
        unique_lock<mutex> lock(mtx);

        // If high priority, raise the flag
        if (is_high_priority)
            waiting_high_priority = true;

        // Wait until CS is free and no high-priority process is blocked (if low-priority)
        while (in_critical_section || (!is_high_priority && waiting_high_priority)) {
            cout << "[" << priority << " P" << id << "] sleeping (CS busy or high-priority waiting)\n";
            cv.wait(lock);
        }

        // Reset high-priority flag if this process is the high-priority one
        if (is_high_priority)
            waiting_high_priority = false;

        // Enter critical section
        in_critical_section = true;
        cout << "**** [" << priority << " P" << id << "] ENTERED Critical Section ****\n";

        int temp = shared_counter;
        this_thread::sleep_for(chrono::milliseconds(400));
        shared_counter = temp + 1;

        cout << "[" << priority << " P" << id << "] updated shared_counter to " << shared_counter << "\n";

        // Leave critical section
        in_critical_section = false;
        cout << "[" << priority << " P" << id << "] leaving CS and notifying others\n";

        lock.unlock();
        cv.notify_all(); // notify all to allow high-priority to wake first

        this_thread::sleep_for(chrono::milliseconds(300)); // Remainder section
    }
}

int main() {
    cout << "\n========= Sleep and Wake-Up with Priority =========\n";

    thread high_priority_thread(process, 0, 3, true);   // High Priority
    thread low_priority_thread(process, 1, 3, false);   // Low Priority

    high_priority_thread.join();
    low_priority_thread.join();

    cout << "\n[Result] shared_counter = " << shared_counter << " (Expected: 6)\n";
    return 0;
}