#include <iostream>
#include <vector>
using namespace std;

struct Page {
    int number;
    bool referenceBit;
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
    int pointer = 0; 
    int pageFaults = 0;

    for (int i = 0; i < n; i++) {
        int current = pages[i];
        bool found = false;

        
        for (auto &f : frames) {
            if (f.number == current) {
                f.referenceBit = true; // Give second chance
                found = true;
                break;
            }
        }

        if (!found) {
            pageFaults++;

            if (frames.size() < frameCount) {
                frames.push_back({current, true});
            } else {
                while (true) {
                    if (!frames[pointer].referenceBit) {
                        cout << "Replacing page " << frames[pointer].number << " with " << current << endl;
                        frames[pointer] = {current, true};
                        pointer = (pointer + 1) % frameCount;
                        break;
                    } else {
                        frames[pointer].referenceBit = false;
                        pointer = (pointer + 1) % frameCount;
                    }
                }
            }
        }

       
        cout << "Frames after accessing " << current << ": ";
        for (auto f : frames) {
            cout << f.number << "(R=" << f.referenceBit << ") ";
        }
        cout << (found ? "No Page Fault" : "Page Fault") << endl;
    }

    cout << "\nTotal Page Faults = " << pageFaults << endl;

    return 0;
}
//1 2 3 2 4 1 5 2