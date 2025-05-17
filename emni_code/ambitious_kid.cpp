#include<bits/stdc++.h>
using namespace std;
int main()
{
ios_base::sync_with_stdio(false);
cin.tie(NULL);
    int n;cin>> n;
    int A[n];
    for(int i=0;i<n;i++)
    {
        cin>> A[i];
    }
    
    int minop  = INT_MAX;
    for(int i=0;i<n;i++)
    {
        minop= min(minop, abs(A[i]));
    }
    cout<<minop<<endl;
    return 0;
}