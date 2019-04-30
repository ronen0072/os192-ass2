#include "../include/Tests.h"
#include "../include/ProjectTest.h"
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <math.h>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <vector>

using std::vector;
using std::streambuf;
using std::istringstream;
using std::to_string;
using std::cout;
using std::sprintf;
using std::complex;
using std::ifstream;
using std::stringstream;

#define RED   "\e[38;5;196m"
#define GRN   "\e[38;5;082m"
#define YEL   "\e[38;5;226m"
#define MAG   "\e[38;5;201m"
#define RESET "\e[0m"

// Defining Global Variables
vector<string> testsInputs;
vector<string> testsExpected;
vector<vector<string>> testsUserPrograms;
vector<string> testsHints;
vector<int> tesTimeLimits;

std::stringstream ss;
std::streambuf *old_buf;
extern int abortExecution;
extern int timeForSmallTest;
extern int testToExecute;

// Initializing before execution of tests
void Initialize()
{ 
  //change the underlying cout buffer and save the old buffer
  old_buf = std::cout.rdbuf(ss.rdbuf());

  // Catching signal
  signal(SIGINT,sigintHandler);
  signal(SIGQUIT,sigquitHandler);
}

// Finialize after execution of tests
void Finialize()
{   
    // Restoring cout buffer
    std::cout.rdbuf(old_buf);

    // Printing cout output
    std::string text_output = ss.str();
    std::cout << text_output;
}

// Initializing tests to be executed
void InitializingTests()
{
    /* ### Example of use ###

     testsFunctions.push_back();

    */

    // Initializing tests to be executed
    testsFunctions.push_back(Operating_System_Test);

    // Defining tests
    string test_0_Input = R"V0G0N(
OforktestO
    )V0G0N";
    string test_0_Expected = R"V0G0N(
$ fork test
fork test OK
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_0_UserPrograms = {"quitXV6","OforktestO"};
    string test_0_Hint = "General problem with xv6 due to changes made to it.";
    int test_0_time_limit = 240000;

    string test_1_Input = R"V0G0N(
OusertestsO
)V0G0N";
    string test_1_Expected = "#ALL TESTS PASSED"; 
    vector<string> test_1_UserPrograms = {"quitXV6","OusertestsO"};
    string test_1_Hint = "General problem with xv6 due to changes made to it."; 
    int test_1_time_limit = 1800000;

    string test_2_Input = R"V0G0N(
kt_sys1
    )V0G0N";
    string test_2_Expected = R"V0G0N(
$ thread 1 entering
thread 1 exiting
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_2_UserPrograms = {"quitXV6","kt_sys1"};
    string test_2_Hint = "Problem with creating threads and exiting threads.";
    int test_2_time_limit = 240000;

    string test_3_Input = R"V0G0N(
kt_sys2
    )V0G0N";
    string test_3_Expected = R"V0G0N(
$ thread 1 entering
thread 1 exiting
thread 2 entering
thread 2 exiting
thread 3 entering
thread 3 exiting
thread 4 entering
thread 4 exiting
thread 5 entering
thread 5 exiting
thread 6 entering
thread 6 exiting
thread 7 entering
thread 7 exiting
thread 8 entering
thread 8 exiting
thread 9 entering
thread 9 exiting
thread 10 entering
thread 10 exiting
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_3_UserPrograms = {"quitXV6","kt_sys2"};
    string test_3_Hint = "Problem with creating threads and exiting threads.";
    int test_3_time_limit = 240000;

    string test_4_Input = R"V0G0N(
kt_sys3
    )V0G0N";
    string test_4_Expected = R"V0G0N(
$ Starting joining threads, should indicate when each thread was sucessfully joined soon
thread 1 entering
thread 1 exiting
Finished joing thread 1
thread 2 entering
thread 2 exiting
Finished joing thread 2
thread 3 entering
thread 3 exiting
Finished joing thread 3
thread 4 entering
thread 4 exiting
Finished joing thread 4
thread 5 entering
thread 5 exiting
Finished joing thread 5
thread 6 entering
thread 6 exiting
Finished joing thread 6
thread 7 entering
thread 7 exiting
Finished joing thread 7
thread 8 entering
thread 8 exiting
Finished joing thread 8
thread 9 entering
thread 9 exiting
Finished joing thread 9
thread 10 entering
thread 10 exiting
Finished joing thread 10
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_4_UserPrograms = {"quitXV6","kt_sys3","sleep"};
    string test_4_Hint = "Problem with joing on threads.";
    int test_4_time_limit = 240000;

    string test_5_Input = R"V0G0N(
kt_sys4
    )V0G0N";
    string test_5_Expected = R"V0G0N(
$ Creating thread 1
Finished creating thread 1
Creating thread 2
Finished creating thread 2
Creating thread 3
Finished creating thread 3
Creating thread 4
Finished creating thread 4
Creating thread 5
Finished creating thread 5
Creating thread 6
Finished creating thread 6
Creating thread 7
Finished creating thread 7
Creating thread 8
Finished creating thread 8
Creating thread 9
Finished creating thread 9
Creating thread 10
Finished creating thread 10
Eexcuting exec sleep
Sleep starting sleep !!!
Sleep exiting !!!
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_5_UserPrograms = {"quitXV6","kt_sys4","sleep"};
    string test_5_Hint = "Problem with exec with threads alive.";
    int test_5_time_limit = 240000;

    string test_6_Input = R"V0G0N(
kt_sys5
    )V0G0N";
    string test_6_Expected = R"V0G0N(
$ Creating thread 1
Created thread 1 successfully
Creating thread 2
Created thread 2 successfully
Creating thread 3
Created thread 3 successfully
Creating thread 4
Created thread 4 successfully
Creating thread 5
Created thread 5 successfully
Creating thread 6
Created thread 6 successfully
Creating thread 7
Created thread 7 successfully
Creating thread 8
Created thread 8 successfully
Creating thread 9
Created thread 9 successfully
Creating thread 10
Created thread 10 successfully
Creating thread 11
Created thread 11 successfully
Creating thread 12
Created thread 12 successfully
Creating thread 13
Created thread 13 successfully
Creating thread 14
Created thread 14 successfully
Creating thread 15
Created thread 15 successfully
Creating thread 16
Created thread 16 unsuccessfully
Should have successfully created all threads but one
Eexcuting exec creThreads
Creating thread 1
Finished creating thread 1 successfully
Creating thread 2
Finished creating thread 2 successfully
Creating thread 3
Finished creating thread 3 successfully
Creating thread 4
Finished creating thread 4 successfully
Creating thread 5
Finished creating thread 5 successfully
Creating thread 6
Finished creating thread 6 successfully
Creating thread 7
Finished creating thread 7 successfully
Creating thread 8
Finished creating thread 8 successfully
Creating thread 9
Finished creating thread 9 successfully
Creating thread 10
Finished creating thread 10 successfully
Creating thread 11
Finished creating thread 11 successfully
Creating thread 12
Finished creating thread 12 successfully
Creating thread 13
Finished creating thread 13 successfully
Creating thread 14
Finished creating thread 14 successfully
Creating thread 15
Finished creating thread 15 successfully
Creating thread 16
Finished creating thread 16 unsuccessfully
Should have sucessfully created all threads but one
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_6_UserPrograms = {"quitXV6","kt_sys5","creThreads"};
    string test_6_Hint = "Problem with exec with threads alive.";
    int test_6_time_limit = 240000;

    string test_7_Input = R"V0G0N(
kt_sys6
    )V0G0N";
    string test_7_Expected = R"V0G0N(
