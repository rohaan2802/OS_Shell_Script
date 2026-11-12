#include <iostream>           // Include standard input-output stream library
#include <unistd.h>           // Include POSIX system calls for process control
#include <sys/types.h>        // Include definitions for data types used in system calls
#include <sys/wait.h>         // Include definitions for waiting for child process completion
#include <stdlib.h>           // Include standard library for functions like srand(), rand()
#include <time.h>             // Include time library to use the time() function for randomization
#include <string.h>           // Include string library for string manipulation
#include <fcntl.h>            // Include file control library for file descriptors (e.g., setting non-blocking)

using namespace std;          // Use the standard namespace to avoid prefixing std::

// Define constants for number of players and other game parameters
#define no_of_players 2        // Set number of players
#define buffer_size 100      // Set the maximum buffer size for messages
#define roll_no 2327      // Define a constant value used for calculating the number of zombies
#define no_of_zombies ((roll_no * 3) % 20 + 1) // Calculate number of zombies based on roll_no

// Define a structure to represent a character (both players and zombies)
struct Character 
{
    int health;             // Store the health of the character
    char weapon[20];        // Store the weapon name of the character
};

// Initialize the players with health and weapon names
Character players[no_of_players] = {{100, "KNIFE"}, {80, "PISTOL"}};
// Declare an array of zombies, the number will be calculated based on no_of_zombies
Character zombies[no_of_zombies];

// Declare pipe arrays for communication between players and zombies
int player_pipes[no_of_players][2];  // Pipes for communication between player processes
int zombie_pipes[no_of_zombies][2];  // Pipes for communication between zombie processes

// Function to set a file descriptor as non-blocking (for pipes)
void set_nonblocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);           // Get the current flags of the file descriptor
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);      // Set the file descriptor to non-blocking mode
}

// Function for player processes
void player_process(int id) 
{
    close(player_pipes[id][0]);  // Close the read end of the pipe for this player (it will only write)
    
    // Close the pipes for other players to ensure no unnecessary open pipes
    for (int j = 0; j < no_of_players; j++) 
    {
        if (j != id) close(player_pipes[j][0]);   // Close read pipe of other players
        if (j != id) close(player_pipes[j][1]);   // Close write pipe of other players
    }

    srand(time(0) + getpid());  // Seed the random number generator based on current time and process ID

    // Main game loop where the player performs actions 5 times
    for (int i = 0; i < 5; i++) 
    {
        char message[buffer_size];  // Buffer to store the message
        int action = rand() % 3;    // Random action: 0 - move, 1 - attack, 2 - find weapon

        // Action based on the random number generated
        if (action == 0) 
            sprintf(message, "PLAYER %d MOVES.", id + 1); // Action: Player moves
        else if (action == 1) 
        { // Action: Player attacks a random zombie
            int target = rand() % no_of_zombies;         // Select a random zombie
            int damage = rand() % 20 + 5;              // Random damage between 5 and 25
            zombies[target].health -= damage;          // Reduce the zombie's health
            sprintf(message, "PLAYER %d ATTACKS ZOMBIE %d FOR %d DAMAGE!", id + 1, target + 1, damage);
        }
        else 
            sprintf(message, "PLAYER %d FINDS A SHOTGUN! [DAMAGE INCREASED]", id + 1); // Action: Player finds a weapon

        write(player_pipes[id][1], message, strlen(message) + 1); // Send the message to the parent process (game manager)
        sleep(1);  // Simulate delay between actions
    }

    close(player_pipes[id][1]);  // Close the write pipe after the actions are done
    exit(0);  // Exit the player process
}

// Function for zombie processes
void zombie_process(int id) 
{
    close(zombie_pipes[id][0]);  // Close the read end of the pipe for this zombie (it will only write)
    
    // Close the pipes for other zombies to ensure no unnecessary open pipes
    for (int j = 0; j < no_of_zombies; j++) 
    {
        if (j != id) close(zombie_pipes[j][0]);   // Close read pipe of other zombies
        if (j != id) close(zombie_pipes[j][1]);   // Close write pipe of other zombies
    }

    srand(time(0) + getpid());  // Seed the random number generator based on current time and process ID

    // Main game loop where the zombie performs actions 5 times
    for (int i = 0; i < 5; i++) 
    {
        char message[buffer_size];  // Buffer to store the message
        int target = rand() % no_of_players;   // Select a random player
        sprintf(message, "ZOMBIE %d BITES PLAYER %d!", id + 1, target + 1); // Action: Zombie bites player

        write(zombie_pipes[id][1], message, strlen(message) + 1);  // Send the message to the parent process (game manager)
        sleep(2);  // Simulate delay between actions
    }

    close(zombie_pipes[id][1]);  // Close the write pipe after the actions are done
    exit(0);  // Exit the zombie process
}

