////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:      board1.txt board2.txt board3.txt board4.txt
// Semester:         CS 354 Fall 2019
//
// Author:           Christopher Wilson
// Email:            cwilson32@wisc.edu
// CS Login:         christopherw
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>     

char *COMMA = ",";  

/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size)
{      
    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, fp) == -1)
    {
        printf("Error in reading the file\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}   


/* 
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size)
{
    //Algorithm
    //    Iterate through the board
    // 	  Count the number of X's and O's (1's amd 2's respectively)
    // 	      if ((total number of X  -  total number of O's) > 1)
    // 	          return 0;
    // 	      for each row, col, and both diagonals of length n
    // 	          if 
    // 	              there are n X's, X wins
    // 	          else if 
    //		      there are n O's, O wins
    // 	     
    // 	      If both X and O wins, 
    // 	          return 0. There can only be one winner or none
    // 	      
    int numX = 0; //number of X's on board
    int numO = 0; //number of O's on board
    int xWins = 0; //variable for recording if X wins by getting n in a row
    int oWins = 0; //variable for recording if O wins by getting n in a row

    for (int i = 0; i < size; i++)
    {
	int rowX = 0;
	int rowO = 0;
	for (int j = 0; j < size; j++)
	{
	    if (*(*(board + i) + j) == 1)
	    {
		numX++;
		rowX++;
	    }
	    else if(*(*(board + i) + j) == 2)
	    {
		numO++;
		rowO++;
	    }
	}
	if(rowX == size)
	{
		xWins = 1;
	}
	if(rowO == size)
	{
		oWins = 1;
	}
    }

    //Check each column for n number of X's or O's
    for (int j = 0; j < size; j++)
    {
        int colX = 0;
        int colO = 0;
        for (int i = 0; i < size; i++)
	{
            if (*(*(board + i) + j) == 1)
	    {
                colX++;
            }else if(*(*(board + i) + j) == 2)
	    {
                colO++;
            }
        }
        if(colX == size)
	{
                xWins = 1;
        }
        if(colO == size)
	{
                oWins = 1;
        }
    }
    
    //Diagonal from top-left to bottom-right 
    int diagX = 0;
    int diagO = 0;
    for (int i = 0; i < size; i++)
    {
	if (*(*(board + i) + i) == 1 )
	{
		diagX++;
	}else if (*(*(board + i) + i) == 2 )
	{
                diagO++;
        }
    }
    if (diagX == size)
    {
	xWins = 1;
    } else if (diagO == size)
    {
	oWins = 1;
    }


    //Diagonal from top-right to bottom-left
    int rdiagX = 0;
    int rdiagO = 0;
    for (int i = 0; i < size; i++)
    {
        if (*(*(board + i) + (size - i)) == 1 )
	{
                rdiagX++;
        }else if (*(*(board + i) + (size - i)) == 2 )
	{
                rdiagO++;
        }
    }
    if (rdiagX == size)
    {
        xWins = 1;
    } else if (rdiagO == size)
    {
        oWins = 1;
    }


    if ((numX - numO) > 1)
    {
	return 0;
    }

    if ((xWins == 1) && (oWins == 1))
    {
	return 0; 
    } 
    else
    {
	return 1; 
    }


    return 0;   
}     


/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[])
{              	
    //Check if number of command-line arguments is correct.
    if (argc < 2)
    {
	    printf("missing argument\n");
	    exit(-1);
    }

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL)
    {
        printf("Cannot open file for reading\n");
        exit(1);
    }
	
	
    //Declare local variables.
    int size;
    int *sizep = &size;

    //Call get_dimensions to retrieve the board size.
    get_dimensions(fp, sizep);	
    

    //Dynamically allocate a 2D array of dimensions retrieved above.
    int **arr = (int **)malloc(*sizep * sizeof(int *)); //Create an integer pointer array of int pointer arrays of size n
    for (int i = 0; i < *sizep; i++) //for each int pointer array, allocate memory of size n
    {
	*(arr + i) = (int *)malloc(*sizep * sizeof(int));
    }

    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < *sizep; i++)
    {
        if (getline(&line, &len, fp) == -1)
	{
            printf("Error while reading the file\n");
            exit(1);
        }

        token = strtok(line, COMMA);
        for (int j = 0; j < *sizep; j++)
	{
            //Complete the line of code below
            //to initialize your 2D array.
            *(*(arr + i) + j) = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }

    //Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    int val = n_in_a_row(arr, *sizep);
    if (val == 1)
    {
	printf("valid\n");
    }
    else
    {
	printf("invalid\n");
    }
 
	
    //Free all dynamically allocated memory.
    for(int i = 0; i < *sizep; i++)
    {
	free(*(arr + i)); *(arr + i) = NULL;
    }
    
    free(arr); arr = NULL;

    //Close the file.
    if (fclose(fp) != 0)
    {
        printf("Error while closing the file\n");
        exit(1);
    } 

    return 0;       
}       

