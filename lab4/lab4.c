/***************************************************************************
 * Name:
 * Userid:
 * Course:         CS 4520
 * Assignment#:    Lab 4
 *
 * File name:      lb4_template.c
 *
 * Question no:	   5.37
 *
 *  Assumptions  1) Which data is involved in race condition ?
 *                  The resources variable is involved in the race condition.
 *	 &		       2) When does the race condition occur ?
                    The race condition occurs when two threads access the resources
                    at the same time and attempt to incement and decrement the variable
                    at the time.
 *	Precautions	 3) MAX_RESOURCES should defines the max number of resources
 * 				       4) NUM_PROCESSES should defines the no of threads to create
 *				       5) cc -pthread -lpthread lb5.c
 *				       6) Check instructions doc for additional instructions
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> //Included only to create a random seed for rand()
#include <errno.h>
//define NUM_PROCESSES
#define NUM_PROCESSES 10

//define MAX_RESOURCES
#define MAX_RESOURCES 5

//global variable declaration for available_resources
sig_atomic_t available_resources = MAX_RESOURCES;
//other global declarations
//Mutex
pthread_mutex_t leMutex = PTHREAD_MUTEX_INITIALIZER;

//function that thread runs
void *workerThread(void*);
//increase_count function
int increase_count(int);
//decrease_count function
int decrease_count(int);

int main(void){
  //initialize random seed
  srand (time(NULL));
  int i = 0;
  int resources_requested = 0;
  pthread_t threads[NUM_PROCESSES];
  pthread_attr_t threadAttributes;
  //Use default attributes
  pthread_attr_init(&threadAttributes);
  for(i=0;i<NUM_PROCESSES;i++){
    resources_requested = (rand()%MAX_RESOURCES)+1;
    pthread_create(&threads[i],&threadAttributes,workerThread,(void *)resources_requested);
  }

  for(i=0;i<NUM_PROCESSES;i++){
    pthread_join(threads[i],NULL);
  }


  //distroy mutex
  pthread_mutex_destroy(&leMutex);
  pthread_exit(NULL);
  return 0;
}

void* workerThread(void *resources){

  //Take the resource, make sure to access and check if there are enough
  do{
    //wait for access
    pthread_mutex_lock(&leMutex);
    //check status
    if((int)available_resources<(int)resources){
     //release mutex if we can't use it
     pthread_mutex_unlock(&leMutex);
    }
    else{
     //if there are enough resources use them
     decrease_count(resources);
     printf("Taking %d resources current count = %d\n",resources,available_resources);
     //release access to the resources
     pthread_mutex_unlock(&leMutex);
     break;
    }
  }while(1);

  sleep(rand()%5);

  //Give the resources back, less strict because we are just returning.
  //wait for access
  pthread_mutex_lock(&leMutex);
  //recieve access to resources
  increase_count(resources);
  printf("Returning %d resources current count = %d\n",resources,available_resources);
  //release access to the resources
  pthread_mutex_unlock(&leMutex);

pthread_exit(NULL);
}

//increase_count function
int increase_count(int request){
  available_resources = available_resources + request;
  return 0;
}
//decrease_count function
int decrease_count(int request){
  available_resources = available_resources - request;
  return 0;
}
