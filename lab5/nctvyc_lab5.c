/***************************************************************************
 * Name:           Nathaniel Thompson
 * Pawprint:       nctvyc
 * Course:         CS 4520
 * Assignment#:    Lab 5
 *
 * File name:      nctvyc_lab5.c
 *
 * Question no:	   Programming project 3
 *
 * Assumptions:    1) Let producer generate numbers between 0 and 100 only
 *		&		   2) Run the final program for 5 seconds with 2 producers
 *	Precautions		  and 5 consumers
 **************************************************************************/
#include "buffer.h"

#define MAX_RAND 100

sem_t empty;
sem_t full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

buffer_item buffer[BUFFER_SIZE];

//remove_item function

//producer thread function
void *producerThread(void*);

//consumer thread function
void *consumerThread(void*);

//main
int main(int argc, char *argv[]){
    srand (time(NULL));
	if(argc!=4){
		printf("Improper args.\n Usage ./lab5 <sleep_time> <num producers> <num consumers>\n");
		return -1;
	}
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    int i = 0;
    int sleeptime = atoi(argv[1]);
    int numProducerThreads = atoi(argv[2]);
    int numConsumerThreads = atoi(argv[3]);

    for(i=0; i<BUFFER_SIZE; i++){
        buffer[i] = -1;
    }

    //set up threads
    pthread_t *producerThreads = malloc(sizeof(pthread_t)*numProducerThreads);
    pthread_t *consumerThreads = malloc(sizeof(pthread_t)*numConsumerThreads);
    pthread_attr_t threadAttributes;
    pthread_attr_init(&threadAttributes);

    for(i=0;i < numProducerThreads; i++){
        pthread_create(&producerThreads[i],&threadAttributes,producerThread,(void *)i);
    }
    for(i=0;i < numConsumerThreads; i++){
        pthread_create(&consumerThreads[i],&threadAttributes,consumerThread,(void *)i);
    }
    sleep(sleeptime);
	return 0;
}

void* producerThread(void *i){
    buffer_item produced_item;
    do {
        /* produce an item in next produced */
        produced_item = rand()%MAX_RAND;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        /* add next produced to the buffer */
        insert_item(produced_item);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    } while (1);
}

void* consumerThread(void *i){
    buffer_item consumed_item;
    do {
        consumed_item = rand()%MAX_RAND;
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        /* remove an item from buffer to next consumed */
        remove_item(consumed_item);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        /* consume the item in next consumed */
    } while (1);

}

int insert_item(buffer_item item) {
    int i = 0;
    for(i = 0; i< BUFFER_SIZE; i++){
        if(buffer[i] == -1){
            buffer[i] = item;
            printf(" Producer produced %d\n",item);
            return 1;
        }
    }
    return 0;
}
int remove_item(buffer_item item) {
    int i = 0;
    for(i = 0; i< BUFFER_SIZE; i++){
        if(buffer[i] == item){
            buffer[i] = -1;
            printf("Consumer consumed %d\n",item);
            return 1;
        }
    }
    return 0;
}
