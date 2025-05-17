#include <iostream>
#include <queue>
#include <unordered_set>
using namespace std;

int main() {
    int n, frameCount;

    cout << "Enter number of pages: ";
    cin >> n;

    int pages[n];
    cout << "Enter the page reference string:\n";
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cout << "Enter number of frames: ";
    cin >> frameCount;

    unordered_set<int> frames;
    queue<int> indexQueue;
    int pageFaults = 0;

    for (int i = 0; i < n; i++) {
        int current = pages[i];

        cout << "Accessing page " << current << ": ";

        if (frames.find(current) == frames.end()) {
            pageFaults++;

            // Page fault occurred
            cout << "Page Fault -> ";

            if (frames.size() == frameCount) {
                int oldest = indexQueue.front();
                indexQueue.pop();
                frames.erase(oldest);
            }

            frames.insert(current);
            indexQueue.push(current);
        } else {
            // No page fault
            cout << "No Page Fault -> ";
        }

        // Print current frames
        cout << "Frames: ";
        for (int page : frames) {
            cout << page << " ";
        }
        cout << endl;
    }

    cout << "\nTotal Page Faults = " << pageFaults << endl;

    return 0;
}
//1 2 3 2 4 1 5 2