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
 *				   directory 
 * 				   4)
 *				   5)
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
/* In Section 2.3, we described a program that copies the contents of one file to a destination file. 
   This program works by first prompting the user for the name of the source and destination files. 
   Write this program using either the Windows or POSIX API. Be sure to include all necessary error 
   checking, including ensuring that the source file exists.
   Once you have correctly designed and tested the program, 
   if you used a system that supports it, run the program using a utility that traces system calls. 
   Linux systems provide the strace utility, and Solaris and Mac OS X systems use the dtrace command. 
   As Windows systems do not provide such features, you will have to trace through the Windows version 
   of this program using a debugger.
*/

/* Thoughts on usage
  as1.c compiles to some copy file (copy)
  copy <no args> -> prints help page
  copy <onearg> -> prints usage and tells user to use copy with no args to get help page
  copy <onearg> <twoarg>  if able to copy copys <onearg> to <twoarg> if it can't copy throws an error message to stderr stating why
*/

//Function prototypes
int print_help(void);		//Help message explaining the programs usage
int print_usage(void);		//Quick usage help
int control(char*,char*);	//Controls errors and what not
int file_state(char*);		//Checks file state and returns state
int copy(char*, char*);		//Does actual file copy after everything is good to go


/* main
 * Checks for corret number of agruments and prints help if needed.
 * If correct number of arguments is passed, sends program to control.
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
	if(copy_error == -1){
		printf("Error copying file\n");
	}
	return 0;
}

/* copy
 *	
 *
 */
int copy(char* source, char* target){
	int *sourceFile;
	int *targetFile;
	ssize_t ret_in, ret_out;
	int buffer_size = 1000;
    char buffer[buffer_size];
	targetFile = open(target,O_WRONLY | O_CREAT, 0644);
	sourceFile = open(source,O_RDONLY);
	if(targetFile == -1 || sourceFile == -1){
		return -1;
	}
	else{
		while((ret_in = read (sourceFile, &buffer, buffer_size)) > 0){
	            ret_out = write (targetFile, &buffer, (ssize_t) ret_in);
	            if(ret_out != ret_in){
	                return -1;
	            }
	    }
	}
    //close files
    close(targetFile);
    close(sourceFile);
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
	char *help_message = "This is the copy command, it can be used to copy one file to another location.";
	printf("%s\n",help_message);
	print_usage();
 return 0;
}

//Prints the usage information for the prog
int print_usage(void){
	char *usage = "usage: copy source_file target_file\n\n";
	printf(usage);
	return 0;
}
