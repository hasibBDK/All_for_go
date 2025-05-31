#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace std;

// Shared resources
int shared_counter = 0;
atomic<int> lock_var(0); // 0 = unlocked, 1 = locked
int naive_lock = 0; // non-atomic for race condition demo
mutex cout_mutex; // For synchronized output

void safe_print(const string& msg) {
    lock_guard<mutex> guard(cout_mutex);
    cout << msg << endl;
}

void process_with_proper_lock(int id) {
    for (int i = 0; i < 3; ++i) {
        safe_print("Process " + to_string(id) + ": Trying to acquire lock (current value = " + to_string(lock_var.load()) + ")");

        while (true) {
            int expected = 0;
            if (lock_var.compare_exchange_weak(expected, 1)) {
                safe_print("Process " + to_string(id) + ": Lock acquired (lock_var = " + to_string(lock_var.load()) + ")");
                break;
            }
            safe_print("Process " + to_string(id) + ": Lock busy (lock_var = " + to_string(lock_var.load()) + "), retrying...");
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        // Critical section
        safe_print("Process " + to_string(id) + ": ENTERED critical section");
        int temp = shared_counter;
        this_thread::sleep_for(chrono::milliseconds(200)); // Simulate work
        shared_counter = temp + 1;
        safe_print("Process " + to_string(id) + ": Updated counter to " + to_string(shared_counter));

        // Exit section
        lock_var = 0;
        safe_print("Process " + to_string(id) + ": Exiting critical section, released lock (lock_var = " + to_string(lock_var.load()) + ")");
        cout << endl << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

void process_with_race_condition(int id) {
    for (int i = 0; i < 3; ++i) {
        safe_print("Process " + to_string(id) + ": Checking lock (naive_lock = " + to_string(naive_lock) + ")");

        if (naive_lock == 0) {
            this_thread::sleep_for(chrono::milliseconds(100)); // Simulate delay before setting
            naive_lock = 1;
            safe_print("Process " + to_string(id) + ": Naively assumes lock acquired (naive_lock = " + to_string(naive_lock) + ")");

            // Critical section
            safe_print("Process " + to_string(id) + ": ENTERED critical section UNSAFELY");
            int temp = shared_counter;
            this_thread::sleep_for(chrono::milliseconds(200));
            shared_counter = temp + 1;
            safe_print("Process " + to_string(id) + ": Updated counter to " + to_string(shared_counter));

            naive_lock = 0;
            safe_print("Process " + to_string(id) + ": Exiting critical section, released lock (naive_lock = " + to_string(naive_lock) + ")");
        } else {
            safe_print("Process " + to_string(id) + ": Lock already held (naive_lock = " + to_string(naive_lock) + "), skipping this round");
        }

        cout << endl << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

int main() {
    cout << "\n================= MUTUAL EXCLUSION DEMO =================\n\n";

    // DEMO 1: Race Condition
    cout << ">>> DEMO 1: Race Condition (No Proper Locking)\n";
    cout << "Expect incorrect results and overlapping critical sections.\n\n";
    shared_counter = 0;
    naive_lock = 0;
    thread t1(process_with_race_condition, 1);
    thread t2(process_with_race_condition, 2);
    t1.join();
    t2.join();
    cout << "\nFinal counter value (should be 6): " << shared_counter
         << "  -->  INCORRECT due to race condition.\n\n";

    // DEMO 2: Proper Locking
    cout << ">>> DEMO 2: Proper Lock with Compare-and-Swap\n";
    cout << "Each process will safely acquire and release the lock.\n\n";
    shared_counter = 0;
    lock_var = 0;
    thread t3(process_with_proper_lock, 1);
    thread t4(process_with_proper_lock, 2);
    t3.join();
    t4.join();
    cout << "\nFinal counter value (should be 6): " << shared_counter
         << "  -->  CORRECT with proper locking.\n";

    return 0;
}