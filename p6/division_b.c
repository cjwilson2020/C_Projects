///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Title:            p6: Signal Handling
// Files:            intdate.c sendsig.c division.c
// Semester:         CS 354 Fall 2019
//
// Author:           (your name)
// Email:            (your email address)
// CS Login:         (your login name)
// Lecturer's Name:  (name of your lecturer)
// Lab Section:      (your lab section number)
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

char input[100];
int successes = 0;
void handler(int sig){
	printf("Tried to divide by 0.\nSuccessful divisons: %d\n", successes);
	exit(0);
}

int main(int argc, char *argv[]){

	struct sigaction action;
	memset (&action, 0, sizeof(action));
	act.sa_handler = &handler;
	sigaction(SIGFPE, &action , NULL);

	while(1){
		printf("Enter first integer: ");
		int int1 = atoi(fgets(input, 100, stdin));
		printf("Enter second integer: ");
	        int int2 = atoi(fgets(input, 100, stdin));

		printf("%d / %d is %d with a remainder of %d\n", int1, int2, int1/int2, int1%int2);
		successes++;
	}
}


