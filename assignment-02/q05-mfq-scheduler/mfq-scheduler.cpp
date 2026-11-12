#include <iostream>                     // Include the iostream library to use I/O Functions i-e cin,cout....
#include <queue>                        // Include the queue library to use STL queues
using namespace std;                    

struct Process                          // Define a structure for each process
{
    int pid, arr_time, burst_time;      // Process ID, arrival time, and burst time
    int rem_time;                       // Remaining time for execution
    int completion_time;                // Completion time after execution
    int turnaround_time;                // Turnaround time = completion - arrival
    int wait_time;                      // Waiting time = turnaround - burst
};

Process processes[100];                 // Declare an array to hold up to 100 processes

int n;                                  // Variable to hold the number of processes

void sort_processes()                   // Function to sort processes based on arrival time
{
    for (int i = 0; i < n - 1; i++)     // Loop for bubble sort, outer loop
    {
        for (int j = 0; j < n - i - 1; j++) // Inner loop for comparing adjacent elements
        {
            if (processes[j].arr_time > processes[j + 1].arr_time) // If the earlier process arrives later
            {
                swap(processes[j], processes[j + 1]); // Swap to ensure ascending order of arrival
            }
        }
    }
}

void MFQ_execute_function()             // Function to implement the Multi-Level Feedback Queue (MFQ) algorithm
{
    queue<int> queue1, queue2, queue3;  // Define three queues for three levels
    int time = 0;                       // Variable to track current time
    int completed = 0;                  // Counter to track number of completed processes
    int index = 0;                      // Index to track which processes have arrived

    for (int i = 0; i < n; i++)         // Loop through all processes
    {
        processes[i].rem_time = processes[i].burst_time; // Initialize remaining time with burst time
    }

    while (completed < n)              // Continue until all processes are completed
    {
        while (index < n && processes[index].arr_time <= time) // If new process has arrived
        {
            queue1.push(index);        // Push the index of the process to queue1
            index++;                   // Move to the next process
        }

        if (!queue1.empty())           // If queue1 has processes
        {
            int i = queue1.front(); queue1.pop();        // Get the front process index and remove it from queue1
            int execTime = min(4, processes[i].rem_time); // Execute for max 4 units or remaining time
            time += execTime;                             // Increment current time
            processes[i].rem_time -= execTime;            // Subtract executed time from remaining

            if (processes[i].rem_time > 0)                // If process not completed
            {
                queue2.push(i);                           // Move to queue2
            } 
            else 
            {
                processes[i].completion_time = time;      // Mark completion time
                completed++;                              // Increment completed count
            }
        } 
        else if (!queue2.empty())      // If queue1 is empty and queue2 has processes
        {
            int i = queue2.front(); queue2.pop();         // Get front process from queue2
            int execTime = min(8, processes[i].rem_time); // Execute for max 8 units
            time += execTime;                             // Increment time
            processes[i].rem_time -= execTime;            // Subtract executed time

            if (processes[i].rem_time > 0)                // If process still not complete
            {
                queue3.push(i);                           // Move to queue3
            } 
            else 
            {
                processes[i].completion_time = time;      // Mark completion
                completed++;                              // Increment completed count
            }
        } 
        else if (!queue3.empty())      // If only queue3 has remaining processes
        {
            int i = queue3.front(); queue3.pop();         // Get front process from queue3
            time += processes[i].rem_time;                // Execute completely
            processes[i].rem_time = 0;                    // Remaining time is now 0
            processes[i].completion_time = time;          // Set completion time
            completed++;                                  // Increase completed count
        }
        else 
        {
            time++;                    // If no processes are ready, advance time (idle)
        }
    }

    for (int i = 0; i < n; i++)        // Calculate turnaround and waiting time for each process
    {
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arr_time; // Turnaround = Completion - Arrival
        processes[i].wait_time = processes[i].turnaround_time - processes[i].burst_time;     // Waiting = Turnaround - Burst
    }
}

void display_results()                 // Function to display turnaround and waiting times
{
    cout << "\n\n\n\t\t\t\tSTATISTICS:\n\n"; // Header
    for (int i = 0; i < n; i++)        // Loop through all processes
    {
        cout << "\n\n\t\t\t\tPROCESS " << processes[i].pid << " -> TURNAROUND TIME: " 
             << processes[i].turnaround_time << ", WAITING TIME: " << processes[i].wait_time; // Display stats
    }
    cout<<"\n\n\n\n";
}

int main()                             // Main function
{
    cout << "\n\n\n\t\t\t\tENTER THE NUMBER OF PROCESSES: 		 "; // Prompt for number of processes
    cin >> n;                          // Input the number of processes

    for (int i = 0; i < n; i++)        // Input loop for each process
    {
        cout << "\n\n\n\t\t\t\tENTER THE PROCESS ID            :	";
        cin >> processes[i].pid;       // Input process ID

        cout << "\n\n\n\t\t\t\tENTER THE PROCESS ARRIVAL TIME  :	";
        cin >> processes[i].arr_time;  // Input arrival time

        cout << "\n\n\n\t\t\t\tENTER THE PROCESS BURST TIME	   : 	";
        cin >> processes[i].burst_time; // Input burst time

        if (i < n - 1)                 // If not last process
        {
            cout << "\n\n\n\t\t\t\tENTER THE DETAILS FOR THE NEXT PROCESS........"; // Prompt for next process
        }
    }

    sort_processes();                 // Sort the processes by arrival time
    MFQ_execute_function();          // Execute MFQ scheduling
    display_results();               // Display final stats

    return 0;                        // Return success
}

