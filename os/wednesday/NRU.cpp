#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Page {
    int number;
    int R, M; 
};

int main() {
    int n, frameCount;

    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter the page reference string:\n";
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cout << "Enter number of frames: ";
    cin >> frameCount;

    vector<Page> frames;
    int pageFaults = 0;

    srand(time(0)); 

    for (int i = 0; i < n; i++) {
        int current = pages[i];
        bool found = false;

        
        for (auto &f : frames) {
            if (f.number == current) {
                f.R = 1; // referenced
                found = true;
                break;
            }
        }

        if (!found) {
            pageFaults++;

            
            if (frames.size() < frameCount) {
                frames.push_back({current, 1, rand() % 2}); 
            } else {
                
                int replaceIndex = -1;
                int classFound = 4;

                for (int c = 0; c < 4; c++) {
                    for (int j = 0; j < frames.size(); j++) {
                        int r = frames[j].R;
                        int m = frames[j].M;
                        int currentClass = r * 2 + m;

                        if (currentClass == c) {
                            replaceIndex = j;
                            classFound = c;
                            break;
                        }
                    }
                    if (replaceIndex != -1) break;
                }

                
                cout << "Replacing page " << frames[replaceIndex].number << " (Class " << classFound << ") with page " << current << "\n";
                frames[replaceIndex] = {current, 1, rand() % 2};
            }
        }

        
        if ((i + 1) % 4 == 0) {
            for (auto &f : frames) f.R = 0;
        }

        
        cout << "Frames after accessing " << current << ": ";
        for (auto f : frames) {
            cout << f.number << "(R=" << f.R << ",M=" << f.M << ") ";
        }
        cout << (found ? "→ No Page Fault" : "→ Page Fault") << "\n";
    }

    cout << "\nTotal Page Faults = " << pageFaults << endl;

    return 0;
}
//1 2 3 2 4 1 5 2