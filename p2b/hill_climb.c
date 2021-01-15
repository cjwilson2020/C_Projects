///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
// Main File:        hill_climb.c
// This File:        hill_climb.c
// Other Files:      hill1.txt hill2.txt hill3.txt hill4.txt
// Semester:         CS 354 Fall 2019
//
// Author:           Christopher Wilson
// Email:            cwilson32@wisc.edu
// CS Login:         christopherw
//
/////////////////////////// OTHER SOURCES OF HELP /////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>   
#include <stdlib.h>     
#include <string.h>  

/*
 * USE THIS:
 * Structure representing the Matrix
 * numRows: number of rows
 * numCols: number of columns
 * matrix: 2D heap allocated array of integers
 */
typedef struct {    
    int numRows;
    int numCols;
    int **matrix;
} Matrix;

/* Get the dimensions of the matrix that are on
 * the first line of the input file.
 * 
 * infp: file pointer for input file
 * nRows: pointer to number of rows in the matrix   
 * nCols: pointer to number of columns in the matrix
 */
void get_dimensions(FILE *infp, int *nRows, int *nCols) {         
    // Read the dimensions on the first line from infp
    char *line = NULL; //Need a string variable to stor the line
    size_t len = 0;
    if(getline(&line, &len, infp) == -1) { //Take a Peak at the file's line to be read
        printf("Error in reading file\n"); //If there's nothing, exit
	exit(1);
    }
	
    char *token = NULL; //Need a string variable to store value of the line
    
    token = strtok(line, ","); 
    *nRows = atoi(token);
    
    token = strtok(NULL, ",");
    *nCols = atoi(token);

    free(line);//Free line pointer
}            

/* Continually find and print the largest neighbor
 * that is also larger than the current position
 * until nothing larger is found.       
 * 
 * outfp: file pointer for output file  
 * mstruct: pointer to the Matrix structure
 */
