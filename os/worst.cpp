#include<bits/stdc++.h>
using namespace std;
int worstfit(int blockSize[],int processSize[],int m,int n)
{
    int allocation[n];
    for(int i=0;i<n;i++)
    {
        allocation[i]=-1;
    }
    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }

        
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }
    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++) {
        cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}


int main()
{
    int m,n;
    cin>>m>>n;
    int blockSize[m];
    int processSize[n] ;
    for(int i=0;i<m;i++)
    {
        cin>>blockSize[i];
    }
    for(int i=0;i<m;i++)
    {
        cin>>processSize[i];
    }

    worstfit(blockSize,processSize,m,n);
 
    return 0;
}