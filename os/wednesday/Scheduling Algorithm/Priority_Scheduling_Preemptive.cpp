#include <bits/stdc++.h>
using namespace std;

struct Process {
    string processName;
    int processID;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingBurstTime;
    int waitingTime;
    int turnAroundTime;
    int startTime;
    int completionTime;
};

// Gantt Chart Printer
void printGanttChart(const vector<pair<string, pair<int, int>>> &timeline) {
    const int boxWidth = 8;

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

    // Time labels
    int prevTime = timeline[0].second.first;
    cout << setw(2) << prevTime;
    for (const auto &entry : timeline) {
        int endTime = entry.second.second;
        int gap = boxWidth - to_string(endTime).length();
        cout << string(gap + 1, ' ') << endTime;
    }
    cout << "\n";
}

void priorityScheduling(Process processList[], int numberOfProcesses) {
    // Sort initially by arrival
    sort(processList, processList + numberOfProcesses, [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    int completedProcesses = 0;
    vector<bool> isCompleted(numberOfProcesses, false);
    vector<int> executionOrder;
    vector<pair<string, pair<int, int>>> timeline;

    int prevProcessIndex = -1;

    while (completedProcesses < numberOfProcesses) {
        int highestPriorityIndex = -1;
        int highestPriority = INT_MAX;

        for (int i = 0; i < numberOfProcesses; i++) {
            if (!isCompleted[i] && processList[i].arrivalTime <= currentTime) {
                if (processList[i].priority < highestPriority) {
                    highestPriority = processList[i].priority;
                    highestPriorityIndex = i;
                } else if (processList[i].priority == highestPriority) {
                    if (processList[i].arrivalTime < processList[highestPriorityIndex].arrivalTime) {
                        highestPriorityIndex = i;
                    }
                }
            }
        }

        if (highestPriorityIndex != -1) {
            if (processList[highestPriorityIndex].remainingBurstTime == processList[highestPriorityIndex].burstTime) {
                processList[highestPriorityIndex].startTime = currentTime;
            }

            // Gantt Chart: Track timeline if process changes
            if (prevProcessIndex != highestPriorityIndex) {
                timeline.push_back({processList[highestPriorityIndex].processName, {currentTime, currentTime + 1}});
            } else {
                timeline.back().second.second++;  // Extend current process block
            }

            processList[highestPriorityIndex].remainingBurstTime--;
            currentTime++;
            prevProcessIndex = highestPriorityIndex;

            if (processList[highestPriorityIndex].remainingBurstTime == 0) {
                isCompleted[highestPriorityIndex] = true;
                completedProcesses++;
                processList[highestPriorityIndex].completionTime = currentTime;
                processList[highestPriorityIndex].turnAroundTime = currentTime - processList[highestPriorityIndex].arrivalTime;
                processList[highestPriorityIndex].waitingTime = processList[highestPriorityIndex].turnAroundTime - processList[highestPriorityIndex].burstTime;

                totalWaitingTime += processList[highestPriorityIndex].waitingTime;
                totalTurnAroundTime += processList[highestPriorityIndex].turnAroundTime;
            }
        } else {
            currentTime++; // CPU is idle
            prevProcessIndex = -1;
        }
    }

    cout << "\nProcess Execution Order:\n";
    for (const auto &entry : timeline)
        cout << entry.first << " ";
    cout << "\n";

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nProcess Name  Process ID  Arrival Time  Burst Time  Priority  Waiting Time  Turnaround Time\n";
    for (int i = 0; i < numberOfProcesses; i++) {
        cout << " " << processList[i].processName << "\t\t"
             << processList[i].processID << "\t\t"
             << processList[i].arrivalTime << "\t\t"
             << processList[i].burstTime << "\t\t"
             << processList[i].priority << "\t\t"
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
        processList[i].remainingBurstTime = processList[i].burstTime;
        processList[i].waitingTime = 0;
        processList[i].turnAroundTime = 0;
        processList[i].startTime = -1;
        processList[i].completionTime = 0;
    }

    priorityScheduling(processList, numberOfProcesses);

    return 0;
}
