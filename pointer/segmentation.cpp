#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

struct SegmentEntry {
    int limit;
    int base; // -1 means not mapped
};

int PHYSICAL_ADDRESS_SPACE;
vector<SegmentEntry> segmentTable;

int getSizeInBytes(int value, char unit) {
    switch (toupper(unit)) {
        case 'B': return value;
        case 'K': return value * 1024;
        case 'M': return value * 1024 * 1024;
        case 'G': return value * 1024 * 1024 * 1024;
        default:
            cout << "Invalid unit! Defaulting to bytes.\n";
            return value;
    }
}

void physicalToSegmentOffset(int physicalAddress) {
    cout << "\n--- Physical to Segment:Offset Conversion ---\n";
    cout << "Physical Address = " << physicalAddress << " bytes\n";

    for (int i = 0; i < segmentTable.size(); ++i) {
        SegmentEntry seg = segmentTable[i];
        if (seg.base != -1 && physicalAddress >= seg.base && physicalAddress < seg.base + seg.limit) {
            int offset = physicalAddress - seg.base;
            cout << "Found in Segment " << i << endl;
            cout << "Base = " << seg.base << ", Limit = " << seg.limit << endl;
            cout << "Offset = Physical - Base = " << physicalAddress << " - " << seg.base << " = " << offset << " bytes\n";
            return;
        }
    }

    cout << "No segment contains this physical address!\n";
}

void segmentOffsetToPhysical(int segmentNumber, int offset) {
    cout << "\n--- Segment:Offset to Physical Address Conversion ---\n";

    if (segmentNumber < 0 || segmentNumber >= segmentTable.size()) {
        cout << "Invalid segment number!\n";
        return;
    }

    SegmentEntry seg = segmentTable[segmentNumber];

    if (seg.base == -1) {
        cout << "Segment " << segmentNumber << " is not mapped in memory!\n";
        return;
    }

    if (offset >= seg.limit || offset < 0) {
        cout << "Offset out of bounds! Must be less than segment limit (" << seg.limit << ").\n";
        return;
    }

    int physicalAddress = seg.base + offset;
    cout << "Base = " << seg.base << ", Offset = " << offset << endl;
    cout << "Physical Address = Base + Offset = " << seg.base << " + " << offset << " = " << physicalAddress << " bytes\n";
}

int main() {
    int value;
    char unit;

    // Input Physical Address Space
    cout << "Choose unit for Physical Address Space (B = Bytes, K = KB, M = MB, G = GB): ";
    cin >> unit;
    cout << "Enter Physical Address Space value: ";
    cin >> value;
    PHYSICAL_ADDRESS_SPACE = getSizeInBytes(value, unit);

    int segmentCount;
    cout << "\nEnter number of segments: ";
    cin >> segmentCount;

    // Input segment limits
    segmentTable.resize(segmentCount);
    cout << "\n--- Enter Segment Limits ---\n";
    for (int i = 0; i < segmentCount; ++i) {
        cout << "Segment " << i << " Limit (e.g., 2K, 500B): ";
        int val;
        char unit;
        cin >> val >> unit;
        segmentTable[i].limit = getSizeInBytes(val, unit);
        segmentTable[i].base = -1; // default unmapped
    }

    // Map segments to physical memory
    cout << "\n--- Map Segments to Physical Memory ---\n";
    for (int i = 0; i < segmentCount; ++i) {
        char choice;
        cout << "Map Segment " << i << " to physical memory? (y/n): ";
        cin >> choice;

        if (tolower(choice) == 'y') {
            int base;
            cout << "Enter base address for Segment " << i << " (in bytes): ";
            cin >> base;

            if (base < 0 || base + segmentTable[i].limit > PHYSICAL_ADDRESS_SPACE) {
                cout << "Invalid base! Segment would exceed physical memory limits.\n";
                --i; // retry current segment
                continue;
            }

            segmentTable[i].base = base;
        }
    }

    // Infinite loop for conversions
    while (true) {
        cout << "\n--- Menu ---\n";
        cout << "1. Physical Address ---> Segment + Offset\n";
        cout << "2. Segment + Offset ---> Physical Address\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";
        int option;
        cin >> option;

        if (option == 1) {
            int physicalAddress;
            cout << "Enter physical address (in bytes): ";
            cin >> physicalAddress;
            if (physicalAddress < 0 || physicalAddress >= PHYSICAL_ADDRESS_SPACE) {
                cout << "Invalid physical address! Out of physical memory range.\n";
                continue;
            }
            physicalToSegmentOffset(physicalAddress);
        }
        else if (option == 2) {
            int segment, offset;
            cout << "Enter segment number: ";
            cin >> segment;
            cout << "Enter offset (in bytes): ";
            cin >> offset;
            segmentOffsetToPhysical(segment, offset);
        }
        else if (option == 3) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
