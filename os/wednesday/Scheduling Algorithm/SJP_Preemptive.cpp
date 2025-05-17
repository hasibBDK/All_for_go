#include <bits/stdc++.h>
using namespace std;

struct Process
{
    string processName;
    int processID;
    int burstTime;
    int remainingBurstTime;
    int arrivalTime;
    int completionTime;
    int waitingTime;
    int turnAroundTime;
};

void printGanttChart(const vector<pair<string, pair<int, int>>> &timeline)
{
    const int boxWidth = 8;

    cout << "\nGantt Chart:\n ";
    for (const auto &entry : timeline)
        cout << string(boxWidth, '-') << " ";
    cout << "\n|";

    for (const auto &entry : timeline)
    {
        string name = entry.first;
        int leftPad = (boxWidth - name.length()) / 2;
        int rightPad = boxWidth - name.length() - leftPad;
        cout << string(leftPad, ' ') << name << string(rightPad, ' ') << "|";
    }

    cout << "\n ";
    for (const auto &entry : timeline)
        cout << string(boxWidth, '-') << " ";
    cout << "\n";

    // Time labels
    cout << timeline[0].second.first;
    for (const auto &entry : timeline)
    {
        int end = entry.second.second;
        cout << setw(boxWidth + 1) << end;
    }
    cout << "\n";
}

void calculateWaitingTime(Process processList[], int numberOfProcesses,
                          vector<string> &executionOrder,
                          vector<pair<string, pair<int, int>>> &timeline)
{
    vector<bool> isCompleted(numberOfProcesses, false);
    int currentTime = 0;
    int completedProcesses = 0;

    string prevProcess = "";
    int blockStartTime = 0;

    while (completedProcesses < numberOfProcesses)
    {
        int shortestProcessIndex = -1;
        int minRemainingTime = INT_MAX;

        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (!isCompleted[i] && processList[i].arrivalTime <= currentTime &&
                processList[i].remainingBurstTime < minRemainingTime)
            {
                minRemainingTime = processList[i].remainingBurstTime;
                shortestProcessIndex = i;
            }
        }

        if (shortestProcessIndex == -1)
        {
            currentTime++;
            continue;
        }

        Process &currentProcess = processList[shortestProcessIndex];

        // Track Gantt chart segments
        if (prevProcess != currentProcess.processName)
        {
            if (!prevProcess.empty())
                timeline.push_back({prevProcess, {blockStartTime, currentTime}});
            blockStartTime = currentTime;
            prevProcess = currentProcess.processName;
        }

        executionOrder.push_back(currentProcess.processName);
        currentProcess.remainingBurstTime--;

        if (currentProcess.remainingBurstTime == 0)
        {
            isCompleted[shortestProcessIndex] = true;
            completedProcesses++;
            currentProcess.completionTime = currentTime + 1;
        }

        currentTime++;
    }

    // Add the last timeline block
    if (!prevProcess.empty())
        timeline.push_back({prevProcess, {blockStartTime, currentTime}});
}

void calculateTurnAroundTime(Process processList[], int numberOfProcesses)
{
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processList[i].turnAroundTime = processList[i].completionTime - processList[i].arrivalTime;
    }
}

void calculateWaitingTimesAndDisplayResults(Process processList[], int numberOfProcesses)
{
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<string> executionOrder;
    vector<pair<string, pair<int, int>>> timeline;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        processList[i].remainingBurstTime = processList[i].burstTime;
    }

    calculateWaitingTime(processList, numberOfProcesses, executionOrder, timeline);
    calculateTurnAroundTime(processList, numberOfProcesses);

    cout << "\nProcess Execution Order:\n";
    for (string process : executionOrder)
        cout << process << " ";
    cout << endl;

    printGanttChart(timeline);

    cout << "\nProcess Name  Process ID  Arrival Time  Burst Time  Waiting Time  Turnaround Time\n";

    for (int i = 0; i < numberOfProcesses; i++)
    {
        processList[i].waitingTime = processList[i].turnAroundTime - processList[i].burstTime;
        totalWaitingTime += processList[i].waitingTime;
        totalTurnAroundTime += processList[i].turnAroundTime;

        cout << setw(12) << processList[i].processName
             << setw(12) << processList[i].processID
             << setw(14) << processList[i].arrivalTime
             << setw(12) << processList[i].burstTime
             << setw(14) << processList[i].waitingTime
             << setw(17) << processList[i].turnAroundTime << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / numberOfProcesses;
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / numberOfProcesses << endl;
}

int main()
{
    int numberOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numberOfProcesses;

    Process processList[numberOfProcesses];

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cout << "Enter Process Name, Arrival Time, and Burst Time for process " << i + 1 << ": ";
        cin >> processList[i].processName >> processList[i].arrivalTime >> processList[i].burstTime;
        processList[i].processID = i + 1;
        processList[i].remainingBurstTime = processList[i].burstTime;
    }

    calculateWaitingTimesAndDisplayResults(processList, numberOfProcesses);
    return 0;
}


////0 7,2 4,4 1,5 4