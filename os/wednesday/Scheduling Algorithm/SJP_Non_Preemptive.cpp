#include <bits/stdc++.h>
using namespace std;

struct Process {
    string processName;
    int processID;
    int burstTime;
    int arrivalTime;
};

// Function to print the Gantt Chart
void printGanttChart(const vector<pair<string, pair<int, int>>> &timeline) {
    const int boxWidth = 8;

    // Top border
    cout << "\nGantt Chart:\n ";
    for (const auto &entry : timeline)
        cout << string(boxWidth, '-') << " ";
    cout << "\n|";

    // Process names
    for (const auto &entry : timeline) {
        string name = entry.first;
        int leftPad = (boxWidth - name.length()) / 2;
        int rightPad = boxWidth - name.length() - leftPad;
        cout << string(leftPad, ' ') << name << string(rightPad, ' ') << "|";
    }
    cout << "\n ";

    // Bottom border
    for (const auto &entry : timeline)
        cout << string(boxWidth, '-') << " ";
    cout << "\n";

    // Time labels
    int prevTime = timeline[0].second.first;
    cout << setw(2) << prevTime;
    for (const auto &entry : timeline) {
        int endTime = entry.second.second;
        int space = boxWidth - to_string(endTime).length();
        cout << string(space + 1, ' ') << endTime;
    }
    cout << "\n";
}

void calculateWaitingTime(Process processList[], int numberOfProcesses, int waitingTime[],
                          vector<string> &executionOrder,
                          vector<pair<string, pair<int, int>>> &timeline) {
    vector<bool> isCompleted(numberOfProcesses, false);
    int currentTime = 0, completedProcesses = 0;

    while (completedProcesses < numberOfProcesses) {
        int shortestProcessIndex = -1;
        int minBurstTime = INT_MAX;

        for (int i = 0; i < numberOfProcesses; i++) {
            if (!isCompleted[i] && processList[i].arrivalTime <= currentTime &&
                processList[i].burstTime < minBurstTime) {
                minBurstTime = processList[i].burstTime;
                shortestProcessIndex = i;
            }
        }

        if (shortestProcessIndex == -1) {
            currentTime++;
            continue;
        }

        Process &p = processList[shortestProcessIndex];

        executionOrder.push_back(p.processName);
        waitingTime[shortestProcessIndex] = currentTime - p.arrivalTime;
        if (waitingTime[shortestProcessIndex] < 0)
            waitingTime[shortestProcessIndex] = 0;

        // Log timeline for Gantt chart
        timeline.push_back({p.processName, {currentTime, currentTime + p.burstTime}});

        currentTime += p.burstTime;
        isCompleted[shortestProcessIndex] = true;
        completedProcesses++;
    }
}

void calculateTurnAroundTime(Process processList[], int numberOfProcesses, int waitingTime[], int turnAroundTime[]) {
    for (int i = 0; i < numberOfProcesses; i++) {
        turnAroundTime[i] = processList[i].burstTime + waitingTime[i];
    }
}

void calculateAndDisplayAverageTimes(Process processList[], int numberOfProcesses) {
    int waitingTime[numberOfProcesses], turnAroundTime[numberOfProcesses];
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<string> executionOrder;
    vector<pair<string, pair<int, int>>> timeline;

    calculateWaitingTime(processList, numberOfProcesses, waitingTime, executionOrder, timeline);
    calculateTurnAroundTime(processList, numberOfProcesses, waitingTime, turnAroundTime);

    cout << "\nProcess Execution Order:\n";
    for (string process : executionOrder)
        cout << process << " ";
    cout << endl;

    printGanttChart(timeline);

    cout << "\nProcess Name  Process ID  Arrival Time  Burst Time  Waiting Time  Turnaround Time\n";
    for (int i = 0; i < numberOfProcesses; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnAroundTime += turnAroundTime[i];

        cout << setw(12) << processList[i].processName
             << setw(12) << processList[i].processID
             << setw(14) << processList[i].arrivalTime
             << setw(12) << processList[i].burstTime
             << setw(14) << waitingTime[i]
             << setw(17) << turnAroundTime[i] << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / numberOfProcesses;
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / numberOfProcesses << endl;
}

int main() {
    int numberOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numberOfProcesses;

    Process processList[numberOfProcesses];
    for (int i = 0; i < numberOfProcesses; i++) {
        cout << "Enter Process Name, Arrival Time and Burst Time for process " << i + 1 << ": ";
        cin >> processList[i].processName >> processList[i].arrivalTime >> processList[i].burstTime;
        processList[i].processID = i + 1;
    }

    calculateAndDisplayAverageTimes(processList, numberOfProcesses);
    return 0;
}

//0 7,2 4,4 1,5 4