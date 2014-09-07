/***************************************************************************
 * Name:           Nathaniel Thompson  
 * Userid:         NCTVYC
 * Course:         CS 4520
 * Assignment#:    Assignment 1
 *
 * File name:      as1.c
 *
 * Question no:	   2.26        
 *
 * Assumptions:    1)Input file name is lb1.input
 *		&		   2)Output file name should be lb1.output
 * Precautions	   3)Ensure that you have read and write permissions in the 
 *				     directory 
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//Function prototypes
int print_help(void);		//Help message explaining the programs usage
int print_usage(void);		//Quick usage help
int control(char*,char*);	//Controls errors and what not
int file_state(char*);		//Checks file state and returns state
int copy(char*, char*);		//Does actual file copy after everything is good to go


/* main
 * 1. Checks for corret number of agruments and prints help if needed.
 * 2. If correct number of arguments is passed, sends program to control.
 */
int main(int argc, char *argv[]){
	//check for no args
	if(argc == 1){
		print_help();
	}
	//one arg 
	else if(argc == 2 || argc > 3){
		print_usage();
	}
	//two args
	else if(argc == 3){
		control(argv[1],argv[2]);
	}
}

/* control
 * 1. Checks to make sure file permissions are correct before using files
 * 2. If permissions are good program moves to copying of files
 */
int control(char* source, char* target){
	int copy_error = 0;
	//check source, source only needs to be readable
	int source_status = file_state(source);
	switch (source_status) {
		case 0:
	  		break;
		case 1:
	  		printf("Source file  does not exist.\n");
	  		break;
		case 2:
	  		printf("Source file is not readable.\n");
	  		break;
  		case 3:
	  		break;
	}
	//check destination, destination should not exist
	int target_status = file_state(target);
	switch (target_status) {
		case 0:
	  		printf("Target file already exists.\n");
	  		break;
		case 1:
	  		break;
		case 2:
	  		printf("Target file already exists.\n");
	  		break;
  		case 3:
	  		printf("Target file already exists.\n");
	  		break;
	}
	
	//if source is good and target doesn't exits copy the file.
	if((source_status == 0 || source_status == 3)&&(target_status == 1)){
		copy_error = copy(source,target);
	}

	//if there was an error copying tell the user
	switch (copy_error){
		case 0:
			break;
		case 1:
			printf("Error opening source file.\n");
			break;
		case 2:
			printf("Error creating target file.\n");
			break;
		case 3:
			printf("Error copying source to target.\n");
			break;
	}
	return 0;
}

/* copy
 * 1. Attempts to open/ create files
 * 2. If successfull, copys the contents of source to target
 */
int copy(char* source, char* target){
	int source_file;
	int target_file;
	ssize_t bytes_read;
	ssize_t bytes_written;
	int buffer_size = 1000;
    char buffer[buffer_size];

    //open files
	target_file = open(target,O_WRONLY | O_CREAT);
	source_file = open(source,O_RDONLY);

	//error if problems opening files
	if(source_file == -1){
		return 1;
	}
	if(target_file == -1){
		return 2;
	}
	
	//good open start copying
	else{
		//while there are still bytes to read, read them to the buffer and write the buffer to the new file
		while((bytes_read = read (source_file, &buffer, buffer_size)) > 0){
	            bytes_written = write (target_file, &buffer, (ssize_t) bytes_read);
	            //if the bytes read and the bytes written dont match up
	            //stop copying and tell the user
	            if(bytes_read != bytes_written){
	                return 3;
	            }
	    }
	}
    //close files
    close(target_file);
    close(source_file);
	return 0;
}


/* File status function
 * 0 = Exists and is readable and writable
 * 1 = Does not exist
 * 2 = Can not read
 * 3 = can not write
 */
int file_state(char* file_name){
	//if the file does not exist return 1
	if( access(file_name, F_OK ) == -1 ) {
    	return 1;
	} 
	//if the file exists and cant be read return 2
	else if(access(file_name, R_OK ) == -1 ){
    	return 2;
	}
	//if the file exists and can't be written return 3
	else if(access(file_name, W_OK) == -1){
		return 3;
	}
	//if the file exists, and is ok for read and write return 0
	else{
		return 0;
	}
}

//Prints the help document
int print_help(void){
	char *help_message = "This is the copy command, it can be used to copy one file to another location.\n\n";
	printf("%s",help_message);
	print_usage();
 return 0;
}

//Prints the usage information for the prog
int print_usage(void){
	char *usage = "usage: copy source_file target_file\n\n";
	printf("%s",usage);
	return 0;
}
