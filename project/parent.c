/********************************************************************************
* Name:           Nathaniel Thompson
* pawprint:       nctvyc
* Course:         CS 4520
* Assignment#:    Project
*
* File name:      parent.c
* sub key: 560efe8677789c44a216b8cc9c36a85f
* Question no:
*
* Assumptions:    1)
*		&		   2)
* Precautions
*********************************************************************************/

#include "project.h"


/* Global variables for parent.c */
   char MUTEXid[32];   /* the unique generated name for the MUTEX semaphore */
   char EMPTYid[32];   /* the unique generated name for the EMPTY semaphore */
   char FULLid[32];    /* the unique generated name for the FULL semaphore  */


/*******************************************************************/
/* Function to cleanup shared memory and semaphores before exiting */
/*******************************************************************/
void cleanup_on_exit()
{
   //shmdt(shmptr);                   /* detach the shared memory */
   //shmctl(shmid,IPC_RMID,NULL);                  /* delete the shared memory segment */
   sem_unlink(MUTEXid);            /* delete the MUTEX semaphore */
   sem_unlink(EMPTYid);            /* delete the EMPTY semaphore */
   sem_unlink(FULLid);             /* delete the FULL sempahore */
   shm_unlink("nctvyc");

}


/**************************************************************************/
/* Function to be invoked by the OS if CTRL-C is pressed during execution */
/**************************************************************************/
void ctrl_c(int signum)
{
   printf("Parent: You have pressed CTRL-C\n");
   cleanup_on_exit();
   exit(0);
}


/**************************************************************************/
/* Main program function                                                  */
/**************************************************************************/
int main(int argc, char *argv[]) {
/* Local variable declarations */

   shr_mem_t *sp;     /* pointer to shared memory structure */
   int pid;           /* process ID number */



/* Setup system environment */
   signal(SIGINT, ctrl_c);    /* specify routine to handle CTRL-C */
   setbuf(stdout, NULL);      /* turn off buffering of stdout */
   setbuf(stderr, NULL);      /* turn off buffering of stderr */

/* Generate unique sempaphore names and initialize the semaphores */
   pid = getpid();   /* get process id for semaphore names */
   sprintf(MUTEXid,"semmutex%d", pid);   /* generate MUTEX semaphore name */
   sprintf(EMPTYid,"semempty%d", pid);   /* generate EMPTY semaphore name */
   sprintf(FULLid,"semfull%d", pid);     /* generate FULL semaphore name  */


   sem_t *mutex = sem_open(MUTEXid, O_CREAT | O_RDWR, 0644, 1);        /* init the MUTEX semaphore */
   sem_t *empty = sem_open(EMPTYid, O_CREAT | O_RDWR, 0644, BUFFSIZE);  		/* init the EMPTY semaphore */
   sem_t *full = sem_open(FULLid, O_CREAT | O_RDWR, 0644, 0);        /* init the FULL semaphore  */


    int fd = shm_open("nctvyc", O_CREAT | O_RDWR, 0644);
    //check if shared memory segment is created successfully
    if (fd == -1){
        printf("Parent Error creating shared memory\n");
        perror("shm_open");
        shm_unlink("nctvyc");
        return -1;
    }

    //allocate page size and check for errors, fill the parameters
    if (ftruncate(fd, sizeof(shr_mem_t)) == -1){
        printf("%d\n",fd);
        printf("Parent error allocating page size\n");
        perror("ftruncate");
        shm_unlink("nctvyc");
        return -1;
    }

    sp = mmap(NULL, sizeof(shr_mem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(sp == MAP_FAILED){
        printf("Parent Invalid address\n");
        shm_unlink("nctvyc");
        return -1;
    }

   /* Initialize shared memory fields */
   /*use the debug flag if you want to print out some additional info when debugging.
   Note : Using this is not necessary*/
   sp->debug = 0;  /* Turn debug flag off */
   /*We have two separate indexes for producer and consumer*/
   sp->in = 0;     /* index of next empty slot */
   sp->out = 0;    /* index of next full slot  */
   strcpy(sp->MUTEXname, MUTEXid);    /* MUTEX semaphore name */
   strcpy(sp->EMPTYname, EMPTYid);    /* EMPTY semaphore name */
   strcpy(sp->FULLname, FULLid);      /* FULL semaphore name  */
   /* Initialize string array to be empty.  WHY?  */
   int i;
   for (i=0; i<MAX_STRINGS; i++){
       sp->strarray[i][0] = '\0';  /* null string */
   }

/* Fork the producer and consumer processes */
   printf("Creating the producer and consumer processes...\n");

   if (fork() == 0) {  /* in producer process */
      /* Replace this program with producer program */
	  //pass parameters in the ...sections if you want to pass any info to the producer
      if (execl("producer",0) == -1) {
         perror("execl failed for producer");
         cleanup_on_exit();   /* clean up before exiting */
         exit(3);
         }
        }

    if (fork() == 0) {  /* in consumer process */
        /* Replace this program with the consumer program */
        //pass parameters in the ...sections if you want to pass any info to the consumer
        if (execl("consumer",0) == -1) {
            perror("execl failed for consumer");
            cleanup_on_exit();  /* clean up before exiting */
            exit(3);
        }
    }

/* Wait for producer and consumer processes to finish */

     wait(NULL);
     wait(NULL);

/* Print termination message, clean up, and exit */
   /* ... completed processing - print out contents of string array ... */
   for(i=0;i<MAX_STRINGS;i++){
       printf("%s",sp->strarray[i]);
   }


   printf("\nParent process exiting.\n");
   munmap(sp,sizeof(shr_mem_t));
   cleanup_on_exit();      /*Remove shared memory and semaphores */
   return(0);
}
