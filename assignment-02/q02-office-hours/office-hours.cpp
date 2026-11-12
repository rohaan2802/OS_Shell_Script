#include <iostream>                     // Include header for input/output
#include <pthread.h>                    // Include pthread library for multi-threading
#include <unistd.h>                     // Include header for sleep and other POSIX functions
#include <atomic>                       // Include atomic library to handle atomic operations
#include <sched.h>                       // Include header for thread scheduling functions

using namespace std;                    // Use the standard namespace to avoid prefixing with 'std::'

// Declare atomic variables to ensure thread-safe operations without locks
atomic<int> current_student(0);         // To track which student's question is being answered (atomic to avoid data races)
atomic<bool> question_asked(false);     // Flag to indicate if a question has been asked (atomic to prevent issues with synchronization)
atomic<bool> answer_given(false);       // Flag to indicate if an answer has been given (atomic to avoid race conditions)
int total_students;                   // To store the total number of students (non-atomic because it's read-only in this context)

// Function to simulate a student asking a question
void question_start(int student_ID)
{
    // Wait until it is the student's turn to ask (i.e., wait until question_asked is false)
    while (question_asked.load()) 
    {
        sched_yield(); // Yield the CPU to allow other threads to run, avoiding busy waiting
    }

    current_student.store(student_ID);    // Store the current student's ID who is asking the question
    answer_given.store(false);            // Reset so this student waits for their own answer
    cout << "\n\n\n\t\t\t\tSTUDENT " << student_ID << " ASKS A QUESTION......";  // Print the message that the student is asking a question
    question_asked.store(true);            // Set the flag to indicate that a question has been asked
}

// Function to print the message indicating the question has been asked
void question_done()
{
    cout << "\n\n\n\t\t\t\tSTUDENT " << current_student.load() << " HAS ASKED THE QUESTION....."; // Print the message confirming that the student has asked the question
}

// Function to simulate the professor answering the question
void answer_start()
{
    // Wait until a student has asked a question (i.e., wait until question_asked is true)
    while (!question_asked.load()) 
    {
        cout << "\n\n\n\t\t\t\tPROFESSOR IS SLEEPING.....";  // Print a message indicating the professor is idle
        sleep(1); // Sleep for 1 second to simulate professor "sleeping"
    }

    // Once a question is asked, the professor starts answering
    cout << "\n\n\n\t\t\t\tPROFESSOR STARTS ANSWERING STUDENT " << current_student.load() << "'S QUESTION....."; // Print when the professor starts answering
    sleep(2); // Sleep for 2 seconds to simulate the time it takes for the professor to answer
}

// Function to simulate the professor finishing the answer
void answer_done()
{
    // Print the message confirming the professor has answered the question
    cout << "\n\n\n\t\t\t\tPROFESSOR HAS ANSWERED STUDENT " << current_student.load() << "'S QUESTION.....";
    answer_given.store(true);  // Set the flag to indicate that an answer has been given
    question_asked.store(false); // Reset the question_asked flag to allow the next student to ask a question
}

// Student thread function
void* student(void* arg)
{
    int student_ID = *(int*)arg;  // Retrieve the student's ID passed as an argument
    question_start(student_ID);   // Call function to ask the question
    question_done();              // Call function to mark question as done
    while (!answer_given.load())   // Wait for the answer to be given (i.e., the professor answers the question)
    {
        sched_yield();            // Yield the CPU to allow other threads to run (prevents busy waiting)
    }
    cout << "\n\n\n\t\t\t\tSTUDENT " << student_ID << " RECEIVED THE ANSWER AND LEAVES......"; // Print when student leaves after receiving the answer
    return NULL; // Return NULL as the function completes
}

// Professor thread function
void* professor(void* arg)
{
    while (true) 
    {
        answer_start();           // Start answering the question (the professor waits for a question to be asked)
        answer_done();            // Finish answering the question (the professor finishes the answer and resets flags)
    }
    return NULL; // Return NULL as the function completes
}

int main() 
{
    cout << "\n\n\n\t\t\t\tENTER THE NUMBER OF STUDENTS: 		";  // Prompt the user to enter the number of students
    cin >> total_students;          // Take input for the number of students

    pthread_t prof_thread, student_threads[total_students];  // Declare threads for professor and students
    int student_ids[total_students];                          // Array to store student IDs for each thread
    
    pthread_create(&prof_thread, NULL, professor, NULL);  // Create the professor thread
    sleep(1);  // Sleep for 1 second before starting the student threads (to give professor time to initialize)
    
    // Create threads for each student
    for (int i = 0; i < total_students; i++) 
    {
        student_ids[i] = i + 1;  // Assign each student a unique ID
        pthread_create(&student_threads[i], NULL, student, &student_ids[i]);  // Create a student thread for each student
        sleep(1);  // Sleep for 1 second before creating the next student thread
    }
    
    // Join all student threads (wait for them to complete)
    for (int i = 0; i < total_students; i++) 
    {
        pthread_join(student_threads[i], NULL);  // Wait for each student thread to finish
    }
    
    cout << "\n\n\n";  // Print a newline to indicate program completion
    return 0;  // End of program
}


/*

Atomic Variables: current_student, question_asked, and answer_given are atomic variables to avoid race conditions and ensure thread-safe operations.

question_start Function: Allows a student to ask a question by checking if the professor is ready. The student's ID is stored and the question_asked flag is set.

question_done Function: Prints the message that the student has asked the question.

answer_start Function: The professor waits for a question to be asked. Once the question is available, the professor starts answering it.

answer_done Function: Marks the question as answered and resets flags to allow the next student to ask a question.

Student Threads: Each student waits for their turn to ask a question, then waits for the answer.

Professor Thread: Continuously checks for questions to answer. It waits until the question is asked, answers it, and resets the flags.


*/


/*Explanation of the Atomic Library Usage:
Why atomic is used:

The atomic library ensures that certain variables are manipulated atomically, meaning they can be safely accessed or modified by multiple threads without causing data races or undefined behavior. It guarantees that the reading, writing, or modifying of atomic variables is done in a thread-safe manner.

In this code, variables like current_student, question_asked, and answer_given are shared between the professor and multiple student threads. Without atomic variables, there could be a race condition where multiple threads attempt to read or modify these variables simultaneously, leading to inconsistencies or crashes.

Atomic Operations:

atomic<int> and atomic<bool> are used to store the student ID, question status, and answer status. Operations like store(), load(), and exchange() are atomic, meaning the thread executing them cannot be interrupted midway, ensuring the integrity of the data shared between threads.

The Problem and the Solution Technique:
The Problem:

In a multi-threaded environment, multiple threads (students and professor) are trying to access and modify shared data (such as current_student, question_asked, and answer_given). This leads to potential issues such as race conditions, where the threads access and modify the shared data simultaneously, causing inconsistency in the program's behavior.

The Solution:

The atomic library is used to handle these shared variables in a thread-safe manner without requiring mutex locks.

The program uses synchronization techniques like sched_yield() to allow threads to yield control voluntarily to avoid one thread hogging the CPU and to manage the state transitions between different threads (professor answering questions and students asking them).

The main synchronization problem (waiting for the professor to answer before proceeding with the next student) is handled by the atomic flags question_asked and answer_given, which ensure that only one student asks a question at a time and the professor answers the question before another student can ask.*/

