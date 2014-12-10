/********************************************************************************
* Name:           Nathaniel Thompson
* pawprint:       nctvyc
* Course:         CS 4520
* Assignment#:    Project
*
* File name:      producer.c
*
* Question no:
*
* Assumptions:    1)
*		&		   2)
* Precautions
*********************************************************************************/

#include "project.h"


/* Global variables for producer.c */
   shr_mem_t *sp;     /* pointer to shared memory structure */
   sem_t *MUTEXptr;   /* pointer to the MUTEX semaphore */
   sem_t *EMPTYptr;   /* pointer to the EMPTY semaphore */
   sem_t *FULLptr;    /* pointer to the FULL semaphore  */


/*******************************************************************/
/* Function to cleanup before exiting                              */
/*******************************************************************/
void cleanup_on_exit()
{
/* Only detach shared memory.  Parent will remove it from system */
/* Don't worry about semaphores.  Parent will delete them */


/* Clean up and exit */
   cleanup_on_exit();
   munmap(&sp,sizeof(shr_mem_t));
}


/**************************************************************************/
/* Function to be invoked by the OS if CTRL-C is pressed during execution */
/**************************************************************************/
void ctrl_c(int signum)
{
   perror("producer: You have pressed CTRL_C\n");
   cleanup_on_exit();  /* Perform cleanup operations before exiting */
   exit(0);
}

/**************************************************************************/
/* Function to produce one item in the buffer                             */
/**************************************************************************/
void produce_item(struct item *item)
{

    /* wait for a empty buffer */
    sem_wait(EMPTYptr);
    /* wait for the mutex lock */

    sem_wait(MUTEXptr);
    /* put item id into buffer */
        /* put item id into buffer */
    /* Place associated string into buffer item string by copying it in
      specified location*/
    sp->buff[sp->in] = *item;
    printf("Producer produced item %d on buffer %d\n",sp->buff[sp->in].id,sp->in);
    sp->in = (sp->in + 1) % BUFFSIZE;   /* increment buffer index */
    /* release the mutex lock */
    sem_post(MUTEXptr);
    /* signal a full buffer slot */
    sem_post(FULLptr);
    return;
}


/**************************************************************************/
/* Main producer routine                                                  */
/**************************************************************************/
int main(int argc, char *argv[]){

    printf("Producer entered\n");
   /* A variable of item used in processing */

/* Setup system environment */
   signal(SIGINT, ctrl_c);      /* specify routine to handle CTRL-C */
   setbuf(stdout, NULL);        /* turn off buffering of stdout */
   setbuf(stderr, NULL);        /* turn off buffering of stderr */


   int fd = shm_open("nctvyc", O_RDWR);
   if (fd == -1){
       printf("Producer Error creating shared memory\n");
       perror("shm_open");
       shm_unlink("nctvyc");
       return -1;
   }

   sp = mmap(NULL, sizeof(shr_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (sp == MAP_FAILED){
       printf("Producer Map Failed\n");
       perror("mmap");
       shm_unlink("nctvyc");
       return -1;
   }

   /* Get pointers to the semaphores created by the parent */
   MUTEXptr = sem_open(sp->MUTEXname, O_RDWR);
   EMPTYptr = sem_open(sp->EMPTYname, O_RDWR);
   FULLptr = sem_open(sp->FULLname, O_RDWR);

   /* Read data and produce items until EOF */
   char line[100];
   int id;
   while (feof(stdin) != 1){ 	/* read lines of data fron stdin */
		/* ... read new data item (id and string) from stdin ... */
		/* validate item id values for correctness */
		fgets(line, 100, stdin);//store line into temp char array
        sscanf(line,"%d ",&id);//extract line from the temp char array above
		//check line numbers here <0 & > 110 are invalid
        if(id>=0 && id <=110){
    		//now store the line number and string in the item struct variable
            struct item *newItem = malloc(sizeof(struct item));
            newItem->id = id;
            strcpy(newItem->str, line);
            //call the produce_item function with the above item struct variable
            produce_item(newItem);
        }
        else{
            printf("line number %d out of range\n",id);
        }


  }
/* At EOF ... */
/* Add a sentinal item in Buffer to signal end of data input */
    struct item *newItem = malloc(sizeof(struct item));
    newItem->id = SENTINEL;
    strcpy(newItem->str, "");
/* produce a sentinel value after EOF */
    produce_item(newItem);
/* Write a termination message */
    sem_wait(MUTEXptr);       /* protect printf as a critical section */
    printf("Producer has exited cleanly\n");
    sem_post(MUTEXptr);
    shm_unlink("nctvyc");
   exit(0);
}
