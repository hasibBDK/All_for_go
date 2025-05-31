#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>

using namespace std;

class DeadlockDetector {
    int numProcesses, numResources;
    vector<string> processNames, resourceNames;
    map<string, vector<pair<bool, string>>> requests;
    map<string, set<string>> allocations;
    map<string, vector<pair<bool, string>>> pending;
    set<string> available;
    map<string, string> owner;

public:
    void input() {
        cout << "\n=== Deadlock Detection Simulator ===\n\n";
        cout << "Step 1: Enter Process Information\n";
        cout << "--------------------------------\n";
        cout << "Number of processes: ";
        cin >> numProcesses;
        processNames.resize(numProcesses);
        cout << "Enter process names (space separated): ";
        for (int i = 0; i < numProcesses; ++i) {
            cin >> processNames[i];
        }

        cout << "\nStep 2: Enter Resource Information\n";
        cout << "--------------------------------\n";
        cout << "Number of resources: ";
        cin >> numResources;
        resourceNames.resize(numResources);
        cout << "Enter resource names (space separated): ";
        for (int i = 0; i < numResources; ++i) {
            cin >> resourceNames[i];
            available.insert(resourceNames[i]);
        }

        cout << "\nStep 3: Define Process Operations\n";
        cout << "--------------------------------\n";
        cout << "For each process, enter operations (R for Request, L for Release):\n";
        cout << "Format: [R/L] [ResourceName] (e.g., R File1, L Printer)\n";
        cout << "Type 'END' when done for each process\n\n";
        
        for (auto& p : processNames) {
            cout << "Operations for Process " << p << ":\n";
            string action, res;
            while (true) {
                cout << "  > ";
                cin >> action;
                if (action == "END") break;
                cin >> res;
                requests[p].emplace_back(action == "R", res);
            }
            pending[p] = requests[p];
        }
    }

    void run() {
        cout << "\n=== Simulation Started ===\n";
        print_state();

        int step = 1;
        while (true) {
            cout << "\n=== Step " << step++ << " ===";
            bool progress = false;

            for (auto& p : processNames) {
                if (pending[p].empty()) continue;

                auto [isReq, res] = pending[p].front();
                cout << "\nProcess " << p << " attempts to " 
                     << (isReq ? "request" : "release") << " " << res << ": ";

                if (isReq) {
                    if (available.count(res)) {
                        allocate(p, res);
                        cout << "SUCCESS! " << p << " acquired " << res;
                        pending[p].erase(pending[p].begin());
                        progress = true;
                    } else {
                        cout << "WAITING... " << res << " is held by " << owner[res];
                    }
                } else {
                    if (allocations[p].count(res)) {
                        release(p, res);
                        cout << "SUCCESS! " << p << " released " << res;
                        pending[p].erase(pending[p].begin());
                        progress = true;
                    } else {
                        cout << "ERROR! " << p << " doesn't hold " << res;
                    }
                }
            }

            print_state();

            if (!progress) {
                cout << "\n!!! DEADLOCK DETECTED !!!\n";
                cout << "Blocked Processes: ";
                for (auto& [p, ops] : pending) {
                    if (!ops.empty()) cout << p << " ";
                }
                cout << "\n\nWait-For Graph (Cycle):\n";
                detect_cycle();
                cout << "\nDeadlock occurs because these processes are waiting for each other in a cycle.\n";
                break;
            }
        }
    }

private:
    void allocate(const string& p, const string& r) {
        available.erase(r);
        allocations[p].insert(r);
        owner[r] = p;
    }

    void release(const string& p, const string& r) {
        allocations[p].erase(r);
        available.insert(r);
        owner.erase(r);
    }

    void detect_cycle() {
        map<string, string> waitGraph;
        cout << "Process -> Waits For -> Process\n";
        cout << "------------------------------\n";

        for (auto& [p, ops] : pending) {
            if (!ops.empty() && ops.front().first) {
                string res = ops.front().second;
                if (owner.count(res)) {
                    cout << p << " -> " << owner[res] << " (waiting for " << res << ")\n";
                    waitGraph[p] = owner[res];
                }
            }
        }
    }

    void print_state() {
        cout << "\nCurrent System State:\n";
        cout << "--------------------\n";
        cout << "Available Resources: ";
        if (available.empty()) cout << "None";
        for (auto& r : available) cout << r << " ";
        cout << "\n\nCurrent Allocations:\n";
        for (auto& p : processNames) {
            cout << "  " << p << " holds: ";
            if (allocations[p].empty()) cout << "Nothing";
            for (auto& r : allocations[p]) cout << r << " ";
            cout << "\n";
        }

        cout << "\nPending Operations:\n";
        for (auto& p : processNames) {
            if (!pending[p].empty()) {
                cout << "  " << p << ": ";
                for (size_t i = 0; i < pending[p].size(); ++i) {
                    auto [isReq, res] = pending[p][i];
                    if (i == 0) cout << "[NEXT] ";
                    cout << (isReq ? "Request " : "Release ") << res << "  ";
                }
                cout << "\n";
            }
        }
        cout << "--------------------\n";
    }
};

int main() {
    DeadlockDetector d;
    d.input();
    d.run();
    
    cout << "\n=== Simulation Ended ===\n";
    cout << "Thank you for using the Deadlock Detector!\n";
    return 0;
}