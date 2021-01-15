#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct block_header {
        int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} block_header;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */

block_header *start_block = NULL;

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use BEST-FIT PLACEMENT POLICY to find the block closest to the required block size
 * - Use SPLITTING to
 *   divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic.
 */
void* Alloc_Mem(int size){

	/* --------- ERROR CHECK ------------*/

	// Check size - Return NULL if not positive or if larger than heap space.
	if (size < 1){
		return NULL;
	} 


	/* ------- FINDING SIZE OF BLOCK NEEDED ------- */

	// Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
	int b_size = size + 4; 
	int padding = 0;
	if (b_size % 8 != 0){ 
		padding = 8 - (b_size % 8);
	}
	b_size = b_size + padding; 
	// Now b_size is a multiple of 8 including header, payload, and padding (if any) 
	// [-SIZE/STATUS-|---------PAYLOAD----|PADDING|-FOOTER-]
	// |-- 4 BYTES --|--------- [int size] BYTES ----------]

			
	/* ----- Use BEST-FIT PLACEMENT POLICY ----- */
        
	block_header* ptr = start_block;      // Point to start_block's header

	block_header* best = NULL;
	
	int ptr_blk_size = 0;
	int ptr_blk_status = 0;
        int best_blk_size = 0;	
	
	while(ptr->size_status != 1){
		
		ptr_blk_status = ptr->size_status % 8;
		ptr_blk_size = ptr->size_status - ptr_blk_status;
	
		// Check what the block_header's size_status is
		if ((ptr_blk_status == 0) || (ptr_blk_status == 2)){ // Check if this block is free
			if(ptr_blk_size >= b_size){ // Check if this block is big enough for "b_size"
				if(best == NULL){ //best == NULL
					best = ptr;
					best_blk_size = ptr_blk_size;
				}else if((best->size_status % 8) == (1 || 3)){ //best hasn't been updated
					best = ptr;
					best_blk_size = ptr_blk_size;

				}else if(ptr_blk_size < best_blk_size){ // If this block is smaller than "best"
					best = ptr;	
					best_blk_size = ptr_blk_size;
				}else if (ptr_blk_size == best_blk_size){ // If current block == "best" block
					if (ptr < best){
						best = ptr;
						best_blk_size = ptr_blk_size;
					}
				}
			}
		}
	
	 	// Point to next block
		ptr = ptr + (ptr_blk_size/4); 
	}
	
    	
	
	/* -------- Use SPLITTING to divide the chosen block into two blocks -------- */
	// - Check if best == NULL
	if(best != NULL){

		int diff = best_blk_size - b_size;
	
		if (diff == 0){ // If no splitting is necessary`, return best payload
			best->size_status++;
			
			return (void *)(best + 1);
		}else if (diff > 0){ //SPLIT
			int split_size = diff;

			// Record where "best" block ends with a new block_header of size "diff"
			block_header *split = best + ((best_blk_size - split_size)/4);


			split->size_status = split_size + 2; //signifies that previous block is allocated

			block_header *split_footer = split + (split_size/4) - 1; 
			split_footer->size_status = split_size;		

			// Change "best" header integer value by substracting 'diff' from it
			best->size_status = best->size_status - diff;
			best->size_status++;

			return (void *)(best + 1);
		}
	}
	return NULL;
}

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int Free_Mem(void *ptr) {
		// Return -1 if ptr is NULL.	
	if (ptr != NULL){
	
		block_header *ptr_head = (block_header *)(ptr-4); //ptr is of type void*, increments by 1
	
		int ptr_status = ptr_head->size_status % 8;
		int ptr_size = ptr_head->size_status - ptr_status;
	
		if( ptr_status == 1 || 3){ //if This block is allocated
			ptr_head->size_status--; // mark it as FREE
			ptr_status--; //mark it as FREE
	
			block_header *ptr_foot = ptr_head - 1 + (ptr_size/4); //Make a footer at the last 4 bytes of block
			ptr_foot->size_status = ptr_size; //set footer to size of block
			
			//update next block
			block_header * next_head = ptr_foot + 1;
	
			if(((next_head->size_status % 8) == 3) || ((next_head->size_status % 8) == 2)){
				next_head->size_status -= 2;// show that current block is free
			}
			
			//Coalesce
			// - Check previous block unitl you run into one that is allocated or NULL
			int prev_size = 0;
	
			while(ptr_status != 2){	//As long as the previous block is free
					// Go to prev_foot					
					block_header *prev_foot = ptr_head - 1;
					
					// Get its value 
					prev_size = prev_foot->size_status;
			
					//Go to previous block header 
					block_header *prev_head =  ptr_head - (prev_size/4);
	
					//Add ptr_size to prev_size and head
					prev_head->size_status += ptr_size;
					prev_size += ptr_size;
	
					//point ptr_head to prev_head
					ptr_head = prev_head;
					ptr = ptr_head + 1;
					ptr_status = ptr_head->size_status % 8;
					ptr_size = prev_size;
					ptr_foot->size_status = ptr_size;
					
			}
			
			// - Check next blocks until you reach an allocateed one or its "size_status" == 1
			block_header *curr = next_head;
			while((curr->size_status % 8) != 1){ //While not at end of heap or while free
				int curr_size = curr->size_status - (curr->size_status % 8); //Get size of curr block
	
				if((curr->size_status % 8) == 0){ //If next block is free
					// Combine sizes
					ptr_size = ptr_size + curr_size;
						
					ptr_head->size_status = ptr_head->size_status + curr_size;
	
					// Change footer
					ptr_foot = ptr_head + (ptr_size/4) - 1;
					ptr_foot->size_status = ptr_size;
				}
				curr += (curr_size/4);
			}
			return 0;  
		}
	} 
	return -1;



}

/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int Init_Mem(int sizeOfRegion) {         
    int pagesize;
    int padsize;
    int fd;
    int alloc_size;
    void* space_ptr;
    block_header* end_mark;
    static int allocated_once = 0;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: Init_Mem has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
                    fd, 0);
    if (MAP_FAILED == space_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // To begin with there is only one big free block
    // initialize heap so that start block meets 
    // double word alignement requirement
    start_block = (block_header*) space_ptr + 1;
    end_mark = (block_header*)((void*)start_block + alloc_size);
  
    // Setting up the header
    start_block->size_status = alloc_size;

    // Marking the previous block as used
    start_block->size_status += 2;

    // Setting up the end mark and marking it as used
    end_mark->size_status = 1;

    // Setting up the footer
    block_header *footer = (block_header*) ((char*)start_block + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
}         
                 
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void Dump_Mem() {         
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end = NULL;
    int t_size;

    block_header *current = start_block;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (block_header*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;
}    