$ Created thread 1 successfully
Created thread 2 successfully
Created thread 3 successfully
Created thread 4 successfully
Created thread 5 successfully
Created thread 6 successfully
Created thread 7 successfully
Created thread 8 successfully
Created thread 9 successfully
Created thread 10 successfully
Created thread 11 successfully
Created thread 12 successfully
Created thread 13 successfully
Created thread 14 successfully
Created thread 15 successfully
Created thread 16 unsuccessfully
Starting joining threads, should indicate when each thread was sucessfully joined soon
Attempting to join thread 1
thread 1 entering
thread 1 exiting
Finished joing thread 1
Attempting to join thread 2
thread 2 entering
thread 2 exiting
Finished joing thread 2
Attempting to join thread 3
thread 3 entering
thread 3 exiting
Finished joing thread 3
Attempting to join thread 4
thread 4 entering
thread 4 exiting
Finished joing thread 4
Attempting to join thread 5
thread 5 entering
thread 5 exiting
Finished joing thread 5
Attempting to join thread 6
thread 6 entering
thread 6 exiting
Finished joing thread 6
Attempting to join thread 7
thread 7 entering
thread 7 exiting
Finished joing thread 7
Attempting to join thread 8
thread 8 entering
thread 8 exiting
Finished joing thread 8
Attempting to join thread 9
thread 9 entering
thread 9 exiting
Finished joing thread 9
Attempting to join thread 10
thread 10 entering
thread 10 exiting
Finished joing thread 10
Attempting to join thread 11
thread 11 entering
thread 11 exiting
Finished joing thread 11
Attempting to join thread 12
thread 12 entering
thread 12 exiting
Finished joing thread 12
Attempting to join thread 13
thread 13 entering
thread 13 exiting
Finished joing thread 13
Attempting to join thread 14
thread 14 entering
thread 14 exiting
Finished joing thread 14
Attempting to join thread 15
thread 15 entering
thread 15 exiting
Finished joing thread 15
Attempting to join thread 16
Error in joing thread 16
Attempting to join thread 1
Thread 1 isn't my thread anymore, as it should be
Attempting to join thread 2
Thread 2 isn't my thread anymore, as it should be
Attempting to join thread 3
Thread 3 isn't my thread anymore, as it should be
Attempting to join thread 4
Thread 4 isn't my thread anymore, as it should be
Attempting to join thread 5
Thread 5 isn't my thread anymore, as it should be
Attempting to join thread 6
Thread 6 isn't my thread anymore, as it should be
Attempting to join thread 7
Thread 7 isn't my thread anymore, as it should be
Attempting to join thread 8
Thread 8 isn't my thread anymore, as it should be
Attempting to join thread 9
Thread 9 isn't my thread anymore, as it should be
Attempting to join thread 10
Thread 10 isn't my thread anymore, as it should be
Attempting to join thread 11
Thread 11 isn't my thread anymore, as it should be
Attempting to join thread 12
Thread 12 isn't my thread anymore, as it should be
Attempting to join thread 13
Thread 13 isn't my thread anymore, as it should be
Attempting to join thread 14
Thread 14 isn't my thread anymore, as it should be
Attempting to join thread 15
Thread 15 isn't my thread anymore, as it should be
Attempting to join thread 16
Thread 16 isn't my thread anymore, as it should be
Created thread 17 successfully
Created thread 18 successfully
Created thread 19 successfully
Created thread 20 successfully
Created thread 21 successfully
Created thread 22 successfully
Created thread 23 successfully
Created thread 24 successfully
Created thread 25 successfully
Created thread 26 successfully
Created thread 27 successfully
Created thread 28 successfully
Created thread 29 successfully
Created thread 30 successfully
Created thread 31 successfully
Created thread 32 unsuccessfully
Attempting to join thread 17
thread 17 entering
thread 17 exiting
Finished joing thread 17
Attempting to join thread 18
thread 18 entering
thread 18 exiting
Finished joing thread 18
Attempting to join thread 19
thread 19 entering
thread 19 exiting
Finished joing thread 19
Attempting to join thread 20
thread 20 entering
thread 20 exiting
Finished joing thread 20
Attempting to join thread 21
thread 21 entering
thread 21 exiting
Finished joing thread 21
Attempting to join thread 22
thread 22 entering
thread 22 exiting
Finished joing thread 22
Attempting to join thread 23
thread 23 entering
thread 23 exiting
Finished joing thread 23
Attempting to join thread 24
thread 24 entering
thread 24 exiting
Finished joing thread 24
Attempting to join thread 25
thread 25 entering
thread 25 exiting
Finished joing thread 25
Attempting to join thread 26
thread 26 entering
thread 26 exiting
Finished joing thread 26
Attempting to join thread 27
thread 27 entering
thread 27 exiting
Finished joing thread 27
Attempting to join thread 28
thread 28 entering
thread 28 exiting
Finished joing thread 28
Attempting to join thread 29
thread 29 entering
thread 29 exiting
Finished joing thread 29
Attempting to join thread 30
thread 30 entering
thread 30 exiting
Finished joing thread 30
Attempting to join thread 31
thread 31 entering
thread 31 exiting
Finished joing thread 31
Attempting to join thread 32
Error in joing thread 32
Attempting to join thread 17
Thread 17 isn't my thread anymore, as it should be
Attempting to join thread 18
Thread 18 isn't my thread anymore, as it should be
Attempting to join thread 19
Thread 19 isn't my thread anymore, as it should be
Attempting to join thread 20
Thread 20 isn't my thread anymore, as it should be
Attempting to join thread 21
Thread 21 isn't my thread anymore, as it should be
Attempting to join thread 22
Thread 22 isn't my thread anymore, as it should be
Attempting to join thread 23
Thread 23 isn't my thread anymore, as it should be
Attempting to join thread 24
Thread 24 isn't my thread anymore, as it should be
Attempting to join thread 25
Thread 25 isn't my thread anymore, as it should be
Attempting to join thread 26
Thread 26 isn't my thread anymore, as it should be
Attempting to join thread 27
Thread 27 isn't my thread anymore, as it should be
Attempting to join thread 28
Thread 28 isn't my thread anymore, as it should be
Attempting to join thread 29
Thread 29 isn't my thread anymore, as it should be
Attempting to join thread 30
Thread 30 isn't my thread anymore, as it should be
Attempting to join thread 31
Thread 31 isn't my thread anymore, as it should be
Attempting to join thread 32
Thread 32 isn't my thread anymore, as it should be
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_7_UserPrograms = {"quitXV6","kt_sys6"};
    string test_7_Hint = "Problem with cleaning after threads in thread join.";
    int test_7_time_limit = 420000;

    string test_8_Input = R"V0G0N(
kt_sys7
    )V0G0N";
    string test_8_Expected = R"V0G0N(
$ Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
Creating process 1 out of 61
Creating process 2 out of 61
Creating process 3 out of 61
Creating process 4 out of 61
Creating process 5 out of 61
Creating process 6 out of 61
Creating process 7 out of 61
Creating process 8 out of 61
Creating process 9 out of 61
Creating process 10 out of 61
Creating process 11 out of 61
Creating process 12 out of 61
Creating process 13 out of 61
Creating process 14 out of 61
Creating process 15 out of 61
Creating process 16 out of 61
Creating process 17 out of 61
Creating process 18 out of 61
Creating process 19 out of 61
Creating process 20 out of 61
Creating process 21 out of 61
Creating process 22 out of 61
Creating process 23 out of 61
Creating process 24 out of 61
Creating process 25 out of 61
Creating process 26 out of 61
Creating process 27 out of 61
Creating process 28 out of 61
Creating process 29 out of 61
Creating process 30 out of 61
Creating process 31 out of 61
Creating process 32 out of 61
Creating process 33 out of 61
Creating process 34 out of 61
Creating process 35 out of 61
Creating process 36 out of 61
Creating process 37 out of 61
Creating process 38 out of 61
Creating process 39 out of 61
Creating process 40 out of 61
Creating process 41 out of 61
Creating process 42 out of 61
Creating process 43 out of 61
Creating process 44 out of 61
Creating process 45 out of 61
Creating process 46 out of 61
Creating process 47 out of 61
Creating process 48 out of 61
Creating process 49 out of 61
Creating process 50 out of 61
Creating process 51 out of 61
Creating process 52 out of 61
Creating process 53 out of 61
Creating process 54 out of 61
Creating process 55 out of 61
Creating process 56 out of 61
Creating process 57 out of 61
Creating process 58 out of 61
Creating process 59 out of 61
Creating process 60 out of 61
Creating process 61 out of 61
Waiting for process 1 out of 61
Waiting for process 2 out of 61
Waiting for process 3 out of 61
Waiting for process 4 out of 61
Waiting for process 5 out of 61
Waiting for process 6 out of 61
Waiting for process 7 out of 61
Waiting for process 8 out of 61
Waiting for process 9 out of 61
Waiting for process 10 out of 61
Waiting for process 11 out of 61
Waiting for process 12 out of 61
Waiting for process 13 out of 61
Waiting for process 14 out of 61
Waiting for process 15 out of 61
Waiting for process 16 out of 61
Waiting for process 17 out of 61
Waiting for process 18 out of 61
Waiting for process 19 out of 61
Waiting for process 20 out of 61
Waiting for process 21 out of 61
Waiting for process 22 out of 61
Waiting for process 23 out of 61
Waiting for process 24 out of 61
Waiting for process 25 out of 61
Waiting for process 26 out of 61
Waiting for process 27 out of 61
Waiting for process 28 out of 61
Waiting for process 29 out of 61
Waiting for process 30 out of 61
Waiting for process 31 out of 61
Waiting for process 32 out of 61
Waiting for process 33 out of 61
Waiting for process 34 out of 61
Waiting for process 35 out of 61
Waiting for process 36 out of 61
Waiting for process 37 out of 61
Waiting for process 38 out of 61
Waiting for process 39 out of 61
Waiting for process 40 out of 61
Waiting for process 41 out of 61
Waiting for process 42 out of 61
Waiting for process 43 out of 61
Waiting for process 44 out of 61
Waiting for process 45 out of 61
Waiting for process 46 out of 61
Waiting for process 47 out of 61
Waiting for process 48 out of 61
Waiting for process 49 out of 61
Waiting for process 50 out of 61
Waiting for process 51 out of 61
Waiting for process 52 out of 61
Waiting for process 53 out of 61
Waiting for process 54 out of 61
Waiting for process 55 out of 61
Waiting for process 56 out of 61
Waiting for process 57 out of 61
Waiting for process 58 out of 61
Waiting for process 59 out of 61
Waiting for process 60 out of 61
Waiting for process 61 out of 61
$ $ Finished Yehonatan Peleg Test, quiting...

)V0G0N";
    vector<string> test_8_UserPrograms = {"quitXV6","kt_sys7","cThreW16T"};
    string test_8_Hint = "Problem with exit when other threads are alive and running, exiting dosen't leave the system in "
                         "consistent state.";
    int test_8_time_limit = 420000;

    string test_9_Input = R"V0G0N(
kt_sys8
    )V0G0N";
    string test_9_Expected = R"V0G0N(
$ Starting joining threads, should indicate when each thread was sucessfully joined soon
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Thread 1 isn't my thread anymore, as it should be
Thread 2 isn't my thread anymore, as it should be
Thread 3 isn't my thread anymore, as it should be
Thread 4 isn't my thread anymore, as it should be
Thread 5 isn't my thread anymore, as it should be
Thread 6 isn't my thread anymore, as it should be
Thread 7 isn't my thread anymore, as it should be
Thread 8 isn't my thread anymore, as it should be
Thread 9 isn't my thread anymore, as it should be
Thread 10 isn't my thread anymore, as it should be
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_9_UserPrograms = {"quitXV6","kt_sys8"};
    string test_9_Hint = "Problem with kthread join, didn't catch threads that exit immediately";
    int test_9_time_limit = 240000;

    string test_10_Input = R"V0G0N(
kt_sys9
    )V0G0N";
    string test_10_Expected = R"V0G0N(
$ Creating thread 1
Created thread 1 successfully
Creating thread 2
Created thread 2 successfully
Creating thread 3
Created thread 3 successfully
Creating thread 4
Created thread 4 successfully
Creating thread 5
Created thread 5 successfully
Creating thread 6
Created thread 6 successfully
Creating thread 7
Created thread 7 successfully
Creating thread 8
Created thread 8 successfully
Creating thread 9
Created thread 9 successfully
Creating thread 10
Created thread 10 successfully
Creating thread 11
Created thread 11 successfully
Creating thread 12
Created thread 12 successfully
Creating thread 13
Created thread 13 successfully
Creating thread 14
Created thread 14 successfully
Creating thread 15
Created thread 15 successfully
Starting joining threads, should indicate when each thread was sucessfully joined soon
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Attempting to join thread 1
Thread 1 isn't my thread anymore, as it should be
Attempting to join thread 2
Thread 2 isn't my thread anymore, as it should be
Attempting to join thread 3
Thread 3 isn't my thread anymore, as it should be
Attempting to join thread 4
Thread 4 isn't my thread anymore, as it should be
Attempting to join thread 5
Thread 5 isn't my thread anymore, as it should be
Attempting to join thread 6
Thread 6 isn't my thread anymore, as it should be
Attempting to join thread 7
Thread 7 isn't my thread anymore, as it should be
Attempting to join thread 8
Thread 8 isn't my thread anymore, as it should be
Attempting to join thread 9
Thread 9 isn't my thread anymore, as it should be
Attempting to join thread 10
Thread 10 isn't my thread anymore, as it should be
Attempting to join thread 11
Thread 11 isn't my thread anymore, as it should be
Attempting to join thread 12
Thread 12 isn't my thread anymore, as it should be
Attempting to join thread 13
Thread 13 isn't my thread anymore, as it should be
Attempting to join thread 14
Thread 14 isn't my thread anymore, as it should be
Attempting to join thread 15
Thread 15 isn't my thread anymore, as it should be
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_10_UserPrograms = {"quitXV6","kt_sys9"};
    string test_10_Hint = "Problem with kthread id, using thread join so it should work also";
    int test_10_time_limit = 240000;
    
    string test_11_Input = R"V0G0N(
kt_sys10
    )V0G0N";
    string test_11_Expected = R"V0G0N(
$ Creating thread 1
Created thread 1 successfully
Creating thread 2
Created thread 2 successfully
Creating thread 3
Created thread 3 successfully
Creating thread 4
Created thread 4 successfully
Creating thread 5
Created thread 5 successfully
Creating thread 6
Created thread 6 successfully
Creating thread 7
Created thread 7 successfully
Creating thread 8
Created thread 8 successfully
Creating thread 9
Created thread 9 successfully
Creating thread 10
Created thread 10 successfully
Creating thread 11
Created thread 11 successfully
Creating thread 12
Created thread 12 successfully
Creating thread 13
Created thread 13 successfully
Creating thread 14
Created thread 14 successfully
Creating thread 15
Created thread 15 successfully
From all threads, become one thread due to exec
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_11_UserPrograms = {"quitXV6","kt_sys10","allBut1"};
    string test_11_Hint = "Problem with exec, multiple exec calls do not work when multiple threads are alive and doing exec";
    int test_11_time_limit = 240000;

    string test_12_Input = R"V0G0N(
kt_sys11
    )V0G0N";
    string test_12_Expected = R"V0G0N(