// Function to manage the game logic (score calculation, display results, etc.)
void game_manager() 
{
    cout << "\n\n\n\t\t\t\t===== ZOMBIE SURVIVAL GAME =====";  // Display game title
    // Display the health and weapon information for each player
    for (int i = 0; i < no_of_players; i++) {
        cout << "\n\n\n\t\t\t\tPLAYER " << i + 1 << ": [HEALTH: " << players[i].health << "] [WEAPON: " << players[i].weapon << "]";
    }

    cout << "\n\n\n\t\t\t\tZOMBIES: " << no_of_zombies << " APPROACHING......";  // Display the number of zombies

    char buffer[buffer_size];  // Buffer to store messages received from players and zombies
    int turns = 0;             // Turn counter
    int player_score = 0, zombie_score = 0;  // Scores for players and zombies

    // Set pipes to non-blocking mode
    for (int i = 0; i < no_of_players; i++) set_nonblocking(player_pipes[i][0]);
    for (int i = 0; i < no_of_zombies; i++) set_nonblocking(zombie_pipes[i][0]);

    // Main game loop (runs for 5 turns)
    while (turns < 5) {
        // Read messages from player pipes and update player score
        for (int i = 0; i < no_of_players; i++) {
            int bytes_read = read(player_pipes[i][0], buffer, buffer_size);
            if (bytes_read > 0) {
                cout << "\n\n\n\t\t\t\t>> " << buffer << endl;  // Display the player's action
                if (strstr(buffer, "ATTACKS")) player_score++;  // Increment score if the player attacks a zombie
            }
        }

        // Read messages from zombie pipes and update zombie score
        for (int i = 0; i < no_of_zombies; i++) {
            int bytes_read = read(zombie_pipes[i][0], buffer, buffer_size);
            if (bytes_read > 0) {
                cout << "\n\n\n\t\t\t\t>> " << buffer << endl;  // Display the zombie's action
                if (strstr(buffer, "BITES")) zombie_score++;  // Increment score if the zombie bites a player
            }
        }

        cout << "\n\n\n\t\t\t\t[NEXT TURN]";  // Display message for the next turn
        sleep(1);  // Simulate delay between turns
        turns++;  // Increment the turn counter
    }

    // Display final game results
    cout << "\n\n\n\t\t\t\t===== GAME OVER! =====";
    cout << "\n\n\n\t\t\t\tPLAYER SCORE: " << player_score;
    cout << "\n\n\n\t\t\t\tZOMBIE SCORE: " << zombie_score;

    // Determine and display the winner
    if (player_score > zombie_score)
        cout << "\n\n\n\t\t\t\t>> PLAYERS WIN!";  // Players win if their score is higher
    else if (zombie_score > player_score)
        cout << "\n\n\n\t\t\t\t>> ZOMBIES WIN!";  // Zombies win if their score is higher
    else
        cout << "\n\n\n\t\t\t\t>> IT'S A DRAW!";  // It's a draw if scores are tied
}

int main() 
{
    srand(time(0));  // Seed the random number generator

    pid_t pid;  // Variable to store process IDs

    // Create pipes for each player and zombie
    for (int i = 0; i < no_of_players; i++) pipe(player_pipes[i]);
    for (int i = 0; i < no_of_zombies; i++) pipe(zombie_pipes[i]);

    // Initialize the zombies with random health and weapon
    for (int i = 0; i < no_of_zombies; i++) 
    {
        zombies[i].health = 50 + (rand() % 30);  // Random health between 50 and 80
        strcpy(zombies[i].weapon, "BITE");  // Assign the weapon as "Bite"
    }

    // Create player processes
    for (int i = 0; i < no_of_players; i++) 
    {
        pid = fork();  // Create a new process
        if (pid == 0) player_process(i);  // In child process, execute player logic
    }

    // Create zombie processes
    for (int i = 0; i < no_of_zombies; i++) 
    {
        pid = fork();  // Create a new process
        if (pid == 0) zombie_process(i);  // In child process, execute zombie logic
    }

    game_manager();  // Execute the game manager logic

    // Wait for all child processes to finish
    while (wait(NULL) > 0);

    // Close the pipe read ends after the game is done
    for (int i = 0; i < no_of_players; i++) close(player_pipes[i][0]);
    for (int i = 0; i < no_of_zombies; i++) close(zombie_pipes[i][0]);

    return 0;  // Exit the program
}

