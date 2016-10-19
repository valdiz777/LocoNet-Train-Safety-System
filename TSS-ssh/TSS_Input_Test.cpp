/*
TSS.c
Authors: Valentine Nwachukwu, Richard Oteri, Michael Jones, Adam Cowell
Date: 2 October 2016
Description: Multithreaded program that provides unit & integration testing for the TSS loco ssh communication functionality
		   for Loco trains for uah's train safety system lab. It uses ssh to connect to JMRI LocoNet Monitor log files. It is
		   necessary to have an ssh key generated and added to authorized hosts at the location loconet is being hosted from.
		   See here http://unix.stackexchange.com/questions/23291/how-to-ssh-to-remote-server-using-a-private-key
		   This is a pure connectivity and load tester.
Inputs: <loconet ipaddress>
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
	printf("%s\n", payload);
    
    pthread_exit(NULL);
    
}

/* LocoNet DataManager */
int main(int argc, char *argv[])
{

    if (argc < 2 || argc > 2) {
        printf("Usage: TSS_ITest <loconet ipaddress>\n\texample: TSS_ITest 10.1.126.56\nMake sure that your ssh key has been added to loconet's authorized keys\nSee here http://unix.stackexchange.com/questions/23291/how-to-ssh-to-remote-server-using-a-private-key\n");
        exit(-1);
    }

    /* Set the SIGINT (Ctrl-C) signal handler for this thread to sigintHandler */
    signal(SIGINT, sigintHandler);
    
    printf("Booting up Train Safety System LocoNet Data Manager....\n");


    /*Here we will continously poll the monitorLog file for data*/
    pthread_t tID;
    
    char cmd[256];
	//TODO: we probably don't want to keep hardcoding where the location for this log file is
    sprintf(cmd, "ssh -t pi@%s \"tail -f ~/JMRI/monitorLog.txt\"", argv[1]);
    FILE *loco_listen = popen(cmd, "r");
    char buf[1024];
    
    if (loco_listen == NULL) //Condition (cond.1) --loconet is not on
    {
        printf("Error during connection to loconet, loconet appears to be down\n");
    } 
    else 
    {

        //read each line of incoming text
        while(fgets(buf, sizeof(buf), loco_listen) != 0) 
        {
            printf("Received:%s\n", buf);
            locodata_dump data;
            data.payload = buf;
        
            /* create worker thread - not in an if block right now because
             we honestly don't care for packet loss, following udp design here */
               
             pthread_create(&tID, NULL, parser , &data);
        }
    }
    
    pthread_join(tID, NULL);
	
	return 1;
}