$ Creating thread 1
Created thread 1 successfully
Creating thread 2
Created thread 2 successfully
Creating thread 3
Created thread 3 successfully
Creating thread 4
Created thread 4 successfully
Creating thread 5
Created thread 5 successfully
Creating thread 6
Created thread 6 successfully
Creating thread 7
Created thread 7 successfully
Creating thread 8
Created thread 8 successfully
Creating thread 9
Created thread 9 successfully
Creating thread 10
Created thread 10 successfully
Creating thread 11
Created thread 11 successfully
Creating thread 12
Created thread 12 successfully
Creating thread 13
Created thread 13 successfully
Creating thread 14
Created thread 14 successfully
Creating thread 15
Created thread 15 successfully
Program has exited successfully with kthread_exit
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_12_UserPrograms = {"quitXV6","kt_sys11"};
    string test_12_Hint = "Problem with kthread_exit, should end process when last thread calls";
    int test_12_time_limit = 240000;

    string test_13_Input = R"V0G0N(
kt_sys12
    )V0G0N";
    string test_13_Expected = R"V0G0N(
$ Creating thread 1
Created thread 1 successfully
Creating thread 2
Created thread 2 successfully
Creating thread 3
Created thread 3 successfully
Creating thread 4
Created thread 4 successfully
Creating thread 5
Created thread 5 successfully
Creating thread 6
Created thread 6 successfully
Creating thread 7
Created thread 7 successfully
Creating thread 8
Created thread 8 successfully
Creating thread 9
Created thread 9 successfully
Creating thread 10
Created thread 10 successfully
Creating thread 11
Created thread 11 successfully
Creating thread 12
Created thread 12 successfully
Creating thread 13
Created thread 13 successfully
Creating thread 14
Created thread 14 successfully
Creating thread 15
Created thread 15 successfully
Main test program exiting 
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_13_UserPrograms = {"quitXV6","kt_sys12","doNoth"};
    string test_13_Hint = "Problem with exit and exec, if executing simultaneously";
    int test_13_time_limit = 420000;

    string test_14_Input = R"V0G0N(
mutex1
    )V0G0N";
    string test_14_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_14_UserPrograms = {"quitXV6","mutex1"};
    string test_14_Hint = "Problem with mutex sys calls, basic implementation";
    int test_14_time_limit = 240000;

    string test_15_Input = R"V0G0N(
mutex2
    )V0G0N";
    string test_15_Expected = R"V0G0N(
$ Starting allocating a lot of mutexes in different process without deallocating
Waiting for 0 process
Waiting for 1 process
Waiting for 2 process
Waiting for 3 process
Waiting for 4 process
Waiting for 5 process
Waiting for 6 process
Waiting for 7 process
Waiting for 8 process
Waiting for 9 process
Waiting for 10 process
Waiting for 11 process
Waiting for 12 process
Waiting for 13 process
Waiting for 14 process
Waiting for 15 process
Waiting for 16 process
Waiting for 17 process
Waiting for 18 process
Waiting for 19 process
Waiting for 20 process
Waiting for 21 process
Waiting for 22 process
Waiting for 23 process
Waiting for 24 process
Waiting for 25 process
Waiting for 26 process
Waiting for 27 process
Waiting for 28 process
Waiting for 29 process
Waiting for 30 process
Waiting for 31 process
Waiting for 32 process
Waiting for 33 process
Waiting for 34 process
Waiting for 35 process
Waiting for 36 process
Waiting for 37 process
Waiting for 38 process
Waiting for 39 process
Waiting for 40 process
Waiting for 41 process
Waiting for 42 process
Waiting for 43 process
Waiting for 44 process
Waiting for 45 process
Waiting for 46 process
Waiting for 47 process
Waiting for 48 process
Waiting for 49 process
Waiting for 50 process
Waiting for 51 process
Waiting for 52 process
Waiting for 53 process
Waiting for 54 process
Waiting for 55 process
Waiting for 56 process
Waiting for 57 process
Waiting for 58 process
Waiting for 59 process
Waiting for 60 process
Waiting for 61 process
Waiting for 62 process
Waiting for 63 process
Waiting for 64 process
Waiting for 65 process
Waiting for 66 process
Waiting for 67 process
Waiting for 68 process
Waiting for 69 process
Waiting for 70 process
Waiting for 71 process
Waiting for 72 process
Waiting for 73 process
Waiting for 74 process
Waiting for 75 process
Waiting for 76 process
Waiting for 77 process
Waiting for 78 process
Waiting for 79 process
Waiting for 80 process
Waiting for 81 process
Waiting for 82 process
Waiting for 83 process
Waiting for 84 process
Waiting for 85 process
Waiting for 86 process
Waiting for 87 process
Waiting for 88 process
Waiting for 89 process
Waiting for 90 process
Waiting for 91 process
Waiting for 92 process
Waiting for 93 process
Waiting for 94 process
Waiting for 95 process
Waiting for 96 process
Waiting for 97 process
Waiting for 98 process
Waiting for 99 process
Waiting for 100 process
Waiting for 101 process
Waiting for 102 process
Waiting for 103 process
Waiting for 104 process
Waiting for 105 process
Waiting for 106 process
Waiting for 107 process
Waiting for 108 process
Waiting for 109 process
Waiting for 110 process
Waiting for 111 process
Waiting for 112 process
Waiting for 113 process
Waiting for 114 process
Waiting for 115 process
Waiting for 116 process
Waiting for 117 process
Waiting for 118 process
Waiting for 119 process
Waiting for 120 process
Waiting for 121 process
Waiting for 122 process
Waiting for 123 process
Waiting for 124 process
Waiting for 125 process
Waiting for 126 process
Waiting for 127 process
Waiting for 128 process
Waiting for 129 process
Waiting for 130 process
Waiting for 131 process
Waiting for 132 process
Waiting for 133 process
Waiting for 134 process
Waiting for 135 process
Waiting for 136 process
Waiting for 137 process
Waiting for 138 process
Waiting for 139 process
Waiting for 140 process
Waiting for 141 process
Waiting for 142 process
Waiting for 143 process
Waiting for 144 process
Waiting for 145 process
Waiting for 146 process
Waiting for 147 process
Waiting for 148 process
Waiting for 149 process
Waiting for 150 process
Waiting for 151 process
Waiting for 152 process
Waiting for 153 process
Waiting for 154 process
Waiting for 155 process
Waiting for 156 process
Waiting for 157 process
Waiting for 158 process
Waiting for 159 process
Waiting for 160 process
Waiting for 161 process
Waiting for 162 process
Waiting for 163 process
Waiting for 164 process
Waiting for 165 process
Waiting for 166 process
Waiting for 167 process
Waiting for 168 process
Waiting for 169 process
Waiting for 170 process
Waiting for 171 process
Waiting for 172 process
Waiting for 173 process
Waiting for 174 process
Waiting for 175 process
Waiting for 176 process
Waiting for 177 process
Waiting for 178 process
Waiting for 179 process
Waiting for 180 process
Waiting for 181 process
Waiting for 182 process
Waiting for 183 process
Waiting for 184 process
Waiting for 185 process
Waiting for 186 process
Waiting for 187 process
Waiting for 188 process
Waiting for 189 process
Waiting for 190 process
Waiting for 191 process
Waiting for 192 process
Waiting for 193 process
Waiting for 194 process
Waiting for 195 process
Waiting for 196 process
Waiting for 197 process
Waiting for 198 process
Waiting for 199 process
Finished allocating a lot of mutexes in different process without deallocating
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_15_UserPrograms = {"quitXV6","mutex2"};
    string test_15_Hint = "Problem with mutex allocation deallocation, allocating a lot of mutexes in different " 
                          "proccesses without explicitly deallocating shouldn't prevent new mutexes to be allocated";
    int test_15_time_limit = 240000;

    string test_16_Input = R"V0G0N(
mutex3
    )V0G0N";
    string test_16_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_16_UserPrograms = {"quitXV6","mutex3"};
    string test_16_Hint = "Problem with mutex allocation deallocation";
    int test_16_time_limit = 240000;

    string test_17_Input = R"V0G0N(
