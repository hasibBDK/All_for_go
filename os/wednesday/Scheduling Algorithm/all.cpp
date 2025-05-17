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
    int completionTime;
    int startTime;
};

// Common Gantt Chart printer
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

// FCFS Scheduling
void fcfsScheduling(vector<Process> &processList) {
    sort(processList.begin(), processList.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<pair<string, pair<int, int>>> ganttChart;

    cout << "\nProcess Execution Order:\n";
    for (auto &p : processList) {
        cout << p.processName << " ";
    }
    cout << "\n";

    cout << "\nProcess Name\tProcess ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";

    for (auto &p : processList) {
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;

        int startTime = currentTime;
        int waitingTime = currentTime - p.arrivalTime;
        int turnAroundTime = waitingTime + p.burstTime;

        totalWaitingTime += waitingTime;
        totalTurnAroundTime += turnAroundTime;

        ganttChart.push_back({p.processName, {startTime, startTime + p.burstTime}});

        cout << "    " << left << p.processName 
             << "  \t\t" << p.processID
             << "  \t\t" << p.arrivalTime
             << "  \t\t" << p.burstTime
             << "  \t\t" << waitingTime << "  \t\t"
             << turnAroundTime << endl;

        currentTime += p.burstTime;
    }

    cout << "\nGantt Chart:\n";
    printGanttChart(ganttChart);

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}

// Priority Scheduling (Non-preemptive)
void prioritySchedulingNonPreemptive(vector<Process> &processList) {
    sort(processList.begin(), processList.end(), [](Process a, Process b) {
        return a.priority < b.priority;
    });

    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<pair<string, pair<int, int>>> timeline;

    cout << "\nProcess Execution Order:\n";
    for (auto &p : processList) {
        cout << p.processName << " ";
    }
    cout << endl;

    cout << "\nProcess Name\tProcess ID\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n";

    for (auto &p : processList) {
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;

        p.waitingTime = currentTime - p.arrivalTime;
        p.turnAroundTime = p.waitingTime + p.burstTime;

        timeline.push_back({p.processName, {currentTime, currentTime + p.burstTime}});

        currentTime += p.burstTime;
        totalWaitingTime += p.waitingTime;
        totalTurnAroundTime += p.turnAroundTime;

        cout << "    " << left << p.processName
             << "  \t\t" << p.processID
             << "  \t\t" << p.burstTime
             << "  \t\t" << p.priority
             << "  \t\t" << p.waitingTime << "  \t\t"
             << p.turnAroundTime << endl;
    }

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}

// Priority Scheduling (Preemptive)
void prioritySchedulingPreemptive(vector<Process> &processList) {
    sort(processList.begin(), processList.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    int completedProcesses = 0;
    vector<bool> isCompleted(processList.size(), false);
    vector<pair<string, pair<int, int>>> timeline;
    vector<int> remainingBurstTime(processList.size());
    vector<int> waitingTime(processList.size(), 0);
    vector<int> turnAroundTime(processList.size(), 0);

    for (int i = 0; i < processList.size(); i++) {
        remainingBurstTime[i] = processList[i].burstTime;
    }

    int prevProcessIndex = -1;

    while (completedProcesses < processList.size()) {
        int highestPriorityIndex = -1;
        int highestPriority = INT_MAX;

        for (int i = 0; i < processList.size(); i++) {
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
            if (remainingBurstTime[highestPriorityIndex] == processList[highestPriorityIndex].burstTime) {
                processList[highestPriorityIndex].startTime = currentTime;
            }

            // Gantt Chart: Track timeline if process changes
            if (prevProcessIndex != highestPriorityIndex) {
                timeline.push_back({processList[highestPriorityIndex].processName, {currentTime, currentTime + 1}});
            } else {
                timeline.back().second.second++;  // Extend current process block
            }

            remainingBurstTime[highestPriorityIndex]--;
            currentTime++;
            prevProcessIndex = highestPriorityIndex;

            if (remainingBurstTime[highestPriorityIndex] == 0) {
                isCompleted[highestPriorityIndex] = true;
                completedProcesses++;
                processList[highestPriorityIndex].completionTime = currentTime;
                turnAroundTime[highestPriorityIndex] = currentTime - processList[highestPriorityIndex].arrivalTime;
                waitingTime[highestPriorityIndex] = turnAroundTime[highestPriorityIndex] - processList[highestPriorityIndex].burstTime;

                totalWaitingTime += waitingTime[highestPriorityIndex];
                totalTurnAroundTime += turnAroundTime[highestPriorityIndex];
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

    cout << "\nProcess Name\tProcess ID\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < processList.size(); i++) {
        cout << "   " << left << processList[i].processName
             << "  \t\t" << processList[i].processID
             << "  \t\t" << processList[i].arrivalTime
             << "  \t\t" << processList[i].burstTime
             << "  \t\t" << processList[i].priority
             << "  \t\t" << waitingTime[i] << "  \t\t"
             << turnAroundTime[i] << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}

// Round Robin Scheduling
void roundRobinScheduling(vector<Process> &processList, int timeQuantum) {
    queue<int> processQueue;
    vector<int> remainingBurstTime(processList.size());
    vector<int> waitingTime(processList.size(), 0);
    vector<int> turnAroundTime(processList.size(), 0);
    vector<pair<string, pair<int, int>>> timeline;

    int currentTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<string> executionOrder;

    for (int i = 0; i < processList.size(); i++) {
        remainingBurstTime[i] = processList[i].burstTime;
        if (processList[i].arrivalTime <= currentTime) {
            processQueue.push(i);
        }
    }

    cout << "\nRound Robin Scheduling:\n";

    while (!processQueue.empty()) {
        int currentIndex = processQueue.front();
        processQueue.pop();

        executionOrder.push_back(processList[currentIndex].processName);

        int executionTime = min(timeQuantum, remainingBurstTime[currentIndex]);
        timeline.push_back({processList[currentIndex].processName, {currentTime, currentTime + executionTime}});
        currentTime += executionTime;
        remainingBurstTime[currentIndex] -= executionTime;

        // Check for newly arrived processes
        for (int i = 0; i < processList.size(); i++) {
            if (remainingBurstTime[i] > 0 && processList[i].arrivalTime <= currentTime && 
                find(executionOrder.begin(), executionOrder.end(), processList[i].processName) == executionOrder.end() && 
                i != currentIndex) {
                processQueue.push(i);
            }
        }

        if (remainingBurstTime[currentIndex] > 0) {
            processQueue.push(currentIndex);
        } else {
            turnAroundTime[currentIndex] = currentTime - processList[currentIndex].arrivalTime;
            waitingTime[currentIndex] = turnAroundTime[currentIndex] - processList[currentIndex].burstTime;
            totalWaitingTime += waitingTime[currentIndex];
            totalTurnAroundTime += turnAroundTime[currentIndex];
        }

        // If queue is empty but there are still processes to arrive
        if (processQueue.empty()) {
            for (int i = 0; i < processList.size(); i++) {
                if (remainingBurstTime[i] > 0) {
                    currentTime = processList[i].arrivalTime;
                    processQueue.push(i);
                    break;
                }
            }
        }
    }

    cout << "\nProcess Execution Order:\n";
    for (const auto &p : executionOrder)
        cout << p << " ";
    cout << endl;

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nProcess Name\tProcess ID\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < processList.size(); i++) {
        cout << "   " << left << processList[i].processName
             << "  \t\t" << processList[i].processID
             << "  \t\t" << processList[i].burstTime
             << "  \t\t" << waitingTime[i]
             << "  \t\t" << turnAroundTime[i] << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}

// SJF (Non-preemptive)
void sjfNonPreemptive(vector<Process> &processList) {
    sort(processList.begin(), processList.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<pair<string, pair<int, int>>> timeline;
    vector<bool> isCompleted(processList.size(), false);
    vector<string> executionOrder;

    cout << "\nSJF (Non-preemptive) Scheduling:\n";

    int completedProcesses = 0;
    while (completedProcesses < processList.size()) {
        int shortestIndex = -1;
        int shortestBurst = INT_MAX;

        for (int i = 0; i < processList.size(); i++) {
            if (!isCompleted[i] && processList[i].arrivalTime <= currentTime && 
                processList[i].burstTime < shortestBurst) {
                shortestBurst = processList[i].burstTime;
                shortestIndex = i;
            }
        }

        if (shortestIndex == -1) {
            currentTime++;
            continue;
        }

        Process &p = processList[shortestIndex];
        executionOrder.push_back(p.processName);

        int startTime = currentTime;
        int waitingTime = currentTime - p.arrivalTime;
        int turnAroundTime = waitingTime + p.burstTime;

        totalWaitingTime += waitingTime;
        totalTurnAroundTime += turnAroundTime;

        timeline.push_back({p.processName, {startTime, startTime + p.burstTime}});
        currentTime += p.burstTime;
        isCompleted[shortestIndex] = true;
        completedProcesses++;
    }

    cout << "\nProcess Execution Order:\n";
    for (const auto &p : executionOrder)
        cout << p << " ";
    cout << endl;

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nProcess Name\tProcess ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < processList.size(); i++) {
        cout << "    " << left << processList[i].processName
             << "  \t\t" << processList[i].processID
             << "  \t\t" << processList[i].arrivalTime
             << "  \t\t" << processList[i].burstTime
             << "  \t\t" << (timeline[i].second.first - processList[i].arrivalTime)
             << "  \t\t" << (timeline[i].second.second - processList[i].arrivalTime) << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}

// SRTF (Preemptive SJF)
void srtfScheduling(vector<Process> &processList) {
    sort(processList.begin(), processList.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0, totalWaitingTime = 0, totalTurnAroundTime = 0;
    int completedProcesses = 0;
    vector<bool> isCompleted(processList.size(), false);
    vector<int> remainingBurstTime(processList.size());
    vector<int> waitingTime(processList.size(), 0);
    vector<int> turnAroundTime(processList.size(), 0);
    vector<pair<string, pair<int, int>>> timeline;
    vector<string> executionOrder;

    for (int i = 0; i < processList.size(); i++) {
        remainingBurstTime[i] = processList[i].burstTime;
    }

    string prevProcess = "";
    int blockStartTime = 0;

    cout << "\nSRTF (Preemptive SJF) Scheduling:\n";

    while (completedProcesses < processList.size()) {
        int shortestIndex = -1;
        int shortestRemaining = INT_MAX;

        for (int i = 0; i < processList.size(); i++) {
            if (!isCompleted[i] && processList[i].arrivalTime <= currentTime && 
                remainingBurstTime[i] < shortestRemaining) {
                shortestRemaining = remainingBurstTime[i];
                shortestIndex = i;
            }
        }

        if (shortestIndex == -1) {
            currentTime++;
            continue;
        }

        Process &currentProcess = processList[shortestIndex];
        executionOrder.push_back(currentProcess.processName);

        // Track Gantt chart segments
        if (prevProcess != currentProcess.processName) {
            if (!prevProcess.empty())
                timeline.push_back({prevProcess, {blockStartTime, currentTime}});
            blockStartTime = currentTime;
            prevProcess = currentProcess.processName;
        }

        remainingBurstTime[shortestIndex]--;
        currentTime++;

        if (remainingBurstTime[shortestIndex] == 0) {
            isCompleted[shortestIndex] = true;
            completedProcesses++;
            turnAroundTime[shortestIndex] = currentTime - currentProcess.arrivalTime;
            waitingTime[shortestIndex] = turnAroundTime[shortestIndex] - currentProcess.burstTime;
            totalWaitingTime += waitingTime[shortestIndex];
            totalTurnAroundTime += turnAroundTime[shortestIndex];
        }
    }

    // Add the last timeline block
    if (!prevProcess.empty())
        timeline.push_back({prevProcess, {blockStartTime, currentTime}});

    cout << "\nProcess Execution Order:\n";
    for (const auto &p : executionOrder)
        cout << p << " ";
    cout << endl;

    cout << "\nGantt Chart:\n";
    printGanttChart(timeline);

    cout << "\nProcess Name\tProcess ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < processList.size(); i++) {
        cout << "    " << left << processList[i].processName
             << "   \t\t" << processList[i].processID
             << "   \t\t" << processList[i].arrivalTime
             << "   \t\t" << processList[i].burstTime
             << "   \t\t" << waitingTime[i]
             << "   \t\t" << turnAroundTime[i] << endl;
    }

    cout << "\nTotal Waiting Time: " << totalWaitingTime;
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (float)totalWaitingTime / processList.size();
    cout << "\nTotal Turnaround Time: " << totalTurnAroundTime;
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (float)totalTurnAroundTime / processList.size() << endl;
}


void displayAllProcesses(const vector<Process>& processList) {
    if (processList.empty()) {
        cout << "No processes available.\n";
        return;
    }

    cout << "\nAll Processes:\n";
    cout << "ID\tName\tArrival\tBurst\tPriority\n";
    cout << "--------------------------------\n";
    for (const auto& p : processList) {
        cout << p.processID << "\t " << p.processName << "\t  " 
             << p.arrivalTime << "\t " << p.burstTime << "\t";
        if (p.priority != 0) {
            cout << "  " << p.priority;
        } else {
            cout << "N/A";
        }
        cout << endl;
    }
}

void addMoreProcesses(vector<Process>& processList) {
    int additionalProcesses;
    cout << "How many additional processes do you want to add? ";
    cin >> additionalProcesses;

    int startID = processList.empty() ? 1 : processList.back().processID + 1;
    
    for (int i = 0; i < additionalProcesses; i++) {
        Process p;
        cout << "Enter details for process " << startID + i << ":\n";
        cout << "Process Name: ";
        cin >> p.processName;
        cout << "Arrival Time: ";
        cin >> p.arrivalTime;
        cout << "Burst Time: ";
        cin >> p.burstTime;
        cout << "Priority (0 if not needed): ";
        cin >> p.priority;
        
        p.processID = startID + i;
        processList.push_back(p);
    }
}

void getInitialProcessInput(vector<Process> &processList) {
    int numberOfProcesses;
    cout << "Enter the number of processes: ";
    cin >> numberOfProcesses;

    processList.resize(numberOfProcesses);
    for (int i = 0; i < numberOfProcesses; i++) {
        cout << "Enter Process Name, Arrival Time, Burst Time, and Priority for process " << i + 1 << ": ";
        cin >> processList[i].processName >> processList[i].arrivalTime >> processList[i].burstTime >> processList[i].priority;
        processList[i].processID = i + 1;
    }
}

void displayMenu() {
    cout << "\nCPU Scheduling Algorithms Simulator\n";
    cout << "1. FCFS (First Come First Serve)\n";
    cout << "2. Priority Scheduling (Non-preemptive)\n";
    cout << "3. Priority Scheduling (Preemptive)\n";
    cout << "4. Round Robin\n";
    cout << "5. SJF (Non-preemptive)\n";
    cout << "6. SRTF (Preemptive SJF)\n";
    cout << "7. Add more processes\n";
    cout << "8. Show all processes\n";
    cout << "9. Exit\n";
    cout << "Enter your choice (1-9): ";
}

int main() {
    vector<Process> processList;
    int timeQuantum;
    bool exitProgram = false;

    // Get initial process inputs
    cout << "Initial Process Setup\n";
    cout << "---------------------\n";
    getInitialProcessInput(processList);

    while (!exitProgram) {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: // FCFS - Uses all processes (name, burst time), arrival time defaults to 0
                fcfsScheduling(processList);
                break;
                
            case 2: // Priority Non-preemptive - Uses name, burst time, priority
                prioritySchedulingNonPreemptive(processList);
                break;
                
            case 3: // Priority Preemptive - Uses name, arrival time, burst time, priority
                prioritySchedulingPreemptive(processList);
                break;
                
            case 4: // Round Robin - Uses name, burst time, asks for time quantum
                cout << "Enter the time quantum: ";
                cin >> timeQuantum;
                roundRobinScheduling(processList, timeQuantum);
                break;
                
            case 5: // SJF Non-preemptive - Uses name, arrival time, burst time
                sjfNonPreemptive(processList);
                break;
                
            case 6: // SRTF - Uses name, arrival time, burst time
                srtfScheduling(processList);
                break;
                
            case 7: // Add more processes
                addMoreProcesses(processList);
                break;
                
            case 8: // Show all processes
                displayAllProcesses(processList);
                break;
                
            case 9: // Exit
                exitProgram = true;
                cout << "Exiting program...\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }

        if (!exitProgram && choice != 8) { // Don't pause after showing processes
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    return 0;
}