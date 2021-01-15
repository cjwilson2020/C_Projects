///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Title:            p6: Signal Handling
// Files:            intdate.c sendsig.c division.c
// Semester:         CS 354 Fall 2019
//
// Author:           Christopher J. Wilson
// Email:            cwilson32@wisc.edu
// CS Login:         christopherw
// Lecturer's Name:  Jim Skrentny
// Lab Section:      001
//
//////////////////// PAIR PROGRAMMERS COMPLETE THIS SECTION ////////////////////
//
//                   CHECK ASSIGNMENT PAGE TO see IF PAIR-PROGRAMMING IS ALLOWED
//                   If pair programming is allowed:
//                   1. Read PAIR-PROGRAMMING policy (in cs302 policy)
//                   2. choose a partner wisely
//                   3. REGISTER THE TEAM BEFORE YOU WORK TOGETHER
//                      a. one partner creates the team
//                      b. the other partner must join the team
//                   4. complete this section for each program file.
//
// Pair Partner:     (name of your pair programming partner)
// Email:            (email address of your programming partner)
// CS Login:         (partner's login name)
// Lecturer's Name:  (name of your partner's lecturer)
// Lab Section:      (your partner's lab section number)
//
//////////////////// STUDENTS WHO GET HELP FROM OTHER THAN THEIR PARTNER //////
//                   must fully acknowledge and credit those sources of help.
//                   Instructors and TAs do not have to be credited here,
//                   but tutors, roommates, relatives, strangers, etc do.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//////////////////////////// 80 columns wide //////////////////////////////////

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//This program will handle 3 signals:
// - a periodic signal from an alarm -
// - a keyboard interrupt signal;
// - a user defined signal

int numSec = 3;
int sigu1_hits = 0;

/**
 * This handler function should print the pid (process id)
 *  of the program and the current time (in the same format
 *  as the Linux date command). It should also re-arm the
 *  alarm to go off again three seconds later, and then return
 *  back to the main function, which continues its infinite loop.
 * */
void a_handler(int sig) 
{
	time_t now = time(NULL);
	char *currTime = ctime(&now);

        printf("PID: %d | Current Time: %s", getpid(), currTime);
	struct tm *local = localtime(&now);
        int seconds = local->tm_sec;
        if ((seconds % 4) == 0){
                kill(getpid(), SIGUSR1);
        }

	alarm (numSec);

}
void u1_handler(int sig){
	sigu1_hits++;
	printf("SIGUSR1 caught!\n");
}
void inter_handler(int sig){
        printf("SIGINT received.\nSIGUSR1 was received %d times. Exiting now.\n", sigu1_hits);
	exit(0);
}

int main()
{
	/*	- FIRST: sets up an alarm that will go off 3 seconds later,
			causing a SIGALRM signal to be sent to the program */
	alarm(numSec); //alarm set up for SIGALRM in 3 seconds
	
	/*      - SECOND: it registers a signal handler to handle the SIGALRM
                      signal so that signal can be received by the program
           	- The signal handler is just another function you need
                      to write inside your program */
	struct sigaction alarm_act; 
	memset (&alarm_act, 0, sizeof(alarm_act));
	alarm_act.sa_handler = &a_handler;	//Install handler
	sigaction(SIGALRM, &alarm_act, NULL);
	
	struct sigaction user_act;
        memset (&user_act, 0, sizeof(user_act));
        user_act.sa_handler = &u1_handler;    //Install handler
        sigaction(SIGUSR1, &user_act, NULL);
	
	struct sigaction stat_act;
        memset (&stat_act, 0, sizeof(stat_act));
        stat_act.sa_handler = &inter_handler;    //Install handler
        sigaction(SIGINT, &stat_act, NULL);

	printf("Pid and time will be printed every 3 seconds.\n");
	printf("Enter ^C to end the program.\n");
	
	
	while (1){	
	}

	exit(0);
}
