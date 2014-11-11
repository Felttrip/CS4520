/***************************************************************************
 * Name:           Nathaniel Thompson
 * Pawprint:       nctvyc
 * Course:         CS 4520
 * Assignment#:    Lab 5
 *
 * File name:      buffer.h
 *
 * Question no:	   Programming project 3
 *
 * Assumptions:    1) Let producer generate numbers between 0 and 100 only
 *		&		   2) Run the final program for 5 seconds with 2 producers
 *	Precautions		  and 5 consumers
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

typedef int buffer_item;
#define BUFFER_SIZE 5
