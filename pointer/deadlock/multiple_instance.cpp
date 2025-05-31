#include <iostream>
#include <vector>

using namespace std;

void printVector(const vector<int>& v) {
    cout << "(";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i != v.size() - 1) cout << "  ";
    }
    cout << ")";
}

bool detectDeadlock(const vector<vector<int>>& allocation,
                   const vector<vector<int>>& request,
                   vector<int> available) {
    int n = allocation.size();    
    int m = available.size();     
    vector<bool> finished(n, false);
    vector<int> work = available;
    vector<int> safeSequence;
    int count = 0;
    
    while (count < n) {
        bool found = false;
        
        for (int i = 0; i < n; ++i) {
            if (!finished[i]) {
                bool canExecute = true;
                for (int j = 0; j < m; ++j) {
                    if (request[i][j] > work[j]) {
                        canExecute = false;
                        break;
                    }
                }
                
                if (canExecute) {
                    for (int j = 0; j < m; ++j) {
                        work[j] += allocation[i][j];
                    }
                    safeSequence.push_back(i);
                    finished[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        
        if (!found) break;
    }
    
    if (count == n) {
        cout << "No deadlock. System is in safe state.\n";
        cout << "E = ";
        printVector(work);
        cout << endl;
        return false;
    } else {
        cout << "Deadlock detected in system!" << endl;
        return true;
    }
}

int main() {
    int n, m;
    
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resource types: ";
    cin >> m;
    
    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> request(n, vector<int>(m));
    vector<int> available(m);
    
    // Input allocation matrix
    cout << "\nEnter allocation matrix (row-wise):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> allocation[i][j];
        }
    }
    
    // Input request matrix
    cout << "\nEnter request matrix (row-wise):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> request[i][j];
        }
    }
    
    // Input available resources
    cout << "\nEnter available resources: ";
    for (int j = 0; j < m; ++j) {
        cin >> available[j];
    }
    
    // Display input
    cout << "\nCurrent allocation matrix:\n";
    for (const auto& row : allocation) {
        cout << "  ";
        printVector(row);
        cout << endl;
    }
    
    cout << "\nRequest matrix:\n";
    for (const auto& row : request) {
        cout << "  ";
        printVector(row);
        cout << endl;
    }
    
    cout << "\nA = ";
    printVector(available);
    cout << endl;
    
    // Perform deadlock detection
    if (!detectDeadlock(allocation, request, available)) {
        cout << "No deadlock exists in the system.\n";
    } else {
        cout << "Deadlock exists in the system!\n";
    }
    
    return 0;
}