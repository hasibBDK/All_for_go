#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int SEGMENT_COUNT;
unordered_map<int, pair<int, int>> segmentTable; 
int getPhysicalAddress(int segmentNumber, int offset)
{
    if (segmentTable.find(segmentNumber) == segmentTable.end())
    {
        cout << "Segment " << segmentNumber << " not initialized!" << endl;
        return -1;
    }

    int base = segmentTable[segmentNumber].first;
    int limit = segmentTable[segmentNumber].second;

    if (offset < 0 || offset >= limit)
    {
        cout << "Segmentation fault: Offset exceeds limit!" << endl;
        return -1;
    }

    return base + offset;
}

int main()
{
    cout << "Enter total number of segments: ";
    cin >> SEGMENT_COUNT;

    int usedSegments;
    cout << "How many segments will you input now? ";
    cin >> usedSegments;

    for (int i = 0; i < usedSegments; ++i)
    {
        int segNum, base, limit;
        cout << "Enter segment number: ";
        cin >> segNum;

        if (segNum < 0 || segNum >= SEGMENT_COUNT)
        {
            cout << "Invalid segment number. Try again.\n";
            --i;
            continue;
        }

        cout << "Segment " << segNum << " Base: ";
        cin >> base;
        cout << "Segment " << segNum << " Limit: ";
        cin >> limit;

        segmentTable[segNum] = {base, limit};
    }

    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Segment + Offset --> Physical Address\n";
        cout << "2. Exit\n";
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            int seg, off;
            cout << "Enter Segment Number: ";
            cin >> seg;
            cout << "Enter Offset: ";
            cin >> off;
            int result = getPhysicalAddress(seg, off);
            if (result != -1)
                cout << "Physical Address = " << result << endl;
        }
        else if (choice == 2)
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