mutex4
    )V0G0N";
    string test_17_Expected = R"V0G0N(
$ Attempting to join thread 1
mutual exclusion satisfied
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_17_UserPrograms = {"quitXV6","mutex4"};
    string test_17_Hint = "Problem with mutex, mutual exclusion not satisfied";
    int test_17_time_limit = 240000;

    string test_18_Input = R"V0G0N(
mutex5
    )V0G0N";
    string test_18_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Sum is 7500
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_18_UserPrograms = {"quitXV6","mutex5"};
    string test_18_Hint = "Problem with mutex, mutual exclusion not satisfied";
    int test_18_time_limit = 420000;

    string test_19_Input = R"V0G0N(
mutex6
    )V0G0N";
    string test_19_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_19_UserPrograms = {"quitXV6","mutex6"};
    string test_19_Hint = "Problem with mutex, deallocated while locked is not prevented";
    int test_19_time_limit = 420000;

    string test_20_Input = R"V0G0N(
mutex7
    )V0G0N";
    string test_20_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Sum is 15000
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_20_UserPrograms = {"quitXV6","mutex7"};
    string test_20_Hint = "Problem with mutex, mutual exclusion not satisfied";
    int test_20_time_limit = 900000;

    string test_21_Input = R"V0G0N(
mutex8
    )V0G0N";
    string test_21_Expected = R"V0G0N(
$ Attempting to join thread 1
Thread 1 starting...
Thread 1 unlocking unlocked lock...
Thread 1 locking unlocked lock...
Thread 2 starting...
Thread 2 locking no true lock...
Thread 2 unlocking locked lock when not owner...
Thread 2 unlocking no true lock...
Thread 2 exiting...
Thread 1 unlocking locked lock...
Thread 1 exiting...
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_21_UserPrograms = {"quitXV6","mutex8"};
    string test_21_Hint = "Problem with mutex, locking and unlocking semantics-i.e., "
    "unlocking an unlock mutex should not be allowed, unlocking mutex when current thread "
    "is not the owner should not be allowed, etc...";
    int test_21_time_limit = 240000;

    string test_22_Input = R"V0G0N(
trntree1
    )V0G0N";
    string test_22_Expected = R"V0G0N(
$ $ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_22_UserPrograms = {"quitXV6","trntree1"};
    string test_22_Hint = "Problem with tournament tree, allocation deallocation";
    int test_22_time_limit = 240000;

    string test_23_Input = R"V0G0N(
trntree2
    )V0G0N";
    string test_23_Expected = R"V0G0N(
$ $ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_23_UserPrograms = {"quitXV6","trntree2"};
    string test_23_Hint = "Problem with tournament tree, acquiring and releasing";
    int test_23_time_limit = 240000;

    string test_24_Input = R"V0G0N(
trntree3
    )V0G0N";
    string test_24_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_24_UserPrograms = {"quitXV6","trntree3"};
    string test_24_Hint = "Problem with tournament tree, acquiring and releasing and dealloc while in use";
    int test_24_time_limit = 240000;

    string test_25_Input = R"V0G0N(
trntree4
    )V0G0N";
    string test_25_Expected = R"V0G0N(
$ Starting tournament test 4

---------------------------------------
Started current test for 1 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finished current test for 1 depth
---------------------------------------

---------------------------------------
Started current test for 2 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Finished current test for 2 depth
---------------------------------------

---------------------------------------
Started current test for 3 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Finished current test for 3 depth
---------------------------------------
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_25_UserPrograms = {"quitXV6","trntree4"};
    string test_25_Hint = "Problem with tournament tree, allocaing deallocating, acquiring and releasing in threads and not in main thread";
    int test_25_time_limit = 240000;

    string test_26_Input = R"V0G0N(
trntree5
    )V0G0N";
    string test_26_Expected = R"V0G0N(
$ Attempting to join thread 1
mutual exclusion satisfied
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_26_UserPrograms = {"quitXV6","trntree5"};
    string test_26_Hint = "Problem with tournament tree, mutual exculision in depth 1";
    int test_26_time_limit = 240000;

    string test_27_Input = R"V0G0N(
trntree6
    )V0G0N";
    string test_27_Expected = R"V0G0N(
$ Starting tournament test 6

---------------------------------------
Started current test for 1 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Sum is 1000
Finished current test for 1 depth
---------------------------------------

---------------------------------------
Started current test for 2 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Sum is 2000
Finished current test for 2 depth
---------------------------------------

---------------------------------------
Started current test for 3 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Sum is 4000
Finished current test for 3 depth
---------------------------------------

---------------------------------------
Started current test for 4 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Attempting to join thread 16
Error in joing thread 16
Sum is 7500
Finished current test for 4 depth
---------------------------------------
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_27_UserPrograms = {"quitXV6","trntree6"};
    string test_27_Hint = "Problem with tournament tree, mutual exculision not satisfied";
    int test_27_time_limit = 900000;

    string test_28_Input = R"V0G0N(
trntree7
    )V0G0N";
    string test_28_Expected = R"V0G0N(
$ Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_28_UserPrograms = {"quitXV6","trntree7"};
    string test_28_Hint = "Problem with tournament tree, deallocated while locked is not prevented";
    int test_28_time_limit = 240000;

    string test_29_Input = R"V0G0N(
trntree8
    )V0G0N";
    string test_29_Expected = R"V0G0N(
$ Starting tournament test 8

---------------------------------------
Started current test for 1 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Sum is 1000
Finished current test for 1 depth
---------------------------------------

---------------------------------------
Started current test for 2 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Sum is 2000
Finished current test for 2 depth
---------------------------------------

---------------------------------------
Started current test for 3 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Sum is 4000
Finished current test for 3 depth
---------------------------------------

---------------------------------------
Started current test for 4 depth
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Attempting to join thread 16
Error in joing thread 16
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Attempting to join thread 3
Finished joing thread 3
Attempting to join thread 4
Finished joing thread 4
Attempting to join thread 5
Finished joing thread 5
Attempting to join thread 6
Finished joing thread 6
Attempting to join thread 7
Finished joing thread 7
Attempting to join thread 8
Finished joing thread 8
Attempting to join thread 9
Finished joing thread 9
Attempting to join thread 10
Finished joing thread 10
Attempting to join thread 11
Finished joing thread 11
Attempting to join thread 12
Finished joing thread 12
Attempting to join thread 13
Finished joing thread 13
Attempting to join thread 14
Finished joing thread 14
Attempting to join thread 15
Finished joing thread 15
Attempting to join thread 16
Error in joing thread 16
Sum is 7500
Finished current test for 4 depth
---------------------------------------
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_29_UserPrograms = {"quitXV6","trntree8"};
    string test_29_Hint = "Problem with tournament tree, mutual exculision not satisfied";
    int test_29_time_limit = 900000;

    string test_30_Input = R"V0G0N(
trntree9
    )V0G0N";
    string test_30_Expected = R"V0G0N(
$ Starting tournament test 9

---------------------------------------
Started current test for 1 depth
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 1 depth with 0 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 1 depth with 0 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 1 depth with 1 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 1 depth with 1 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
Finished current test for 1 depth
---------------------------------------

---------------------------------------
Started current test for 2 depth
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 2 depth with 0 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 2 depth with 0 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 2 depth with 1 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 2 depth with 1 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 2 depth with 2 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 2 depth with 2 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 2 depth with 3 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 2 depth with 3 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
Finished current test for 2 depth
---------------------------------------

---------------------------------------
Started current test for 3 depth
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 0 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 0 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 1 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 1 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 2 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 2 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 3 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 3 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 4 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 4 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 5 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 5 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 6 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 6 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 3 depth with 7 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 3 depth with 7 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
Finished current test for 3 depth
---------------------------------------

