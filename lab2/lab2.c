/***************************************************************************
 * Name:           Nathaniel Thompson    
 * Userid:         nctvyc
 * Course:         CS 4520
 * Assignment#:    Assignment 2
 *
 * File name:      lab2.c
 *
 * Question no:	   3.22        
 *
 * Assumptions:    1) Link with -lrt when compiling
 *		&		   2)
 * Precautions	   3)
 * 				   4)
 *				   5) 
 **************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc, char **argv)
{
	//initializations
	
	//error checks
	
	//create a shared memory segment, fill the parameters
    int fd = shm_open("/sharedmemory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	//check if shared memory segment is created successfully
	if (fd == -1){
		printf("Error creating shared memory\n");
		shm_unlink("/sharedmemory");
		return -1;
	}

	//allocate page size and check for errors, fill the parameters
    if (ftruncate(fd, sizeof(int)) == -1){
    	printf("error allocating page size\n");
    	shm_unlink("/sharedmemory");
    	return -1;
    }
	
	//retrieve the address of shared memory segment, fill the parameters
    int *addr = mmap(NULL, sizeof(int)*500, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	//initialize array to -1
	int i;
	for(i=0;i<500;i++){
		addr[i]=-1;
	}
	//check if address valid
	if(addr == MAP_FAILED){
		printf("Invalid address\n");
		shm_unlink("/sharedmemory");
		return -1;
	}
	//create child process and write rest of the logic
    pid_t pid;
	pid = fork();
	//child
	if (pid == 0) {
		int input = 0;
		printf("Please enter a positive integer less than 715827882 for the Collatz conjecture.\n");
		scanf("%d",&input);
		//error checking
		if(input < 1){
			printf("Error, not a possitive integer, quitting\n");
			shm_unlink("/sharedmemory");
			return -1;
		}
		if(input > 715827882){
			printf("Error, numbers larger than 715827882 are not supported, quitting\n");
			shm_unlink("/sharedmemory");
			return -1;
		}
		int i = 0;
		do{
			addr[i] = input;
			if(input%2 == 0){
				input = input/2;
			}
			else if (input!=1){
				input = 3*input+1;
			}
			i++;
			//edge case
			if(input == 1){
				addr[i] = input;	
			}
		}while(input!=1);
	}
	//parent
	else if (pid > 0) { 
		wait(NULL);
		int i = 0;
		while(addr[i] > -1){
			printf("%d ",addr[i]);
			i++;
		}
		printf("\n");
		
	}

	//ensure you use this statement at each place your program exits
    shm_unlink("/sharedmemory");
	
    return 0;
}
