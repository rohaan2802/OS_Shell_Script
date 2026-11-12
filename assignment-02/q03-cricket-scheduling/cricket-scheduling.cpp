#include <iostream>                     // Header file for standard input and output
#include <vector>                       // Header file to use vector container
#include <algorithm>                   // Header file for algorithms like sort
#include <queue>                       // Header file to use queue data structure
#include <iomanip>                     // Header file for formatting output

using namespace std;  // Allows direct use of standard library classes and functions (like cout, cin, vector, etc.) without prefixing with 'std::'

struct Batsman                        // Structure to hold batsman details
{
    string batsman_name;              // Name of the batsman
    int matches_played;               // Total matches played by the batsman
    float strike_rate;                // Strike rate of the batsman
    float average_score;              // Average score per match
};

struct Bowler                        // Structure to hold bowler details
{
    string bowler_name;              // Name of the bowler
    int matches_played;              // Total matches played by the bowler
    float economy_rate;              // Economy rate of the bowler
    int wickets;                     // Total wickets taken
    float average;                   // Bowling average
};

// Function to sort batsmen using SJF (Shortest Job First) — based on higher strike rate
bool compare_batsmen_SJF(Batsman a, Batsman b) 
{
    return a.strike_rate > b.strike_rate;     // Batsman with higher strike rate comes first
}

// Function to sort bowlers using SJF — based on lower economy rate
bool compare_bowlers_SJF(Bowler a, Bowler b) 
{
    return a.economy_rate < b.economy_rate;   // Bowler with lower economy rate comes first
}

// Function to sort batsmen using SRTF (Shortest Remaining Time First) — based on higher average score
bool compare_batsmen_SRTF(Batsman a, Batsman b) 
{
    return a.average_score > b.average_score; // Batsman with higher average score comes first
}

// Function to sort bowlers using SRTF — based on more wickets taken
bool compare_bowlers_SRTF(Bowler a, Bowler b) 
{
    return a.wickets > b.wickets;             // Bowler with more wickets comes first
}

// Function to simulate Round Robin bowling rotation
void round_robin_bowling(vector<Bowler> &bowlers, int totalOvers) 
{
    queue<Bowler> bowlerQueue;                        // Queue to hold bowlers for rotation
    for (auto &bowler : bowlers)                      // Push each bowler into the queue
    {
        bowlerQueue.push(bowler);                     
    }

    cout << "\n\n\n\t\t\t\tBOWLING ORDER (ROUND ROBIN):";     // Print header for bowling order
    for (int i = 0; i < totalOvers; i++)              // Loop through each over
    {
        Bowler current = bowlerQueue.front();         // Get the bowler at the front of the queue
        bowlerQueue.pop();                            // Remove the front bowler from queue
        cout << "\n\n\n\t\t\t\tOVER " << i + 1 << ": " << current.bowler_name;  // Display the current over number and the bowler's name
        bowlerQueue.push(current);                    // Add the bowler back to the end of the queue
    }
}

// Function to display predicted batting order and total score
void display_batting_report(vector<Batsman> &batsmen)
{
    cout << "\n\n\n\t\t\t\tOPTIMAL BATTING ORDER:\n";   // Header for batting order
    float predictedScore = 0;                          // Variable to hold predicted score

    for (auto &batsman : batsmen)                      // Loop through sorted batsmen
    {
        cout << "\n\n\n\t\t\t\t" << batsman.batsman_name << " - STRIKE RATE: " << batsman.strike_rate << " - AVG SCORE: " << batsman.average_score; // Print batsman info
        predictedScore += batsman.average_score;       // Sum average scores
    }
    cout << "\n\n\n\t\t\t\tPREDICTED TEAM SCORE: " << fixed << setprecision(2) << predictedScore << " RUNS\n\n\n\n"; // Display predicted score
}

// Function to display bowling report with predicted total wickets
void display_bowling_report(vector<Bowler> &bowlers)
{
    float predictedWickets = 0;                        // Variable for total predicted wickets
    for (auto &bowler : bowlers)                       // Loop through each bowler
    {
        predictedWickets += (float)bowler.wickets / bowler.matches_played; // Avg wickets per match
    }
    cout << "\n\n\n\t\t\t\tPREDICTED TOTAL WICKETS TAKEN: " << fixed << setprecision(2) << predictedWickets << endl; // Display total
}

