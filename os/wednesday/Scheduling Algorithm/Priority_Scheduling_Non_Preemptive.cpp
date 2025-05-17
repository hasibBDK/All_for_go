#include <bits/stdc++.h>
using namespace std;

struct Process {
    string processName;
    int processID;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnAroundTime;
};

void printGanttChart(const vector<pair<string, pair<int, int>>> &timeline) {
    const int boxWidth = 20;

    // Top border
    cout << " ";
    for (const auto &entry : timeline)
        cout << string(boxWidth, '-') << " ";
    cout << "\n|";

    // Process names centered
    for (const auto &entry : timeline) {
        string name = entry.first;
        int leftPadding = (boxWidth - name.length()) / 2;
        int rightPadding = boxWidth - name.length() - leftPadding;
        cout << string(leftPadding, ' ') << name << string(rightPadding, ' ') << "|";
    }
    cout << "\n ";

    // Bottom border
    for (const auto &entry : timeline)
        cout << string(boxWidth, '-') << " ";
    cout << "\n";

    // Time labels aligned under '|'
    int currentTime = timeline[0].second.first;
    cout << setw(2) << currentTime;

    for (const auto &entry : timeline) {
        int endTime = entry.second.second;
        string endStr = to_string(endTime);
        int space = boxWidth - endStr.length();
        cout << string(space + 1, ' ') << endStr;
    }
    cout << "\n";
}

void priorityScheduling(Process processList[], int numberOfProcesses) {
    sort(processList, processList + numberOfProcesses, [](Process a, Process b) {
        return a.priority < b.priority;
    });

    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnAroundTime = 0;

    vector<pair<string, pair<int, int>>> timeline;

    for (int i = 0; i < numberOfProcesses; i++) {
        if (currentTime < processList[i].arrivalTime)
            currentTime = processList[i].arrivalTime;

        processList[i].waitingTime = currentTime - processList[i].arrivalTime;
        processList[i].turnAroundTime = processList[i].waitingTime + processList[i].burstTime;

        timeline.push_back({processList[i].processName, {currentTime, currentTime + processList[i].burstTime}});

        currentTime += processList[i].burstTime;
        totalWaitingTime += processList[i].waitingTime;
        totalTurnAroundTime += processList[i].turnAroundTime;
    }

    cout << "\nProcess Execution Order:\n";
    for (int i = 0; i < numberOfProcesses; i++) {
        cout << processList[i].processName << " ";
    }
    cout << endl;

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nProcess Name  Process ID  Burst Time  Waiting Time  Turnaround Time\n";
    for (int i = 0; i < numberOfProcesses; i++) {
        cout << " " << processList[i].processName << "\t\t"
             << processList[i].processID << "\t\t"
             << processList[i].burstTime << "\t\t"
             << processList[i].waitingTime << "\t\t"
             << processList[i].turnAroundTime << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / numberOfProcesses;
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / numberOfProcesses << endl;
}

int main() {
    int numberOfProcesses;
    cout << "Enter number of processes: ";
    cin >> numberOfProcesses;

    Process processList[numberOfProcesses];

    for (int i = 0; i < numberOfProcesses; i++) {
        cout << "Enter Process Name, Arrival Time, Burst Time, and Priority for process " << i + 1 << ": ";
        cin >> processList[i].processName >> processList[i].arrivalTime >> processList[i].burstTime >> processList[i].priority;
        processList[i].processID = i + 1;
    }

    priorityScheduling(processList, numberOfProcesses);
    return 0;
}
//10 3,1 1, 2 4,1 5, 5 2