#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int PAGE_SIZE;
int LOGICAL_ADDRESS_SPACE;
int PHYSICAL_ADDRESS_SPACE;
int PAGE_TABLE_SIZE;
int FRAME_COUNT;
vector<int> pageTable;

int getSizeInBytes(int value, char unit)
{
    switch (unit)
    {
    case 'B':
        return value;
    case 'K':
        return value * 1024;
    case 'M':
        return value * 1024 * 1024;
    case 'G':
        return value * 1024 * 1024 * 1024;
    default:
        cout << "Invalid unit! Defaulting to bytes.\n";
        return value;
    }
}

int logicalToPhysical(int logicalAddress)
{
    int pageNumber = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;

    cout << "\n--- Logical to Physical Conversion ---\n";
    cout << "Logical Address = " << logicalAddress << " bytes\n";
    cout << "Page Size = " << PAGE_SIZE << " bytes\n";
    cout << "Page Number = Logical Address / Page Size = " << logicalAddress << " / " << PAGE_SIZE << " = " << pageNumber << endl;
    cout << "Offset = Logical Address % Page Size = " << logicalAddress << " % " << PAGE_SIZE << " = " << offset << endl;

    if (pageNumber >= PAGE_TABLE_SIZE || pageNumber < 0)
    {
        cout << "Invalid logical address! Page number out of bounds.\n";
        return -1;
    }

    int frameNumber = pageTable[pageNumber];
    if (frameNumber == -1)
    {
        cout << "Page " << pageNumber << " is not assigned to any frame!\n";
        return -1;
    }

    int physicalAddress = frameNumber * PAGE_SIZE + offset;

    cout << "Frame Number from Page Table = " << frameNumber << endl;
    cout << "Physical Address = Frame Number * Page Size + Offset = " << frameNumber << " * " << PAGE_SIZE << " + " << offset << " = " << physicalAddress << " bytes\n";
    return physicalAddress;
}

int physicalToLogical(int physicalAddress)
{
    int frameNumber = physicalAddress / PAGE_SIZE;
    int offset = physicalAddress % PAGE_SIZE;

    cout << "\n--- Physical to Logical Conversion ---\n";
    cout << "Physical Address = " << physicalAddress << " bytes\n";
    cout << "Page Size = " << PAGE_SIZE << " bytes\n";
    cout << "Frame Number = Physical Address / Page Size = " << physicalAddress << " / " << PAGE_SIZE << " = " << frameNumber << endl;
    cout << "Offset = Physical Address % Page Size = " << physicalAddress << " % " << PAGE_SIZE << " = " << offset << endl;

    if (frameNumber >= FRAME_COUNT || frameNumber < 0)
    {
        cout << "Invalid physical address! Frame number out of range.\n";
        return -1;
    }

    for (int i = 0; i < PAGE_TABLE_SIZE; ++i)
    {
        if (pageTable[i] == frameNumber)
        {
            int logicalAddress = i * PAGE_SIZE + offset;
            cout << "Page Number = Index of Frame in Page Table = " << i << endl;
            cout << "Logical Address = Page Number * Page Size + Offset = " << i << " * " << PAGE_SIZE << " + " << offset << " = " << logicalAddress << " bytes\n";
            return logicalAddress;
        }
    }

    cout << "Frame number not found in page table!\n";
    return -1;
}

int main()
{
    int value;
    char unit;

    cout << "Choose unit for Logical Address Space (B = Bytes, K = KB, M = MB, G = GB): ";
    cin >> unit;
    cout << "Enter Logical Address Space value: ";
    cin >> value;
    LOGICAL_ADDRESS_SPACE = getSizeInBytes(value, toupper(unit));

    // Physical Address Space
    cout << "Choose unit for Physical Address Space (B = Bytes, K = KB, M = MB, G = GB): ";
    cin >> unit;
    cout << "Enter Physical Address Space value: ";
    cin >> value;
    PHYSICAL_ADDRESS_SPACE = getSizeInBytes(value, toupper(unit));

    // Page Size
    cout << "Choose unit Page Size (B = Bytes, K = KB, M = MB, G = GB): ";
    cin >> unit;
    cout << "Enter Page Size value: ";
    cin >> value;
    PAGE_SIZE = getSizeInBytes(value, toupper(unit));

    if (LOGICAL_ADDRESS_SPACE % PAGE_SIZE != 0)
    {
        cout << "Warning: Logical address space not perfectly divisible by page size. Adjusting...\n";
    }

    PAGE_TABLE_SIZE = LOGICAL_ADDRESS_SPACE / PAGE_SIZE;
    FRAME_COUNT = PHYSICAL_ADDRESS_SPACE / PAGE_SIZE;

    cout << "\nTotal Pages: " << PAGE_TABLE_SIZE << endl;
    cout << "Total Frames: " << FRAME_COUNT << endl;

    pageTable.resize(PAGE_TABLE_SIZE);
    cout << "Enter page table entries (frame numbers, -1 if unassigned):\n";

    for (int i = 0; i < PAGE_TABLE_SIZE; ++i)
    {
        int frame;
        while (true)
        {
            cout << "Page " << i << " -> Frame: ";
            cin >> frame;
            if (frame == -1 || (frame >= 0 && frame < FRAME_COUNT))
            {
                pageTable[i] = frame;
                break;
            }
            else
            {
                cout << "Invalid frame number! Must be between 0 and " << FRAME_COUNT - 1 << " or -1.\n";
            }
        }
    }

    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Logical to Physical\n";
        cout << "2. Physical to Logical\n";
        cout << "3. Exit\n";
        cout << "Choose option: ";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            int logicalAddress;
            cout << "Enter logical address (in bytes): ";
            cin >> logicalAddress;
            int result = logicalToPhysical(logicalAddress);
            if (result != -1)
                cout << "Physical address = " << result << " bytes\n";
        }
        else if (choice == 2)
        {
            int physicalAddress;
            cout << "Enter physical address (in bytes): ";
            cin >> physicalAddress;
            int result = physicalToLogical(physicalAddress);
            if (result != -1)
                cout << "Logical address = " << result << " bytes\n";
        }
        else if (choice == 3)
        {
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}