int main() 
{
    int num_Batsmen, num_Bowlers;                     // Variables to store number of batsmen and bowlers
    cout << "\n\n\n\t\t\t\tENTER THE NUMBER OF BATSMEN: ";
    cin >> num_Batsmen;                               // Input number of batsmen
    cout << "\n\n\n\t\t\t\tENTER THE NUMBER OF BOWLERS: ";
    cin >> num_Bowlers;                               // Input number of bowlers

    vector<Batsman> batsmen(num_Batsmen);             // Vector of batsmen
    vector<Bowler> bowlers(num_Bowlers);              // Vector of bowlers

// Input batsmen
for (int i = 0; i < num_Batsmen; i++) // Loop through each batsman
{
    cout << "\n\n\n\t\t\t\tENTER THE BATSMAN_NAME FOR BATSMAN " << i + 1 << "\t\t"; 
    // Prompt user to enter batsman name
    cin >> batsmen[i].batsman_name; // Read the batsman's name into the batsmen array
    
    cout << "\n\n\n\t\t\t\tENTER THE MATCHES_PLAYED FOR BATSMAN " << i + 1 << "\t\t"; 
    // Prompt user to enter number of matches played by the batsman
    cin >> batsmen[i].matches_played; // Read the number of matches played into the batsmen array
    
    cout << "\n\n\n\t\t\t\tENTER THE STRIKE_RATE FOR BATSMAN " << i + 1 << "\t\t"; 
    // Prompt user to enter the strike rate of the batsman
    cin >> batsmen[i].strike_rate; // Read the batsman's strike rate into the batsmen array
    
    cout << "\n\n\n\t\t\t\tENTER THE AVGSCORE FOR BATSMAN " << i + 1 << "\t\t"; 
    // Prompt user to enter the average score of the batsman
    cin >> batsmen[i].average_score; // Read the batsman's average score into the batsmen array
}


  // Input bowlers
for (int i = 0; i < num_Bowlers; i++) // Loop through each bowler
{
    cout << "\n\n\n\t\t\t\tENTER THE BOWLER NAME FOR BOWLER " << i + 1 << "\t\t"; 
    // Prompt user to enter the name of the bowler
    cin >> bowlers[i].bowler_name; // Read the bowler's name into the bowlers array
    
    cout << "\n\n\n\t\t\t\tENTER THE MATCHES PLAYED FOR BOWLER " << i + 1 << "\t\t"; 
    // Prompt user to enter the number of matches the bowler has played
    cin >> bowlers[i].matches_played; // Read the number of matches into the bowlers array
    
    cout << "\n\n\n\t\t\t\tENTER THE ECONOMY RATE FOR BOWLER " << i + 1 << "\t\t"; 
    // Prompt user to enter the economy rate of the bowler
    cin >> bowlers[i].economy_rate; // Read the economy rate into the bowlers array
    
    cout << "\n\n\n\t\t\t\tENTER THE WICKETS TAKEN BY BOWLER " << i + 1 << "\t\t"; 
    // Prompt user to enter the number of wickets taken by the bowler
    cin >> bowlers[i].wickets; // Read the number of wickets taken into the bowlers array
    
    cout << "\n\n\n\t\t\t\tENTER THE BOWLING AVERAGE FOR BOWLER " << i + 1 << "\t\t"; 
    // Prompt user to enter the bowling average of the bowler
    cin >> bowlers[i].average; // Read the bowling average into the bowlers array
}


    char choice;                                       // Variable to store team choice
    int algoChoice;                                    // Variable to store algorithm choice

    cout << "\n\n\n\t\t\t\tARE YOU BATTING (B) OR BOWLING (O)? ";
    cin >> choice;                                     // Take choice input

    if (choice == 'B' || choice == 'b')                // If team is batting
    {
        cout << "\n\n\n\t\t\t\tSELECT SCHEDULING ALGORITHM:\n\n\n\t\t\t\t1. SJF\n\n\n\t\t\t\t2. SRTF\n\n\n\t\t\t\t";
        cin >> algoChoice;                             // Choose SJF or SRTF

        if (algoChoice == 1)
        {
            sort(batsmen.begin(), batsmen.end(), compare_batsmen_SJF);  // Sort batsmen using SJF
        }
        else if (algoChoice == 2)
        {
            sort(batsmen.begin(), batsmen.end(), compare_batsmen_SRTF); // Sort batsmen using SRTF
        }

        display_batting_report(batsmen);                 // Call function to display report
    } 
    else if (choice == 'O' || choice == 'o')           // If team is bowling
    {
        cout << "\n\n\n\t\t\t\tSELECT SCHEDULING ALGORITHM:\n\n\n\t\t\t\t1. SJF\n\n\n\t\t\t\t2. SRTF\n\n\n\t\t\t\t3. ROUND ROBIN\n\n\n\t\t\t\t";
        cin >> algoChoice;                             // Get bowler strategy

        if (algoChoice == 1)
        {
            sort(bowlers.begin(), bowlers.end(), compare_bowlers_SJF);  // SJF sorting
            for (auto &b : bowlers) cout << "\n\n\n\t\t\t\t" << b.bowler_name << " - ECONOMY: " << b.economy_rate; // Display
            display_bowling_report(bowlers);            // Show predicted wickets
        }
        else if (algoChoice == 2)
        {
            sort(bowlers.begin(), bowlers.end(), compare_bowlers_SRTF); // SRTF sorting
            for (auto &b : bowlers) cout << "\n\n\n\t\t\t\t" << b.bowler_name << " - WICKETS: " << b.wickets; // Show sorted
            display_bowling_report(bowlers);            // Show predicted wickets
        }
        else if (algoChoice == 3)
        {
            int totalOvers;                            // Total overs for round robin
            cout << "\n\n\n\t\t\t\tENTER TOTAL OVERS TO BE BOWLED:           ";
            cin >> totalOvers;                         // Input overs
            round_robin_bowling(bowlers, totalOvers);    // Round robin simulation
            display_bowling_report(bowlers);             // Predicted wickets
        }
    }
    else
    {
    cout<<"\n\n\n\t\t\t\tWRONG INPUT ---> PROGRAM EXITING........\n\n\n";
    exit(1);
    }

    return 0; // End of program
}
