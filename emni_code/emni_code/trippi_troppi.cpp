#include<bits/stdc++.h>
using namespace std;
int main()
{
ios_base::sync_with_stdio(false);
cin.tie(NULL);
    int t;
    cin >> t;
    while(t--)
    {
         string s,s1,s2; 
        cin >> s >> s1 >> s2;
        string k = string(1, s[0]) + string(1, s1[0]) + string(1, s2[0]);
        cout<<k<<endl;
  }
    return 0;
}