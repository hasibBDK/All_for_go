#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

struct Hole {
    int start;
    int size;

    bool operator<(const Hole& other) const {
        return start < other.start;
    }
};

map<int, pair<int, int>> allocations; // processID -> {start, size}
vector<Hole> holes;
int MEMORY_SIZE;

// Converts string like "10KB", "100B", "2MB" to bytes
int parseSize(string input) {
    int num = 0;
    int i = 0;

    // Parse the number part
    while (i < input.size() && isdigit(input[i])) {
        num = num * 10 + (input[i] - '0');
        ++i;
    }

    // Extract and convert the unit
    string unit = "";
    while (i < input.size()) {
        unit += toupper(input[i]);
        ++i;
    }

    if (unit == "B" || unit == "") return num;
    else if (unit == "KB") return num * 1024;
    else if (unit == "MB") return num * 1024 * 1024;
    else if (unit == "GB") {
        cout << "GB is not supported due to memory limitations.\n";
        return -1;
    } else {
        cout << "Invalid unit. Use B, KB, or MB.\n";
        return -1;
    }
}

void printMemoryState() {
    cout << "\n--- Memory Holes ---\n";
    for (const auto& hole : holes) {
        cout << "Start: " << hole.start << ", Size: " << hole.size << " bytes\n";
    }
    cout << "\n--- Allocations ---\n";
    for (const auto& alloc : allocations) {
        cout << "Process " << alloc.first << ": Start = " << alloc.second.first
             << ", Size = " << alloc.second.second << " bytes\n";
    }
    cout << "---------------------\n";
}

void mergeHoles() {
    sort(holes.begin(), holes.end());
    vector<Hole> merged;

    for (const auto& hole : holes) {
        if (!merged.empty() && merged.back().start + merged.back().size == hole.start) {
            merged.back().size += hole.size;
        } else {
            merged.push_back(hole);
        }
    }

    holes = merged;
}

void allocateMemory(int processID, int size) {
    if (allocations.find(processID) != allocations.end()) {
        cout << "Process " << processID << " already allocated!" << endl;
        return;
    }

    for (auto it = holes.begin(); it != holes.end(); ++it) {
        if (it->size >= size) {
            int allocStart = it->start;
            allocations[processID] = {allocStart, size};

            // Adjust or remove hole
            if (it->size == size) {
                holes.erase(it);
            } else {
                it->start += size;
                it->size -= size;
            }

            cout << "Memory allocated to process " << processID << " at address " << allocStart << endl;
            return;
        }
    }

    cout << "Allocation failed: Not enough memory.\n";
}

void deallocateMemory(int processID) {
    if (allocations.find(processID) == allocations.end()) {
        cout << "Invalid process ID!\n";
        return;
    }

    auto [start, size] = allocations[processID];
    allocations.erase(processID);

    holes.push_back({start, size});
    mergeHoles();

    cout << "Memory deallocated for process " << processID << endl;
}

int main() {
    cout << "Enter total memory size (e.g., 1000B, 2KB, 5MB): ";
    string memInput;
    cin >> memInput;

    MEMORY_SIZE = parseSize(memInput);
    if (MEMORY_SIZE <= 0) {
        cout << "Invalid memory size. Exiting.\n";
        return 1;
    }

    int holeCount;
    cout << "Enter number of memory holes: ";
    cin >> holeCount;
    for (int i = 0; i < holeCount; ++i) {
        string startStr, sizeStr;
        int start, size;
        cout << "Hole " << i + 1 << " Start Address: ";
        cin >> start;
        cout << "Hole " << i + 1 << " Size (e.g., 500B, 1KB): ";
        cin >> sizeStr;
        size = parseSize(sizeStr);

        if (start < 0 || start + size > MEMORY_SIZE || size <= 0) {
            cout << "Invalid hole range. Skipping.\n";
        } else {
            holes.push_back({start, size});
        }
    }
    mergeHoles();

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Allocate Memory (First Fit)\n";
        cout << "2. Deallocate Memory\n";
        cout << "3. Show Memory State\n";
        cout << "4. Exit\n";
        cout << "Choose option: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int pid;
            string sizeInput;
            cout << "Enter Process ID: ";
            cin >> pid;
            cout << "Enter Memory Size (e.g., 100B, 2KB, 1MB): ";
            cin >> sizeInput;
            int size = parseSize(sizeInput);
            if (size > 0) {
                allocateMemory(pid, size);
            } else {
                cout << "Allocation failed due to invalid size.\n";
            }
        } else if (choice == 2) {
            int pid;
            cout << "Enter Process ID to deallocate: ";
            cin >> pid;
            deallocateMemory(pid);
        } else if (choice == 3) {
            printMemoryState();
        } else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}