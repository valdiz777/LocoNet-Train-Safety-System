/*
TSS_Parser_Tester.c
Authors: Valentine Nwachukwu, Richard Oteri, Michael Jones, Adam Cowell
Date: 2 October 2016
Description: Multithreaded program that provides unit & integration testing for the TSS loco parsing functionality
		   for Loco trains for uah's train safety system lab. It uses JMRI Loconet Monitor logs to 
		   detect occupany information for train HO models. Test files are usually captured log files
		   from runs. This program thereby provides a means to playback lab runs.
Inputs: <Loconet Monitor logfile>
Outputs:
Comments:
Version:1.0
*/

#include <stdio.h>
#include<time.h> //I'm sure we'll need this soon enough, leaving it here for this sprint vcn
#include <stdlib.h>
#include <signal.h> //
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <pthread.h>

/* defines */
typedef struct locodata_struct{
    char *payload;
} locodata_dump;


/* control sensor read behavior. Default: Sync mode */

/* globals */

/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
    Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);
    printf("\nLoconet capture stop signal is detected.\nShutting down....\n");
    fflush(stdout);
}

/* This is our thread function.  It is like main(), but for a parser thread */
void * parser (void * locodata)
{   

    //Capture loco thread data
    locodata_dump freshDump = *((locodata_dump*)locodata);
    
    
    //take data string here 
    char *payload = freshDump.payload;
    
    printf("The Input file:%s\n", payload);
	
	/* Begin testing */
    FILE *fp;
	fp = fopen(payload, "r");
	char buff[1024];
	while(fgets(buff, sizeof(buff), fp) != 0) {
		char occupancy_filter[] = " () is Lo";
		char * pch;
		if ((pch=strstr(buff,occupancy_filter)) != NULL){
			int IndxStart = (int) (pch - buff);
			char LSstr[4]; int LSNum;
			
			LSstr[0] = buff[IndxStart - 4];
			LSstr[1] = buff[IndxStart - 3];
			LSstr[2] = buff[IndxStart - 2];	
			LSstr[3] = buff[IndxStart - 1];			
			LSNum = atoi(LSstr);
			int section = ((LSNum+16-1)/16) % 10;
			section = (section == 0)? 16 : section;
			int bdNum = (LSNum%16);
			bdNum = (bdNum == 0)? 16 : bdNum;
			printf("LSNum:%d, Occupancy at Track Section: %d-%d\n", LSNum, section, bdNum);
			printf("%s\n", buff);
		}		
	}
    
    //Test send to Anti-Collision thread here
    
    pthread_exit(NULL);
    
}

/* LocoNet Parser Test Suite Launcher */
int main(int argc, char *argv[])
{

    if (argc < 2 || argc > 2) {
        printf("Usage: ./TSS_PTest <Loconet Monitor logfile> \n\texample: TSS_PTest monitorLog.txt\n");
        exit(-1);
    }

    /* Set the SIGINT (Ctrl-C) signal handler for this thread to sigintHandler */
    signal(SIGINT, sigintHandler);
    
    printf("Booting up Train Safety System LocoNet Parser Test Suite....\n");

    pthread_t tID;
   
    locodata_dump data;
    data.payload = argv[1];
        
    /* create worker thread - not in an if block right now because
    we honestly don't care for packet loss, following udp design here */
               
    pthread_create(&tID, NULL, parser , &data);
    
    pthread_join(tID, NULL);
	
	return 1;
}