---------------------------------------
Started current test for 4 depth
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 0 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 0 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 1 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 1 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 2 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 2 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 3 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 3 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 4 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 4 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 5 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 5 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 6 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 6 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 7 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 7 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 8 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 8 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 9 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 9 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 10 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 10 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 11 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 11 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 12 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 12 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 13 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 13 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 14 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 14 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
$$$$$$$$$$$$$$$$$$$$$$$$$$
Starting test for 4 depth with 15 user
Attempting to join thread 1
Finished joing thread 1
Attempting to join thread 2
Finished joing thread 2
Finishing test for 4 depth with 15 user
$$$$$$$$$$$$$$$$$$$$$$$$$$
Finished current test for 4 depth
---------------------------------------
$ $ Finished Yehonatan Peleg Test, quiting...
)V0G0N";
    vector<string> test_30_UserPrograms = {"quitXV6","trntree9"};
    string test_30_Hint = "Problem with tournament tree, dealloc while in use";
    int test_30_time_limit = 900000;

    // Adding Tests inputs and expected
    testsInputs.push_back(test_0_Input);
    testsExpected.push_back(test_0_Expected);
    testsUserPrograms.push_back(test_0_UserPrograms);
    testsHints.push_back(test_0_Hint);
    tesTimeLimits.push_back(test_0_time_limit);

    testsInputs.push_back(test_1_Input);
    testsExpected.push_back(test_1_Expected);
    testsUserPrograms.push_back(test_1_UserPrograms);
    testsHints.push_back(test_1_Hint);
    tesTimeLimits.push_back(test_1_time_limit);

    testsInputs.push_back(test_2_Input);
    testsExpected.push_back(test_2_Expected);
    testsUserPrograms.push_back(test_2_UserPrograms);
    testsHints.push_back(test_2_Hint);
    tesTimeLimits.push_back(test_2_time_limit);

    testsInputs.push_back(test_3_Input);
    testsExpected.push_back(test_3_Expected);
    testsUserPrograms.push_back(test_3_UserPrograms);
    testsHints.push_back(test_3_Hint);
    tesTimeLimits.push_back(test_3_time_limit);

    testsInputs.push_back(test_4_Input);
    testsExpected.push_back(test_4_Expected);
    testsUserPrograms.push_back(test_4_UserPrograms);
    testsHints.push_back(test_4_Hint);
    tesTimeLimits.push_back(test_4_time_limit);

    testsInputs.push_back(test_5_Input);
    testsExpected.push_back(test_5_Expected);
    testsUserPrograms.push_back(test_5_UserPrograms);
    testsHints.push_back(test_5_Hint);
    tesTimeLimits.push_back(test_5_time_limit);

    testsInputs.push_back(test_6_Input);
    testsExpected.push_back(test_6_Expected);
    testsUserPrograms.push_back(test_6_UserPrograms);
    testsHints.push_back(test_6_Hint);
    tesTimeLimits.push_back(test_6_time_limit);

    testsInputs.push_back(test_7_Input);
    testsExpected.push_back(test_7_Expected);
    testsUserPrograms.push_back(test_7_UserPrograms);
    testsHints.push_back(test_7_Hint);
    tesTimeLimits.push_back(test_7_time_limit);

    testsInputs.push_back(test_8_Input);
    testsExpected.push_back(test_8_Expected);
    testsUserPrograms.push_back(test_8_UserPrograms);
    testsHints.push_back(test_8_Hint);
    tesTimeLimits.push_back(test_8_time_limit);

    testsInputs.push_back(test_9_Input);
    testsExpected.push_back(test_9_Expected);
    testsUserPrograms.push_back(test_9_UserPrograms);
    testsHints.push_back(test_9_Hint);
    tesTimeLimits.push_back(test_9_time_limit);

    testsInputs.push_back(test_10_Input);
    testsExpected.push_back(test_10_Expected);
    testsUserPrograms.push_back(test_10_UserPrograms);
    testsHints.push_back(test_10_Hint);
    tesTimeLimits.push_back(test_10_time_limit);

    testsInputs.push_back(test_11_Input);
    testsExpected.push_back(test_11_Expected);
    testsUserPrograms.push_back(test_11_UserPrograms);
    testsHints.push_back(test_11_Hint);
    tesTimeLimits.push_back(test_11_time_limit);

    testsInputs.push_back(test_12_Input);
    testsExpected.push_back(test_12_Expected);
    testsUserPrograms.push_back(test_12_UserPrograms);
    testsHints.push_back(test_12_Hint);
    tesTimeLimits.push_back(test_12_time_limit);

    testsInputs.push_back(test_13_Input);
    testsExpected.push_back(test_13_Expected);
    testsUserPrograms.push_back(test_13_UserPrograms);
    testsHints.push_back(test_13_Hint);
    tesTimeLimits.push_back(test_13_time_limit);

    testsInputs.push_back(test_14_Input);
    testsExpected.push_back(test_14_Expected);
    testsUserPrograms.push_back(test_14_UserPrograms);
    testsHints.push_back(test_14_Hint);
    tesTimeLimits.push_back(test_14_time_limit);

    testsInputs.push_back(test_15_Input);
    testsExpected.push_back(test_15_Expected);
    testsUserPrograms.push_back(test_15_UserPrograms);
    testsHints.push_back(test_15_Hint);
    tesTimeLimits.push_back(test_15_time_limit);

    testsInputs.push_back(test_16_Input);
    testsExpected.push_back(test_16_Expected);
    testsUserPrograms.push_back(test_16_UserPrograms);
    testsHints.push_back(test_16_Hint);
    tesTimeLimits.push_back(test_16_time_limit);

    testsInputs.push_back(test_17_Input);
    testsExpected.push_back(test_17_Expected);
    testsUserPrograms.push_back(test_17_UserPrograms);
    testsHints.push_back(test_17_Hint);
    tesTimeLimits.push_back(test_17_time_limit);

    testsInputs.push_back(test_18_Input);
    testsExpected.push_back(test_18_Expected);
    testsUserPrograms.push_back(test_18_UserPrograms);
    testsHints.push_back(test_18_Hint);
    tesTimeLimits.push_back(test_18_time_limit);

    testsInputs.push_back(test_19_Input);
    testsExpected.push_back(test_19_Expected);
    testsUserPrograms.push_back(test_19_UserPrograms);
    testsHints.push_back(test_19_Hint);
    tesTimeLimits.push_back(test_19_time_limit);

    testsInputs.push_back(test_20_Input);
    testsExpected.push_back(test_20_Expected);
    testsUserPrograms.push_back(test_20_UserPrograms);
    testsHints.push_back(test_20_Hint);
    tesTimeLimits.push_back(test_20_time_limit);

    testsInputs.push_back(test_21_Input);
    testsExpected.push_back(test_21_Expected);
    testsUserPrograms.push_back(test_21_UserPrograms);
    testsHints.push_back(test_21_Hint);
    tesTimeLimits.push_back(test_21_time_limit);

    testsInputs.push_back(test_22_Input);
    testsExpected.push_back(test_22_Expected);
    testsUserPrograms.push_back(test_22_UserPrograms);
    testsHints.push_back(test_22_Hint);
    tesTimeLimits.push_back(test_22_time_limit);

    testsInputs.push_back(test_23_Input);
    testsExpected.push_back(test_23_Expected);
    testsUserPrograms.push_back(test_23_UserPrograms);
    testsHints.push_back(test_23_Hint);
    tesTimeLimits.push_back(test_23_time_limit);

    testsInputs.push_back(test_24_Input);
    testsExpected.push_back(test_24_Expected);
    testsUserPrograms.push_back(test_24_UserPrograms);
    testsHints.push_back(test_24_Hint);
    tesTimeLimits.push_back(test_24_time_limit);

    testsInputs.push_back(test_25_Input);
    testsExpected.push_back(test_25_Expected);
    testsUserPrograms.push_back(test_25_UserPrograms);
    testsHints.push_back(test_25_Hint);
    tesTimeLimits.push_back(test_25_time_limit);

    testsInputs.push_back(test_26_Input);
    testsExpected.push_back(test_26_Expected);
    testsUserPrograms.push_back(test_26_UserPrograms);
    testsHints.push_back(test_26_Hint);
    tesTimeLimits.push_back(test_26_time_limit);

    testsInputs.push_back(test_27_Input);
    testsExpected.push_back(test_27_Expected);
    testsUserPrograms.push_back(test_27_UserPrograms);
    testsHints.push_back(test_27_Hint);
    tesTimeLimits.push_back(test_27_time_limit);

    testsInputs.push_back(test_28_Input);
    testsExpected.push_back(test_28_Expected);
    testsUserPrograms.push_back(test_28_UserPrograms);
    testsHints.push_back(test_28_Hint);
    tesTimeLimits.push_back(test_28_time_limit);

    testsInputs.push_back(test_29_Input);
    testsExpected.push_back(test_29_Expected);
    testsUserPrograms.push_back(test_29_UserPrograms);
    testsHints.push_back(test_29_Hint);
    tesTimeLimits.push_back(test_29_time_limit);

    testsInputs.push_back(test_30_Input);
    testsExpected.push_back(test_30_Expected);
    testsUserPrograms.push_back(test_30_UserPrograms);
    testsHints.push_back(test_30_Hint);
    tesTimeLimits.push_back(test_30_time_limit);
}

