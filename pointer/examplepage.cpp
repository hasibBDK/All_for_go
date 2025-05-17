#include<bits/stdc++.h>
using namespace std;
const int page_size = 4;
const int page_table_size = 4;
int page_table[page_table_size] = {5, 6, 1, 2};
int logical_To_physical(int logicalAddress)
{
    int pageNumber = logicalAddress / page_size;
    int offset = logicalAddress % page_size;
    if (pageNumber >= page_table_size)
    {
        cout <<"Invalid logical address"<<endl;
        return -1;
    }
    int frameNumber = page_table[pageNumber];
    int physicalAddress = frameNumber * page_size + offset;
    return physicalAddress;
}
int main()
{
    int logicalAddress;
    cout << "Enter logical address (0 to 15): ";
    cin >> logicalAddress;
    int physicalAddress = logical_To_physical(logicalAddress);
    if (physicalAddress != -1)
    {
        cout << "Physical address: " << physicalAddress << endl;
    }
    return 0;
}