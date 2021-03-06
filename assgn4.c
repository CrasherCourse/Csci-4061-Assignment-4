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

pthread_mutex_t nameTransferLock, sizeTransferLock;
int firstThread;

// This is the Thread function
void * traverseDirectory(void * input)
{
	DIR * dir;
	struct dirent *dirEntry;
	struct stat fileStat;
	char *entryName, *dirName;
	int i = 0, tIndex = 0;
	tData *parData, myData;
	pthread_t tid[NUM_THREADS];
	
	
	parData = (tData *) input;								// make a tData pointer out of the input
	dirName = (char *) malloc(NAMESIZE * sizeof(char));		// save a string of the current directory
	strcpy(dirName, parData->childName);
	if(firstThread) firstThread = 0;						// unlock name transfer mutex if not the first thread
	else pthread_mutex_unlock(&nameTransferLock);

	myData.parentSize = 0;									// initilize myData and entry Name
	myData.childName = (char *) malloc(NAMESIZE * sizeof(char));
	entryName = (char *) malloc(NAMESIZE * sizeof(char));	

	if( (dir = opendir(dirName)) == NULL)					// open the directory
	{
		perror("opendir: ");
		exit(1);
	}
	
	while( (dirEntry = readdir(dir)) != NULL)					// Read an entry from the directory
	{
		if((strcmp(dirEntry->d_name, ".") == 0) ||				// ignore current and parent directory
			(strcmp(dirEntry->d_name, "..") == 0))	continue;

		strcpy(entryName, dirName);								// create a path to the current dirEntry
		strcat(entryName, "/");
		strcat(entryName, dirEntry->d_name);

		if((lstat(entryName, &fileStat)) == -1)		// Make a lstat of the entryName
		{
			perror("lstat: ");
		}
		
		if(S_ISREG(fileStat.st_mode))				// Add size of regular file to diretory size
		{
			myData.parentSize += fileStat.st_size;
		}
		else if(S_ISDIR(fileStat.st_mode))			// Create a new thread for the subdirectory
		{
			pthread_mutex_lock(&nameTransferLock);	// Make a crit section to protect transfer of directory name
			strcpy(myData.childName, entryName);
			pthread_create(&tid[tIndex], NULL, traverseDirectory, ((void *) &myData));	// Create thread for subdirectory
			tIndex++;
		}
	}
	// wait for child threads to return
	for(i = 0; i < tIndex; i++)
	{
		pthread_join(tid[i], NULL);
	}
	#ifdef DEBUG
	printf("DEBUG: %s/ %d\n", dirName, myData.parentSize);
	#endif
	
	pthread_mutex_lock(&sizeTransferLock);			// Make a critical section for adding to size
	parData->parentSize += myData.parentSize;		// Add myData's size to Parent's data
	pthread_mutex_unlock(&sizeTransferLock);
	
	free(dirName);									// Free up the malloc'd char*'s
	free(myData.childName);
	free(entryName);
	pthread_exit(NULL);								// Exit thread
}

int main(int argc, char *argv[])
{
	char *input_dir_name, *mydirpath, *chptr;
	int totalSize;
    struct stat statbuf;
	pthread_t tid;
	void * status, *voidTotal;
	tData result;

	pthread_mutex_init(&nameTransferLock, NULL);
	pthread_mutex_init(&sizeTransferLock, NULL);
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
	firstThread = 1;			// Used to prevent signaling when there is no waiting
	
	pthread_create(&tid, NULL, traverseDirectory, ((void *) &result));	// Create the first thread
	pthread_join(tid, NULL);
	printf("\nTotal Size: %d\n\n", result.parentSize);					// Print out the total size
	
	free(input_dir_name);
	free(mydirpath);
	return 0;
}
