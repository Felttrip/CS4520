/***************************************************************************
 * Name:           Nathaniel Thompson
 * Pawprint:       NCTVYC
 * Course:         CS 4520
 * Assignment#:    Lab 3
 *
 * File name:      lb3.c
 *
 * Question no:	   4.26        
 *
 * Assumptions:    1) compile & link with cc -pthread -lpthread lb4.c
 *		&		   2) Have an array to store max of 50 numbers
 * Precautions	   3)
 * 				   4)
 *				   5) 
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define ARRAY_SIZE 51
//define global variable
long int fibSequence[ARRAY_SIZE];
//define function that the thread runs
void *fibThread(void *param);
//define main
int main(int argc, char *argv[]){ 
	//thread idenrifier and attributes
	pthread_t threadId;
	pthread_attr_t threadAttributes;

	//check input for correct args and size
	if (argc != 2) {
		fprintf(stderr,"usage: a.out <integer value>\n"); 
		return -1;
	}
	if(atoi(argv[1]) <= 0) {
		fprintf(stderr,"Input must be > 0\n"); 
		return -1;
	}
	if(atoi(argv[1]) > 50) {
		fprintf(stderr,"Input must be <= 50\n"); 
		return -1;
	}

	//Use default attributes
	pthread_attr_init(&threadAttributes);

	//create thread begin controll in fibThread 
	pthread_create(&threadId,&threadAttributes,fibThread,argv[1]); 
	
	//wait for thread to finish
	pthread_join(threadId,NULL);

	//print fib sequence
	int i = 0;
	while(fibSequence[i]!=-1){
		printf("%ld, ",fibSequence[i]);
		i++;	
	}
	printf("\n"); 
	

}  

//Where the thread starts
void *fibThread(void *param){ 
	//local vars for calcularing
	int i;
	int sizeOfSequence = atoi(param); 
	long int currentNumber = 1;
	long int pastNumber = 0;
	//iniitalize the array to -1
	for(i=0;i<ARRAY_SIZE;i++){
		fibSequence[i]=-1;
	}
	//calculate the fib sequence for a given size
	fibSequence[0] = 0;
	if(sizeOfSequence > 1){
	 	fibSequence[1] = 1;
	    for (i = 2; i < sizeOfSequence; i++){
	    	fibSequence[i] = currentNumber + pastNumber;
	       	pastNumber = currentNumber;
	    	currentNumber = fibSequence[i];
	    }
	}
	pthread_exit(0);
}
