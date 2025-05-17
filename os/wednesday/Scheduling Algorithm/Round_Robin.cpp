#include <bits/stdc++.h>
using namespace std;

struct Process {
    string processName;
    int processID, arrivalTime, burstTime;
};

// Gantt Chart printer
void printGanttChart(const vector<pair<string, pair<int, int>>> &timeline) {
    const int boxWidth = 8;

    // Top border
    cout << " ";
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

// Round Robin Scheduling with Gantt Chart
void roundRobinScheduling(vector<Process> processList, int timeQuantum) {
    queue<Process> processQueue;
    vector<int> remainingBurstTime(processList.size());
    vector<int> waitingTime(processList.size(), 0);
    vector<int> turnAroundTime(processList.size(), 0);
    vector<pair<string, pair<int, int>>> timeline;

    int currentTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<string> executionOrder;

    for (int i = 0; i < processList.size(); i++) {
        remainingBurstTime[i] = processList[i].burstTime;
        processQueue.push(processList[i]);
    }

    cout << "\nRound Robin Scheduling:\n";

    while (!processQueue.empty()) {
        Process currentProcess = processQueue.front();
        processQueue.pop();
        int index = currentProcess.processID - 1;

        executionOrder.push_back(currentProcess.processName);

        int executionTime = min(timeQuantum, remainingBurstTime[index]);
        timeline.push_back({currentProcess.processName, {currentTime, currentTime + executionTime}});
        currentTime += executionTime;
        remainingBurstTime[index] -= executionTime;

        if (remainingBurstTime[index] > 0) {
            processQueue.push(currentProcess);
        } else {
            waitingTime[index] = currentTime - processList[index].arrivalTime - processList[index].burstTime;
            turnAroundTime[index] = currentTime - processList[index].arrivalTime;
            totalWaitingTime += waitingTime[index];
            totalTurnAroundTime += turnAroundTime[index];
        }
    }

    cout << "\nProcess Execution Order:\n";
    for (const auto &p : executionOrder)
        cout << p << " ";
    cout << endl;

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nProcess Name  Process ID  Burst Time  Waiting Time  Turnaround Time\n";
    for (int i = 0; i < processList.size(); i++) {
        cout << " " << processList[i].processName << "\t\t"
             << processList[i].processID << "\t\t"
             << processList[i].burstTime << "\t\t"
             << waitingTime[i] << "\t\t"
             << turnAroundTime[i] << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}

int main() {
    int numberOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numberOfProcesses;
    vector<Process> processList(numberOfProcesses);

    for (int i = 0; i < numberOfProcesses; i++) {
        cout << "Enter Process Name and Burst Time for process " << i + 1 << ": ";
        cin >> processList[i].processName >> processList[i].burstTime;
        processList[i].processID = i + 1;
        processList[i].arrivalTime = 0;
    }

    int timeQuantum;
    cout << "Enter the time quantum: ";
    cin >> timeQuantum;

    cout << endl;
    roundRobinScheduling(processList, timeQuantum);
    return 0;
}
//53 17 68 24
