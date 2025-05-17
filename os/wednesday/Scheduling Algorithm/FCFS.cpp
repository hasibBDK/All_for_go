#include <bits/stdc++.h>
using namespace std;

struct Process {
    string processName;
    int processID, arrivalTime, burstTime;
};

// Function to display the Gantt chart
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

    // Time labels: align exactly below the vertical borders (|) of the blocks
    int currentTime = timeline[0].second.first;
    cout << setw(2) << currentTime; // First time at the very start

    for (const auto &entry : timeline) {
        int endTime = entry.second.second;
        string endStr = to_string(endTime);
        // Each box ends after boxWidth characters, so align to that
        int space = boxWidth - endStr.length();
        cout << string(space + 1, ' ') << endStr;
    }
    cout << "\n";
}


void fcfsScheduling(vector<Process> &processList) {
    sort(processList.begin(), processList.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<pair<string, pair<int, int>>> ganttChart; // {processName, {start, end}}

    cout << "\nProcess Execution Order:\n";
    for (auto &p : processList) {
        cout << p.processName << " ---> ";
    }
    cout << "End\n";

    cout << "\nProcess  ID  BurstTime  WaitingTime  TurnaroundTime\n";

    for (auto &p : processList) {
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;

        int startTime = currentTime;
        int waitingTime = currentTime - p.arrivalTime;
        int turnAroundTime = waitingTime + p.burstTime;

        totalWaitingTime += waitingTime;
        totalTurnAroundTime += turnAroundTime;

        ganttChart.push_back({p.processName, {startTime, startTime + p.burstTime}});

        cout << "    " << setw(6) << left << p.processName
             << setw(8) << p.processID
             << setw(12) << p.burstTime
             << setw(14) << waitingTime
             << turnAroundTime << endl;

        currentTime += p.burstTime;
    }

    cout << endl << endl << "Ghant Chart : " << endl << endl;
    printGanttChart(ganttChart);

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
        cout << "Enter Process Name, Arrival Time, and Burst Time for process " << i + 1 << ": ";
        cin >> processList[i].processName >> processList[i].arrivalTime >> processList[i].burstTime;
        processList[i].processID = i + 1;
    }

    cout << endl;
    fcfsScheduling(processList);
    return 0;
}