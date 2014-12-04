/********************************************************************************
* Name:           Nathaniel Thompson
* Userid:         nctvyc
* Course:         CS 4520
* Assignment#:    Lab 6
*
* File name:      lab6.c
*
* Question no:	   9.39
*
* Assumptions:    1) Use this string : 1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6
*		&		   2)
* Precautions
*********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
//structure to store important information
typedef struct{
	int occupied;//is the frame occupied ?
	int turnOfEntry;//when did the page occupy the frame?
	int frameNo;//frame number
	int lastUsed;//when was it last used
} frame;

//have individual methods for each algorithm
int fifo(int);
int lru(int);
int opt(int);
//helper methods
int lastUsed(frame*,int);
int furthestOut(frame*, int, int);
int exists(int, frame*, int);
int lastAdded(frame*, int);
int nextRef(int, int);


//globals
int randomPageRef[20] = {1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6};
int pageRefLen = sizeof(randomPageRef)/sizeof(randomPageRef[0]);

//main
int main(void){
	int i = 0;
	 for(i=1;i<=7;i++){
	 	fifo(i);
	 }
	 for(i=1;i<=7;i++){
	 	lru(i);
	 }
	for(i=1;i<=7;i++){
		opt(i);
	}


}

//First in First out algo
int fifo(int numFrames){
	int turnCount = 0;
	int pagefaults = 0;
	frame *frameArr = malloc(sizeof(frame)*numFrames);
	int i = 0;
	int replaceIndex = 0;

	//initialize frames
	for(i = 0; i<numFrames; i++){
		frameArr[i].occupied = 0;
		frameArr[i].turnOfEntry = -1;
		frameArr[i].frameNo = -1;
		frameArr[i].lastUsed = 0;
	}

	for(i = 0; i<pageRefLen; i++){
		if(exists(randomPageRef[i],frameArr,numFrames)>-1){
		}
		else{
			pagefaults++;
			replaceIndex = lastAdded(frameArr,numFrames);
			frameArr[replaceIndex].occupied = 1;
			frameArr[replaceIndex].turnOfEntry = turnCount;
			frameArr[replaceIndex].frameNo = randomPageRef[i];
			//printf("Page fault on number %d at index %d\n",randomPageRef[i],replaceIndex);
		}
		turnCount++;
	}
	printf("Page faults for FIFO for %d no of frames is : %d\n",numFrames,pagefaults);
	return 1;
}

//least recently used
int lru(int numFrames){
	int turnCount = 0;
	int pagefaults = 0;
	frame *frameArr = malloc(sizeof(frame)*numFrames);
	int i = 0;
	int existsIndex = 0;
	int replaceIndex = 0;

	//initialize frames
	for(i = 0; i<numFrames; i++){
		frameArr[i].occupied = 0;
		frameArr[i].turnOfEntry = -1;
		frameArr[i].frameNo = -1;
		frameArr[i].lastUsed = -1;
	}
	for(i = 0; i<pageRefLen; i++){
		existsIndex = exists(randomPageRef[i],frameArr,numFrames);
		if(existsIndex>-1){
			frameArr[existsIndex].lastUsed = turnCount;
			//printf("Used frame %d on turn %d\n",randomPageRef[i],turnCount);
		}
		else{
			//add a page fault
			pagefaults++;
			//find the one to replace by finding the last used frame
			replaceIndex = lastUsed(frameArr,numFrames);
			//set it as occupies
			frameArr[replaceIndex].occupied = 1;
			//update its turn of entry
			frameArr[replaceIndex].turnOfEntry = turnCount;
			//update its frame number
			frameArr[replaceIndex].frameNo = randomPageRef[i];
			//set its last used to the current turn count
			frameArr[replaceIndex].lastUsed = turnCount;
			//printf("Page fault on number %d at index %d\n",randomPageRef[i],replaceIndex);
		}
		turnCount++;
	}
	printf("Page faults for LRU for %d no of frames is : %d\n",numFrames,pagefaults);
	return 1;
}

int opt(int numFrames){
	int turnCount = 0;
	int pagefaults = 0;
	frame *frameArr = malloc(sizeof(frame)*numFrames);
	int i = 0;
	int existsIndex = 0;
	int replaceIndex = 0;

	//initialize frames
	for(i = 0; i<numFrames; i++){
		frameArr[i].occupied = 0;
		frameArr[i].turnOfEntry = -1;
		frameArr[i].frameNo = -1;
		frameArr[i].lastUsed = -1;
	}
	for(i = 0; i<pageRefLen; i++){
		existsIndex = exists(randomPageRef[i],frameArr,numFrames);
		if(existsIndex>-1){
		}
		else{
			pagefaults++;
			//find the one to replace by finding the last used frame
			replaceIndex = furthestOut(frameArr,numFrames,turnCount);
			frameArr[replaceIndex].occupied = 1;
			frameArr[replaceIndex].turnOfEntry = turnCount;
			frameArr[replaceIndex].frameNo = randomPageRef[i];
			frameArr[replaceIndex].lastUsed = turnCount;
		}
		turnCount++;
	}
	printf("Page faults for OPT for %d no of frames is : %d\n",numFrames,pagefaults);
	return 1;

}

int lastUsed(frame *frameArr,int numFrames){
	int i;
	int min = 999;
	int minIndex = 999;
	for(i=0;i<numFrames;i++){
		//if its not ocupied return the frame
		if(frameArr[i].occupied == 0){
			return i;
		}
		//if it is ocupied find the least used frame by finding the frame
		//with the smallest lastUsed value
		if(frameArr[i].lastUsed<min){
			min = frameArr[i].lastUsed;
			minIndex = i;
		}
	}
	return minIndex;
}

//returns 1 if in array returns -1 if not in array
int exists(int num, frame *frameArr, int numFrames){
	int i = 0;
	for(i=0;i<numFrames;i++){
		if(num == frameArr[i].frameNo){
			return i;
		}
	}
	return -1;
}

int lastAdded(frame *frameArr, int numFrames){
	int i = 0;
	int min = 999;
	int minIndex = 999;
	for(i=0;i<numFrames;i++){
		if(frameArr[i].occupied==0){
			return i;
		}
		if(frameArr[i].turnOfEntry<min){
			min = frameArr[i].turnOfEntry;
			minIndex = i;
		}
	}
	return minIndex;
}


int furthestOut(frame *frameArr, int numFrames, int currentTurn){
	int i = 0;
	int j = 0;
	int futhestIndex = 0;
	int indexToReplace = 999;

	//check for empty frames
	for(i=0;i<numFrames;i++){
		//if the frame isn't filled fill it
		if(frameArr[i].occupied == 0){
			return i;
		}
	}

	//for each frame
	int temp;
	for(i=0;i<numFrames;i++){
		//find the soonest refeerence to the value in the frame
		temp = nextRef(currentTurn,frameArr[i].frameNo);
		if(temp == -1){
			return i;
		}
		if(temp>futhestIndex){
			futhestIndex = temp;
			indexToReplace = i;
		}
	}
	//return the frame with the furthest out ref.
	return indexToReplace;
}

int nextRef(int currentTurn, int frameNumToCheck){
	int i;
	for(i=currentTurn; i<pageRefLen; i++){
		//frame matches ref
		if(frameNumToCheck==randomPageRef[i]){
			return i;
		}
	}
	return -1;
}
