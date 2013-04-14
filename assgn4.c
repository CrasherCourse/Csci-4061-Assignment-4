/***********************************************************************************************
 CSci 4061 Spring 2013
 Assignment# 4: Multithreaded applications using the pthread library

 Student name: <full name of first student>   
 Student ID:   <first student's ID>   

 Student name: <full name of second student>   
 Student ID:   <Second student's ID>   

 X500 id: <id1>, <id2 (optional)>

 Operating system on which you tested your code: Linux, Unix, Solaris, MacOS
 CSELABS machine: <machine you tested on eg: xyz.cselabs.umn.edu>

 GROUP INSTRUCTION: Please make ONLY one submission when working in a group.
***********************************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#define NAMESIZE 256
#define NUM_THREADS 100
#define DEBUG	
// tData is used to pass data to and from threads
typedef struct tData
{
	int parentSize;
	char * childName;
} tData;

void * traverseDirectory(void * input)
{
	DIR * dir;
	struct dirent *dirEntry;
	struct stat fileStat;
	char *entryName;
	tData *parData, myData;
	pthread_t tid;
	
	myData.parentSize = 0;		// initilize myData
	myData.childName = (char *) malloc(NAMESIZE * sizeof(char));
	parData = (tData *) input;
	entryName = (char *) malloc(NAMESIZE * sizeof(char));

	printf("%s\n", parData->childName);
	if( (dir = opendir(parData->childName)) == NULL)
	{
		perror("opendir: ");
		exit(1);
	}
	
	while( (dirEntry = readdir(dir)) != NULL)
	{

		strcpy(entryName, parData->childName);
		strcat(entryName, "/");
		strcat(entryName, dirEntry->d_name);
		
		if((strcmp(dirEntry->d_name, ".") == 0) ||				// ignore current and parent directory
			(strcmp(dirEntry->d_name, "..") == 0))	continue;
		if((lstat(entryName, &fileStat)) == -1)
		{
			perror("lstat: ");
		}
		
		if(S_ISREG(fileStat.st_mode))				// Add size of regular file to diretory size
		{
			myData.parentSize += fileStat.st_size;
		}
		else if(S_ISDIR(fileStat.st_mode))
		{
			pthread_create(&tid, NULL, traverseDirectory, ((void *) &myData));
			pthread_join(tid, NULL);
		}
	}
	#ifdef DEBUG
	printf("DEBUG: %s/ %d\n", parData->childName, myData.parentSize);
	#endif
	parData->parentSize += myData.parentSize;
	pthread_exit((void *) result);
}

int main(int argc, char *argv[])
{
	char *input_dir_name, *mydirpath, *chptr;
    struct stat statbuf;
	pthread_t tid;
	void * status;
	tData result;

	input_dir_name = (char *) malloc(NAMESIZE * sizeof(char));
	mydirpath = (char *) malloc(NAMESIZE * sizeof(char));
	printf("\n");
	printf("Enter a directory name in the current directory: ");
	scanf("%s", input_dir_name);
	
	/*Form a full path to the directory and check if it exists*/
	mydirpath = getcwd(mydirpath,NAMESIZE);
	strcat(mydirpath,"/");
	chptr = strchr(input_dir_name,'/');
	if(chptr != NULL){
	    strcpy(chptr,"\0");
	}
	strcat(mydirpath,input_dir_name);
    lstat(mydirpath,&statbuf);
	if(!(S_ISDIR(statbuf.st_mode))){
		printf("\nThe directory name is not valid. Directory does not exist\n");
		exit(1);
	}
	/****************************************************************************/
	// Initialize tData for final result
	result.parentSize = 0;
	result.childName = (char *) malloc(NAMESIZE * sizeof(char));
	strcpy(result.childName, input_dir_name);
	
	pthread_create(&tid, NULL, traverseDirectory, ((void *) &result));
	pthread_join(tid, NULL);
	printf("\nTotal Size: %d\n", result.parentSize);
	
	free(input_dir_name);
	free(mydirpath);
	return 0;
}
