#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <climits> // For INT_MAX
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

// Converts "10KB", "100B", "2MB" to bytes
int parseSize(string input) {
    int num = 0, i = 0;
    while (i < input.size() && isdigit(input[i]))
        num = num * 10 + (input[i++] - '0');

    string unit = "";
    while (i < input.size())
        unit += toupper(input[i++]);

    if (unit == "B" || unit == "") return num;
    else if (unit == "KB") return num * 1024;
    else if (unit == "MB") return num * 1024 * 1024;
    else if (unit == "GB") {
        cout << "GB is not supported due to limitations.\n";
        return -1;
    } else {
        cout << "Invalid unit. Use B, KB, or MB.\n";
        return -1;
    }
}

void mergeHoles() {
    sort(holes.begin(), holes.end());
    vector<Hole> merged;

    for (auto& hole : holes) {
        if (!merged.empty() && merged.back().start + merged.back().size == hole.start)
            merged.back().size += hole.size;
        else
            merged.push_back(hole);
    }

    holes = merged;
}

void printMemoryState() {
    cout << "\n--- Memory Holes ---\n";
    for (const auto& hole : holes)
        cout << "Start: " << hole.start << ", Size: " << hole.size << " bytes\n";

    cout << "\n--- Allocations ---\n";
    for (const auto& alloc : allocations)
        cout << "Process " << alloc.first << ": Start = " << alloc.second.first
             << ", Size = " << alloc.second.second << " bytes\n";

    cout << "---------------------\n";
}

void allocateMemory(int processID, int size) {
    if (allocations.find(processID) != allocations.end()) {
        cout << "Process " << processID << " is already allocated!\n";
        return;
    }

    int bestIndex = -1;
    int bestSize = INT_MAX; // Initialize with maximum possible value

    for (int i = 0; i < holes.size(); ++i) {
        if (holes[i].size >= size && holes[i].size < bestSize) {
            bestIndex = i;
            bestSize = holes[i].size;
        }
    }

    if (bestIndex == -1) {
        cout << "Allocation failed: No suitable hole found.\n";
        return;
    }

    int allocStart = holes[bestIndex].start;
    allocations[processID] = {allocStart, size};

    if (holes[bestIndex].size == size)
        holes.erase(holes.begin() + bestIndex);
    else {
        holes[bestIndex].start += size;
        holes[bestIndex].size -= size;
    }

    cout << "Memory allocated to process " << processID << " at address " << allocStart << endl;
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
    cout << "Enter the number of memory holes: ";
    cin >> holeCount;

    for (int i = 0; i < holeCount; ++i) {
        int start;
        string sizeStr;
        cout << "Enter start address for hole " << i + 1 << ": ";
        cin >> start;
        cout << "Enter size for hole " << i + 1 << " (e.g., 500B, 1KB): ";
        cin >> sizeStr;
        int size = parseSize(sizeStr);
        if (size > 0) {
            holes.push_back({start, size});
        } else {
            cout << "Invalid hole size. Skipping...\n";
        }
    }

    mergeHoles(); // Merging holes after input

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Allocate Memory (Best Fit)\n";
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
            if (size > 0)
                allocateMemory(pid, size);
        }
        else if (choice == 2) {
            int pid;
            cout << "Enter Process ID to deallocate: ";
            cin >> pid;
            deallocateMemory(pid);
        }
        else if (choice == 3)
            printMemoryState();
        else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        }
        else
            cout << "Invalid choice. Try again.\n";
    }

    return 0;
}