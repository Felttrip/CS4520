/********************************************************************************
* Name:           Nathaniel Thompson
* pawprint:       nctvyc
* Course:         CS 4520
* Assignment#:    Project
*
* File name:      project.h
*
* Question no:
*
* Assumptions:    1)
*		&		   2)
* Precautions
*********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <memory.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>

#define NUM_CONSUMERS 3
#define BUFFSIZE 11
#define MAX_STRINGS 112
#define SENTINEL -999999999  /* sentinel value for consumer process */

/* Bounded Buffer item structure */
struct item {
   int  id;  /* string index value */
   char str[90];  /* string value */
   };

/* Structure for the shared memory region */
typedef struct{
   int    debug;           /* debug flag */
   int    in;              /* index of next empty slot */
   int    out;             /* index of next full slot  */
   char   MUTEXname[32];   /* name of the MUTEX semaphore */
   char   EMPTYname[32];   /* name of the EMPTY semaphore */
   char   FULLname[32];    /* name of the FULL semaphore  */
   struct item buff[BUFFSIZE];  /* circular buffer for producer/consumer items*/
   char   strarray[MAX_STRINGS][100]; /* shared array of strings for consumers */
} shr_mem_t;
