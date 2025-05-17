#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to display memory layout
void printMemory(const vector<int>& memory) {
    cout << "\nMemory Status:\n";
    for (int cell : memory) {
        cout << cell;
    }
    cout << endl;
}

int main() {
    int totalMemory = 0, numBlocks = 0;

    // Input validation
    while (true) {
        cout << "Enter total memory size: ";
        cin >> totalMemory;
        cout << "Enter number of blocks: ";
        cin >> numBlocks;

        if (totalMemory > 0 && numBlocks > 0 && totalMemory % numBlocks == 0)
            break;
        cout << "❌ Invalid input! Total memory must be divisible by number of blocks and greater than 0.\n";
    }

    int blockSize = totalMemory / numBlocks;
    cout << "Block size = " << blockSize << endl;

    vector<int> memory(totalMemory, 0);  // Raw memory array

    int numProcesses;
    cout << "Enter number of processes: ";
    cin >> numProcesses;

    for (int i = 0; i < numProcesses; ++i) {
        if (i >= numBlocks) {
            cout << "Memory full. No more blocks for process " << i + 1 << ".\n";
            break;
        }

        string name;
        int size;

        cout << "Enter process name: ";
        cin >> name;
        cout << "Enter size of process " << name << ": ";
        cin >> size;

        if (size > blockSize) {
            cout << "❌ Process size exceeds block size (" << blockSize << "). Skipping.\n";
            continue;
        }

        int startIdx = i * blockSize;

        // Mark memory
        for (int j = 0; j < size; ++j)
            memory[startIdx + j] = 1;
    }

    printMemory(memory);

    return 0;
}
//give me the code of worst fit (memory management)