/* ### Example of use ###

// Executing DEMO_TEST
void DEMO_TEST()
{
  // Initializing
  currentTestName = DEMO_TEST

  // Testing

  test(0,"got","expected");

  try
  {
    test(1,"maybe exception will be thrwon from here,"$$$ ASSERT_THROWN_EXCEPTIONS $$$");
  }
  catch (ExceptionType exp)
  {
    test("","$$$ DECLARE GOOD TEST $$$");
  }

}
*/

// Processing test
bool procceseTest(string testName,unsigned int testNumber){
    // Initializing 
    string testBaseFolder = "Tests/" + testName;
    string testFolder = "./Project_Test/" + testBaseFolder;
    string testInputFileName = "/testInput.txt";
    string testOutputFileName = "/testOutput.txt";
    string testMakefileErrorFileName = "/makefileError.txt";
    string testExecuteTestFileName = "/executeTest.sh";
    string testExecuteTestGDBFileName = "/executeTestGDB.sh";
    string testsExpectedFileName = "/testExpected.txt";
    int testCompletedFlag;

    // Setting test time limist 
    timeForSmallTest = tesTimeLimits.at(testNumber);
    
    // Creating XV6 Tests Folder
    createXV6_TestMakefile(testBaseFolder + "/Makefile",testsUserPrograms.at(testNumber));

    // Executing test
    string test_makefileCommand = "make --makefile=" + testFolder +  "/Makefile clean qemu";
    string test_command = "cd .. && " + test_makefileCommand + " -s < " + testFolder + testInputFileName;
    string got_test = GetStdoutFromCommandAsync(test_command,"Finished Yehonatan Peleg Test, quiting...",timeForSmallTest,testCompletedFlag);    
    
    // Cleaning after test
    string clean_makefileCommand = "make --makefile=" + testFolder +  "/Makefile clean";
    string clean_command = "cd .. && " + clean_makefileCommand;
    GetStdoutFromCommand(clean_command);

    // Creating test execute file
    std::ofstream outTestExecuteCommand(testBaseFolder + testExecuteTestFileName);
    outTestExecuteCommand << test_makefileCommand;
    outTestExecuteCommand.close();

    // Creating test execute GDB file
    std::ofstream outTestExecuteGDBCommand(testBaseFolder + testExecuteTestGDBFileName);
    outTestExecuteGDBCommand << test_makefileCommand + "-gdb";
    outTestExecuteGDBCommand.close();

    // Authorizing executing test execute file
    string authorizeExecuteTestFile_command = "chmod +x " + testBaseFolder + testExecuteTestFileName;
    GetStdoutFromCommand(authorizeExecuteTestFile_command);

    // Authorizing executing test execute GDB file
    string authorizeExecuteTestGDBFile_command = "chmod +x " + testBaseFolder + testExecuteTestGDBFileName;
    GetStdoutFromCommand(authorizeExecuteTestGDBFile_command);
    
    // Processing test output
    got_test = processTestOutput(got_test);

    // Writing test output to file
    std::ofstream out(testBaseFolder + testOutputFileName);
    out << got_test;
    out.close();

    // Retrieving test hint 
    string test_hint = testsHints.at(testNumber);

    // Asserting test completed
    if(testCompletedFlag == 1){
        // Retrieving test expected
        string expected_test = processCompareString(testsExpected.at(testNumber));

        if(expected_test.at(0) == '#'){
          if(got_test.find(expected_test.substr(1)) != std::string::npos){
              test(testNumber,"","$$$ DECLARE GOOD TEST $$$");
          }
          else{
              test(testNumber,got_test,"Output should have contain this: \n" + expected_test,vector<string>{"String value with /n",test_hint});
          }
          
        }
        else if(expected_test.at(0) == '+'){
            if(countSubStr(got_test,expected_test.substr(2)) == (expected_test.at(1) - '0')){
                test(testNumber,"","$$$ DECLARE GOOD TEST $$$");
            }
            else{
               test(testNumber,got_test,"Output should have contain this: \n" + expected_test.substr(2) + "\n" + 
                    expected_test.at(1) + " times",vector<string>{"String value with /n",test_hint});
            }
        }
        else{
            test(testNumber,got_test,expected_test,vector<string>{"String value with /n",test_hint});
        }
    }
    else if(testCompletedFlag == 0){
      // Creating Test Expected
      std::ofstream outError(testBaseFolder + testMakefileErrorFileName);
      outError << got_test;
      outError.close();

      // Declaring Test Execution Ended With Timeout
      test(testNumber,"$$$ SMALL TEST EXECUTION TIMED OUT $$$","",vector<string>{"",test_hint});
    }
    else if(testCompletedFlag == 2){
      // Declaring Test Execution Was Terminated By User
      test(testNumber,"$$$ USER TERMINATED TEST $$$","");
    }
    else{
      // Declaring Test Execution Ended With Error
      test(testNumber,"$$$ TEST EXECUTION ERROR $$$","");
    }

    return 0;
}

// Running specific test
void runSpecificTest(int testToExecute){
    // Initializing
    string test_name = "test_" + std::to_string(testToExecute);
  
    // Declaring specific test is running
    printf("Running ");
    printf("test_");
    printf("%d for specific test request\n\n",testToExecute);

    // Runnig specific test
    procceseTest(test_name,testToExecute);
}

// Creating Tests
unsigned int CreateTests()
{
  // Creating Tests Folder
  string createTestFolderCommand = "rm -rf Tests && mkdir Tests";
  string got_createTestFolder = GetStdoutFromCommand(createTestFolderCommand);

   // Creating Tests
  for(unsigned int i = 0;i < testsInputs.size();i++)
  { 
    // Retrieving current test to create
    string currentTestInput = testsInputs.at(i);
    string currentTestExpected = testsExpected.at(i);

    // Defining Test Folder
    string testFolder = "./Tests/test_" + std::to_string(i);
    string testFile = testFolder + "/" + "testInput" + ".txt";
    string testExpectedFile = testFolder + "/" + "testExpected" + ".txt";

    // Creating Current Test Files  
    string createTestsFilesCommand = "mkdir " + testFolder + " && touch " + testFile;
    string got_createTestsFilesCommand  = GetStdoutFromCommand(createTestsFilesCommand );

    // Creating Test Input
    std::ofstream outInput(testFile);
    outInput << currentTestInput + "\nquitXV6\n";
    outInput.close();

    // Creating Test Expected
    std::ofstream outExpected(testExpectedFile);
    outExpected << processCompareString(currentTestExpected);
    outExpected.close();
  }

  return testsInputs.size();
}

