/********************************************************************************
 * Name:           Nathaniel Thompson
 * pawprint:       nctvyc
 * Course:         CS 4520
 * Assignment#:    Project
 *
 * File name:      consumer.c
 *
 * Question no:
 *
 * Assumptions:    1)
 *		&		   2)
 * Precautions
 *********************************************************************************/

#include "project.h"


/* Global variables for consumer.c  */
   sem_t *MUTEXptr;   /* pointer to the MUTEX semaphore */
   sem_t *EMPTYptr;   /* pointer to the EMPTY semaphore */
   sem_t *FULLptr;    /* pointer to the FULL semaphore  */
   shr_mem_t *sp;     /* pointer to shared memory structure */



/*******************************************************************/
/* Function to cleanup before exiting                              */
/*******************************************************************/
void cleanup_on_exit()
{
/* Only detach shared memory.  Parent will remove it from system */
/* Don't worry about semaphores.  Parent will delete them */


/* Clean up and exit */
    munmap(&sp,sizeof(shr_mem_t));

}


/**************************************************************************/
/* Function to be invoked by the OS if CTRL-C is pressed during execution */
/**************************************************************************/
void ctrl_c(int signum)
{
   perror("consumer: You have pressed CTRL_C\n");
   cleanup_on_exit();  /* Perform cleanup operations before exiting */
   exit(0);
}

void *ThreadRunner(void *arg){
    long id = 1;
    int threadId = (int)arg;
    /* Consume items until the SENTINEL is found */
    while(1) {
        /* wait for a empty buffer */
        sem_wait(FULLptr);
        /* wait for the mutex lock */
        sem_wait(MUTEXptr);
        //check for sentinal
        id = sp->buff[sp->out].id;

        if (id!=SENTINEL) {
            printf("Consumer id %d consumed item %d on buffer %d\n",threadId,sp->buff[sp->out].id,sp->out);


            /* .... place item string into string array .... */
            strcpy(sp->strarray[id],sp->buff[sp->out].str);

            /* increment buffer index*/
            sp->out = (sp->out + 1) % BUFFSIZE;
        }
        else{
            break;
        }
        //if sentinel is encountered, then put the item into the buffer and break
        //from the loop
        /* release the mutex lock */
        /* signal an empty buffer slot */
        /* release the mutex lock */
        sem_post(MUTEXptr);
        /* signal a full buffer slot */
        sem_post(EMPTYptr);
    }
    sem_post(MUTEXptr);
    sem_post(FULLptr);
    return 1;
}

/**************************************************************************/
/* Main Consumer Routine                                                  */
/**************************************************************************/
int main(int argc, char *argv[])
{

    printf("Consumer entered\n");
    /* A variable of item used in processing */

    /* Setup system environment */
    signal(SIGINT, ctrl_c);      /* specify routine to handle CTRL-C */
    setbuf(stdout, NULL);        /* turn off buffering of stdout */
    setbuf(stderr, NULL);        /* turn off buffering of stderr */


    int fd = shm_open("nctvyc", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1){
        printf("Consumer Error creating shared memory\n");
        perror("shm_open");
        shm_unlink("nctvyc");
        return -1;
    }

    sp = mmap(NULL, sizeof(shr_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sp == MAP_FAILED){
        printf("Consumer Map Failed\n");
        perror("mmap");
        shm_unlink("nctvyc");
        return -1;
    }

    /* Get pointers to the semaphores created by the parent */
    MUTEXptr = sem_open(sp->MUTEXname, O_RDWR);
    EMPTYptr = sem_open(sp->EMPTYname, O_RDWR);
    FULLptr = sem_open(sp->FULLname, O_RDWR);

	//thread creation
    pthread_t threads[NUM_CONSUMERS];
    pthread_attr_t threadAttributes;

    //Use default attributes
    pthread_attr_init(&threadAttributes);

    //create threads
    int i;
    for(i=0;i<NUM_CONSUMERS;i++){
        pthread_create(&threads[i],&threadAttributes,ThreadRunner,(void *)i);
    }

    //wait for threads to finish
    for(i=0;i<NUM_CONSUMERS;i++){
        pthread_join(threads[i],NULL);
    }

	//wait till threads complete execution
    pthread_exit(NULL);


/* Write a termination message */
    sem_wait(MUTEXptr);      /* protect printf as a critical section */
    printf("Consumer has exited cleanly\n");
    sem_post(MUTEXptr);

   cleanup_on_exit();
   exit(0);
}