void hill_climb(FILE *outfp, Matrix *mstruct) {  
    int maxX = mstruct->numRows;
    int maxY = mstruct->numCols;
    int largestNeighbor =  *(*(mstruct->matrix + 0) + 0);
    int canLoop = 1;
    typedef struct{
	int x;
	int y;	
    } Position; 
    
    Position pos;
    pos.x = 0;
    pos.y = 0;

    // Write the intial starting number to outfp
    
    fprintf(outfp, "%d", largestNeighbor);
	
    // Starting at this position (0, 0)
    // Find the largest neighbor that is also larger than the current position (x,y),
    // if one exists
    //
    // Algorithm:
    //     r = 0
    //     c = 0
    //
    //   check up to 8 neighbors
    //
    // (pos.x, pos.y) is the position of the current element in the matrix
 
    while (canLoop == 1) { //while this element is smaller than it's largest neighbor

	Position maxPos = {pos.x, pos.y};
	largestNeighbor = *(*(mstruct->matrix + maxPos.x) + maxPos.y);
    	if (pos.x == 0) { //x == 0
   	    if (pos.y == 0) { //y == 0
	
                //middle right
 	        if ( *(*(mstruct->matrix + pos.x) + pos.y + 1) > *(*(mstruct->matrix + pos.x) + pos.y)){
                    if(*(*(mstruct->matrix + pos.x) + pos.y + 1) > largestNeighbor){
                        largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y + 1) ;
                        maxPos.x = pos.x;
                        maxPos.y = pos.y + 1;
                    }
                }
           	
                for(int i = 0; i < 2; i++){ //lower center, right
		    if ( *(*(mstruct->matrix + pos.x + 1) + pos.y + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                       if(*(*(mstruct->matrix + pos.x + 1) + pos.y + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x + 1) + pos.y + i) ;
		            
	    		    maxPos.x = pos.x + 1;
                            maxPos.y = pos.y + i;
                        }
                    }
                }
		
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
		    canLoop = 0;
		}
	  } else if ((0 < pos.y) && (pos.y < maxY - 1)) { //x == 0, 0 < y < max
		

                for(int i = 0; i < 2; i++){ //middle left and right
		    if ( *(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) ;
			    maxPos.x = pos.x;
			    maxPos.y = pos.y - 1 + (2*i);
                        }
                    }
                }
                for(int i = 0; i < 3; i++){ //lower left, center, right
		    if (  *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) ;
			    maxPos.x = pos.x + 1;
			    maxPos.y = pos.y - 1 + i;
                        }
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
                
            } else { //pos.y must be at maxY //x == 0, y == max
	    	

                //check middle left
                if ( *(*(mstruct->matrix + pos.x) + pos.y - 1) > *(*(mstruct->matrix + pos.x) + pos.y)){
                    if(*(*(mstruct->matrix + pos.x) + pos.y - 1) > largestNeighbor){
                        largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y - 1) ;
                        maxPos.x = pos.x;
                        maxPos.y = pos.y - 1;
                    }
                }
                
                for(int i = 0; i < 2; i++){ //lower left, center
                    if (  *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) ;
                            maxPos.x = pos.x + 1;
                            maxPos.y = pos.y - 1 + i;
                        }
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
         	    }
	
        }else if ((0 < pos.x) && (pos.x < maxX - 1)) { //0 < x < max
	    if (pos.y == 0) { //y == 0
		

                for(int i = 0; i < 2; i++){ //top center and right
                    if (  *(*(mstruct->matrix + pos.x - 1) + pos.y + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x - 1) + pos.y + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x - 1) + pos.y + i) ;
                            maxPos.x = pos.x - 1;
                            maxPos.y = pos.y + i;
                        }
                    }
                }
                for(int i = 1; i < 2; i++){ //middle left and right
                    if ( *(*(mstruct->matrix + pos.x) + pos.y + i) > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x) + pos.y + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y + i) ;
                            maxPos.x = pos.x;
                            maxPos.y = pos.y + i;
                        }
                    }
                }
                for(int i = 0; i < 2; i++){ //lower left, center, right
                    if (  *(*(mstruct->matrix + pos.x + 1) + pos.y + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x + 1) + pos.y + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x + 1) + pos.y + i) ;
                            maxPos.x = pos.x + 1;
                            maxPos.y = pos.y + i;
                        }
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
	    } else if ((0 < pos.y) && (pos.y < maxY - 1)) { //0 < x < max, 0 < y < max /---------MOST COMMON-------/
		

		for(int i = 0; i < 3; i++){ //top 3 elements
		    if (  *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
			if(*(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) > largestNeighbor){
			    largestNeighbor = *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) ;
			    maxPos.x = pos.x - 1;
			    maxPos.y = pos.y - 1 + i;
		    	}
		    }
                }
		for(int i = 0; i < 2; i++){ //middle left and right
		    if ( *(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) > *(*(mstruct->matrix + pos.x) + pos.y)){
                     	if(*(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) > largestNeighbor){
               		    largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) ;
			    maxPos.x = pos.x;
			    maxPos.y = pos.y - 1 + (2*i);
                        }
                    }
                }
                for(int i = 0; i < 3; i++){ //lower left, center, right
		    if (  *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) ;
			    maxPos.x = pos.x + 1;
			    maxPos.y = pos.y - 1 + i;
                        }
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
           } else { //pos.y must be at maxY //0 < x < max, y == max
	    	

                for(int i = 0; i < 2; i++){ //top left, center
                    if (  *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) ;
                            maxPos.x = pos.x - 1;
                            maxPos.y = pos.y - 1 + i;
                        }
                    }
                }
                //check middle left
                if ( *(*(mstruct->matrix + pos.x) + pos.y - 1) > *(*(mstruct->matrix + pos.x) + pos.y)){
                    if(*(*(mstruct->matrix + pos.x) + pos.y - 1) > largestNeighbor){
                        largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y - 1) ;
                        maxPos.x = pos.x;
                        maxPos.y = pos.y - 1;
                    }
                }
                
                for(int i = 0; i < 2; i++){ //lower left, center
                    if (  *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x + 1) + pos.y - 1 + i) ;
                            maxPos.x = pos.x + 1;
                            maxPos.y = pos.y - 1 + i;
                        }
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
   	    }
        } else { //pos.x must be at maxX
    	    if (pos.y == 0) { //y == 0
		
                for(int i = 0; i < 2; i++){ //top center, right 
                    if (  *(*(mstruct->matrix + pos.x - 1) + pos.y + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x - 1) + pos.y + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x - 1) + pos.y + i) ;
                            maxPos.x = pos.x - 1;
                            maxPos.y = pos.y + i;
                        }
                    }
                }
                
		//middle right
                if ( *(*(mstruct->matrix + pos.x) + pos.y + 1) > *(*(mstruct->matrix + pos.x) + pos.y)){
                    if(*(*(mstruct->matrix + pos.x) + pos.y + 1) > largestNeighbor){
                        largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y + 1) ;
                        maxPos.x = pos.x;
                        maxPos.y = pos.y + 1;
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
               
            } else if ((0 < pos.y) && (pos.y < maxY - 1)) { //x = max, 0 < y < max   
		
		
		for(int i = 0; i < 3; i++){ //top 3 elements
		    if (  *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
			if(*(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) > largestNeighbor){
			    largestNeighbor = *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) ;
			    maxPos.x = pos.x - 1;
			    maxPos.y = pos.y - 1 + i;
		    	}
		    }
                }
                for(int i = 0; i < 2; i++){ //middle left and right
		    if ( *(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y - 1 + (2*i)) ;
			    maxPos.x = pos.x;
			    maxPos.y = pos.y - 1 + (2*i);
                        }
                    }
                }
                
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
		
            } else { //pos.y must be at maxY //x = max, y == max
                

                for(int i = 0; i < 2; i++){ //top left, center
                    if (  *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i)  > *(*(mstruct->matrix + pos.x) + pos.y)){
                        if(*(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) > largestNeighbor){
                            largestNeighbor = *(*(mstruct->matrix + pos.x - 1) + pos.y - 1 + i) ;
                            maxPos.x = pos.x - 1;
                            maxPos.y = pos.y - 1 + i;
                        }
                   }
                }
                //check middle lefti
                if ( *(*(mstruct->matrix + pos.x) + pos.y - 1) > *(*(mstruct->matrix + pos.x) + pos.y)){
                    if(*(*(mstruct->matrix + pos.x) + pos.y - 1) > largestNeighbor){
                        largestNeighbor = *(*(mstruct->matrix + pos.x) + pos.y - 1) ;
                        maxPos.x = pos.x;
                        maxPos.y = pos.y - 1;
                    }
                }
		if ( *(*(mstruct->matrix + pos.x) + pos.y) == *(*(mstruct->matrix + maxPos.x) + maxPos.y)){
                    canLoop = 0;
                }
       	    }
        }
	 
	if (largestNeighbor != *(*(mstruct->matrix + pos.x) + pos.y)){
		pos.x = maxPos.x;
		pos.y = maxPos.y;
		fprintf(outfp, " %d", largestNeighbor);
	}
        
	//Load all neighbors into array
    
        //traverse array, if an element in neighbor
        //array is greater than currN, set currN and maxN to that element

        // Move to that position  
    
        // Write that number to outfp
    
        // Repeat until you can't find anything larger (you may use any loop)
    }
    fprintf(outfp, "\n");

}    