// Signal handler for SIGINT
void sigintHandler(int num){
    // Declaring execution should be aborted
    abortExecution = 1;
    
    // Catching signal
    signal(SIGINT,sigintHandler);
}

// Signal handler for SIGQUIT
void sigquitHandler(int num){
    // Catching signal
    signal(SIGQUIT,sigquitHandler);
}

// Processing test output
string processTestOutput(string output){
  // Serching for start of test code
  unsigned int inputStartPos = output.find("$",0);

  // Returning test code
  if(inputStartPos < output.length() && inputStartPos >= 0){
      return processCompareString(output.substr(inputStartPos,output.length())); 
  }
  else{
      return output;
  }
}

// Processing compare string, i.e removing spaces from edges
string processCompareString(string str){
    // Initializing
    int start = -1;
    int end = -1;

    // Retrieving location of first space from start
    for(unsigned int i = 0;i < str.length();i++){
      if(str.at(i) > 32){
        start = i;
        break;
      }
    }

    // Retrieving location of first space from end
    for(unsigned int i = str.length() - 1;i >= 0;i--){
      if(str.at(i) > 32){
        end = i;
        break;
      }
    }

    // Asserting there are spaces at the edegs and if so removing them
    if(start == -1 || end == -1){
      return str;
    }
    else{
      return str.substr(start,end - start + 1);
    }

}

// Creating XV6 Tests Makefile
void createXV6_TestMakefile(string makefilePath,vector<string> userspacePrograms){
    // Initializing
    string data;
    FILE * stream = fopen("../Makefile","r");
    const int max_buffer = 4000;
    char buffer[max_buffer];
    vector<string> defaultUserspacePrograms;
    string xv6TestsBaseFolder = "XV6_Tests/";
    string xv6TestsQEMUFolder = "./Project_Test/" + xv6TestsBaseFolder;

    // Creating default user space programs
    defaultUserspacePrograms = {"sh", "init","ls", "echo", "cat"};

    // Reading project makefile and adding tests user space programs
    // while code is for telling the makefile to include these user space programs
    while (!feof(stream))
    { 
      // Reading line of test output
      if (fgets(buffer, max_buffer, stream) != NULL){
            if(strstr(buffer,"UPROGS=\\")){
              data.append(buffer);
              eraseUserPrograms(data, stream);
              data.append(processAppendMakefileUPROGS(userspacePrograms));
              data.append(processAppendMakefileUPROGS(defaultUserspacePrograms));
              data.append("\n");
            }
            else{
              data.append(buffer);
            }
      }
     
    }

    // Adding build commands for each user space program
    addUserProgramsBuildCommands(data, userspacePrograms, xv6TestsQEMUFolder);
    addUserProgramsBuildCommands(data, defaultUserspacePrograms, "");
    
    // Creating Tests Makefile
    std::ofstream outMakefile(makefilePath);
    outMakefile << data;
    outMakefile.close();
}

// Processing user space programs and creating an entry for each one 
// inorder to take the makefile of the xv6 to include them
string processAppendMakefileUPROGS(vector<string> append){
    // Initializinh
    string result;

    // Creating entries
    for (vector<string>::iterator it = append.begin() ; it != append.end(); ++it){
      result.append("\t_" + *it + "\\\n");
    }

    return result;
}

// Erasing user space programs
void eraseUserPrograms(string &data, FILE * stream){
    // Initializing
    const int max_buffer = 4000;
    char buffer[max_buffer];

     // Erasing user space programs
     while (!feof(stream))
    {  
        // Reading line from stream and asserting if its a user program
      if (fgets(buffer, max_buffer, stream) != NULL){
            // If its not a user program, appending to buffer and exiting
            if(!(strstr(buffer,"\\") && strstr(buffer,"\n"))){
                break;
            }
      }
    }
}

// Adding build command for user programs
void addUserProgramsBuildCommands(string &data, vector<string> &userspacePrograms, string folder){
    // Initializing
    string userSpaceProgramMakeCode =  
    "_%: $(ULIB)\n" 
        "\tgcc -fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie -c -o %.o #.c\n"
        "\tld -m elf_i386 -N -e main -Ttext 0 -o _% %.o $(ULIB)\n" 
        "\tobjdump -S _% > %.asm\n" 
        "\t$(OBJDUMP) -t _% | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > %.sym\n";

    // Adding build command for user programs
    for (vector<string>::iterator it = userspacePrograms.begin() ; it != userspacePrograms.end(); ++it){
      string temp = replaceInString(userSpaceProgramMakeCode,'%',*it) + "\n";
      string temp2 = replaceInString(temp,'#',folder + *it);
      data.append(temp2);
    }
}

// Replacing all occurences of toReplace char with replaceWith string in str
string replaceInString(string str,char toReplace,string replaceWith){
    // Initializing
    string result;

    // Replacing
    for (string::iterator it=str.begin(); it!=str.end(); ++it){
        if(*it == toReplace){
          result.append(replaceWith);
        }
        else{
          result.append(1,*it);
        }
    }

    return result;
}

// Finding number of occurences of substr in string
int countSubStr(string str,string findSubStr){
    // Initializing
    int occurrences = 0;
    string::size_type pos = 0;

    // Counting
    while ((pos = str.find(findSubStr, pos)) != std::string::npos) {
          ++occurrences;
          pos += findSubStr.length();
    }
   
   return occurrences;
}

// Executing Operating_System_Test
void Operating_System_Test()
{
  // Initializing
  currentTestName = "Operating_System_Test";
  char arr[50];
  memset(arr,' ',50);
  arr[50] = 0;
  int progress_index = 0;
  int progress;
  const char* no_error_progress = "\r\e[38;5;082m[%s]\e[38;5;226m%i%% %d/%d\r\e[0m";
  const char* yes_error_progress = "\r\e[38;5;082m[%s]\e[38;5;196m%i%% %d/%d\r\e[0m";

  // Creating Tests
  unsigned int numberOfTests = CreateTests();
  
  // Running specific test if demanded
  if(0 <=  testToExecute && ((unsigned int)testToExecute) < numberOfTests){
      runSpecificTest(testToExecute);
      return;
  }
  else if(testToExecute != -1){
      printf("Specific test request was out of bounds(%d)\n\n",testToExecute);
  }

  // Printing initial progress
  printf(no_error_progress,arr,0,0,numberOfTests);
  fflush(stdout);

  // Testing
  for(unsigned int i = 0;i < numberOfTests;i++)
  { 
    // Testing
    if(abortExecution == 0){
      procceseTest("test_" + std::to_string(i),i);
    }
    else{
      // Declaring test was aborted
      cout << RED << std::endl << "Operating System Test Was Aborted With " << i << " Tests Executed Out Of " << numberOfTests << " !!!" << RESET << std::endl;
      red = red + (numberOfTests - i);
      break;
    }

    float float_index = (float)(i + 1);
    progress = (float_index/numberOfTests) * 100;

    // Updating progress 
    if(progress > progress_index)
    { 
      progress_index += 1;
      memset(arr,'#',(int)((float_index/numberOfTests) * 50));
    }
    
    if(red == 0){
      printf(no_error_progress,arr,progress,i+1,numberOfTests);
    }
    else{
      printf(yes_error_progress,arr,progress,i+1,numberOfTests);
    }
    fflush(stdout);
  }
  
  // Cleaning after progress bar
  printf("\r                                                                                   \r");
}
