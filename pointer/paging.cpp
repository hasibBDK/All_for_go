#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

int PAGE_SIZE;
int PAGE_TABLE_SIZE;
vector<int> pageTable;

long long convertToBytes(string input, string &unit) {
    long long value = 0;
    int i = 0;
    while (i < input.size() && isdigit(input[i])) {
        value = value * 10 + (input[i] - '0');
        i++;
    }
    unit = input.substr(i);
    if (unit == "KB" || unit == "kb") value *= 1024;
    else if (unit == "MB" || unit == "mb") value *= 1024 * 1024;
    else if (unit == "B" || unit == "b" || unit == "") value *= 1;
    else {
        cout << "Invalid unit. Use B, KB or MB.\n";
        return -1;
    }
    return value;
}

void displayAddress(long long bytes, string unit) {
    if (unit == "KB" || unit == "kb")
        cout << bytes / 1024 << " KB\n";
    else if (unit == "MB" || unit == "mb")
        cout << bytes / (1024 * 1024) << " MB\n";
    else
        cout << bytes << " B\n";
}

int logicalToPhysical(long long logicalAddress) {
    int pageNumber = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;

    if (pageNumber >= PAGE_TABLE_SIZE || pageNumber < 0) {
        cout << "Invalid logical address!\n";
        return -1;
    }

    int frameNumber = pageTable[pageNumber];
    if (frameNumber == -1) {
        cout << "Page " << pageNumber << " is not assigned to any frame!\n";
        return -1;
    }

    return frameNumber * PAGE_SIZE + offset;
}

int main() {
    string pageSizeInput;
    string unit;
    cout << "Enter page size (e.g., 4KB, 1MB, 1024): ";
    cin >> pageSizeInput;
    PAGE_SIZE = convertToBytes(pageSizeInput, unit);
    if (PAGE_SIZE == -1) return 0;

    cout << "Enter page table size: ";
    cin >> PAGE_TABLE_SIZE;

    pageTable.resize(PAGE_TABLE_SIZE);
    cout << "Enter page table entries (frame numbers, use -1 if unassigned):\n";
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i) {
        cout << "Page " << i << " -> Frame: ";
        cin >> pageTable[i];
    }
    
      while (true) {
        cout << "\nMenu:\n";
        cout << "1. Logical to Physical\n";
        cout << "2. Exit\n";
        cout << "Choose option: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string input;
            cout << "Enter logical address (e.g., 2048B, 2KB): ";
            cin >> input;
            string inputUnit;
            long long logical = convertToBytes(input, inputUnit);
            if (logical == -1) continue;
            int result = logicalToPhysical(logical);
            if (result != -1) {
                cout << "Physical address = ";
                displayAddress(result, inputUnit);
            }
        } else if (choice == 2) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