/* This program reads an m by n matrix from the input file
 * and outputs to a file the path determined from the
 * hill climb algorithm.    
 *
 * argc: CLA count
 * argv: CLA values
 */
int main(int argc, char *argv[]) {
    // Check if number of command-line arguments is correct
    if(argc < 3) { //One for the exe, in, out
	printf("missing argument");
	exit(-1);
    }

    // Open the input file and check if it opened successfully
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
	printf("Cannot open file for reading\n");
	exit(1);
    }

    // Declare local variables including the Matrix structure 
    int *cols = malloc(sizeof(int));
    int *rows = malloc(sizeof(int));  
    Matrix *m = malloc(sizeof(Matrix)); //Should be size of 2 ints (2*4 Bytes = 8 Bytes)
   					//And an int** (4 Bytes) 
					//8 Bytes + 4 Bytes = 12 bytes
	
    // Call the function get_dimensions
    get_dimensions(fp, rows, cols);
    m->numRows = *rows;
    m->numCols = *cols;
    m->matrix = malloc(sizeof(int *) * *rows);
    for (int i = 0; i < *rows; i++){
	 *(m->matrix + i) = malloc(sizeof(int) * *cols);
    }

    // Read the file line by line base on the number of rows
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;

    // Tokenize each line wrt comma to store the values in the matrix
    for (int i = 0; i < *rows; i++) {
	if (getline(&line, &len, fp) == -1) {
	    printf("Error while reading the file\n");
	    exit(1);
	}

	token = strtok(line, ",");
	for (int j = 0; j < *cols; j++) {
		*(*(m->matrix + i) + j) = atoi(token);
		token = strtok(NULL, ",");
	}
    }
 
    // Open the output file  
    FILE *outf = fopen(*(argv + 2), "w");
    if(outf == NULL) {
        printf("Cannot open file for writing\n");
	exit(1);
    }
	
    // Call the function hill_climb
    hill_climb(outf, m);
    
    // Free the dynamically allocated memory
    for (int i = 0; i < *rows; i++){
	free(*(m->matrix + i));
    }    

    free(m->matrix);
    free(m);
    free(rows);
    free(cols);

    // Close the input file  
    fclose(fp);

    // Close the output file
    fclose(outf);
    return 0; 
}   
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
