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

// This a recursive function to process the thing
int traverseDirectory(char * dirPath, char * dirName)
{
	DIR * dir;
	struct dirent *dir_entry;
	struct stat fileStat;
	char * tempPath, *tempName;
	int size = 0;
	
	if( (dir = opendir(dirPath)) == NULL)
	{
		perror("opendir: ");
		exit(1);
	}
	
	while( (dir_entry = readdir(dir)) != NULL)
	{
		if((strcmp(dir_entry->d_name, ".") == 0) ||				// ignore current and parent directory
			(strcmp(dir_entry->d_name, "..") == 0))	continue;
		if((lstat(dir_entry->d_name, &fileStat)) == -1)
		{
			perror("lstat: ");
		}
		
		if(S_ISREG(fileStat.st_mode))				// Add size of regular file to diretory size
		{
			size += fileStat.st_size;
		}
		else if(S_ISDIR(fileStat.st_mode))
		{
			tempName = (char *) malloc(NAMESIZE * sizeof(char));
			tempPath = (char *) malloc(NAMESIZE * sizeof(char));
			strcpy(tempName, dirName);
			strcat(tempName, "/");
			strcat(tempName, dir_entry->d_name);
			strcpy(tempPath, dirPath);
			strcat(tempPath, "/");
			strcat(tempPath, dir_entry->d_name);
			size += traverseDirectory(tempPath, tempName);
		}
		
	}
	
	printf("DEBUG: %s/ %d\n", dirName, size);
	return size;
	
}

int main(int argc, char *argv[])
{
	char *input_dir_name, *mydirpath, *chptr;
    	struct stat statbuf;
	int totalSize;

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
	totalSize = traverseDirectory(mydirpath, input_dir_name);
	printf("\nTotal Size: %d\n", totalSize);

	free(input_dir_name);
	free(mydirpath);
	return 0;
}

