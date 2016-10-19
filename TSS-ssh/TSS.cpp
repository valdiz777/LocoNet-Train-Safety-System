/*
TSS.c
Authors: Valentine Nwachukwu, Richard Oteri, Michael Jones, Adam Cowell
Date: 2 October 2016
Description: Multithreaded program that provides an anti-collision safety system for Loco trains for uah's train safety system lab.
           It binds (via ssh) to the location where the JMRI software (specified by the ip address) is running and uses the JMRI 
		   Loconet Monitor logs to provide collision prevention for train HO models. It is necessary to have an ssh key generated and
		   added to authorized hosts at the location loconet is being hosted from.
		   See here http://unix.stackexchange.com/questions/23291/how-to-ssh-to-remote-server-using-a-private-key
Inputs: <loconet ipaddress> <debug mode>
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

/* defines

/* globals */
char locodata_dump[1024];
int debug_mode = 0;
int capture_log = 0;

/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
    Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);
    printf("\nLoconet capture stop signal is detected.\nShutting down....\n");
	capture_log = 0;
    fflush(stdout);
}

/* This is our thread function.  It is like main(), but for a parser thread */
void * parser (void* arg)
{   
	char payload[1024], hrs[2],mins[2], secs[2], msecs[3];
	int last_time = -1, time_now = -1;
    while(capture_log) {
		
		//check if this is a fresh data, first convert string to milliseconds
		hrs[0] = locodata_dump[0], hrs[1] = locodata_dump[1];
		mins[0] = locodata_dump[3], mins[1] = locodata_dump[4];
		secs[0] = locodata_dump[6], secs[1] = locodata_dump[7];
		msecs[0] = locodata_dump[9], msecs[1] = locodata_dump[10], msecs[2] = locodata_dump[11];
		
		// time_now = (((hours*3600) + (minutes * 60) + seconds) * 1000) + milliseconds
		time_now = (((atoi(hrs) * 3600) + (atoi(mins) * 60) + atoi(secs)) * 1000) + atoi(msecs);
		
		// now check freshness
		if (time_now > last_time) {
			last_time = time_now;
			strncpy(payload, locodata_dump, 1024);
			if (debug_mode) printf("PARSER PAYLOAD RECEIVED:%s\n", payload);
    
			//Start Parsing string here
			char occupancy_filter[] = " () is Hi";
			char * pch;
			
			if ((pch=strstr(payload,occupancy_filter)) != NULL){
				int IndxStart = (int) (pch - payload);
				char LSstr[4]; int LSNum;
		
				LSstr[0] = payload[IndxStart - 4];
				LSstr[1] = payload[IndxStart - 3];
				LSstr[2] = payload[IndxStart - 2];	
				LSstr[3] = payload[IndxStart - 1];			
				LSNum = atoi(LSstr);
				int section = ((LSNum+16-1)/16) % 10;
				section = (section == 0)? 16 : section;
				int bdNum = (LSNum%16);
				bdNum = (bdNum == 0)? 16 : bdNum;

				printf("LSNum:%d, Occupancy at Track Section: %d-%d\n", LSNum, section, bdNum);
			}			
			
			//Send to Anti-Collision thread here
		}
    }
    pthread_exit(NULL);
    
}

/* LocoNet DataManager */
int main(int argc, char *argv[])
{

    if (argc < 2 || argc > 3) {
        printf("Usage: ./TSS <loconet ipaddress> <debug mode>\n\texample: TSS 10.1.126.56 or TSS 10.1.126.56 1 to enable debug mode\nMake sure that your ssh key has been added to loconet's authorized keys\nSee here http://unix.stackexchange.com/questions/23291/how-to-ssh-to-remote-server-using-a-private-key\n");
        exit(-1);
    } else if (argc == 3) {
		debug_mode = 1;
		printf("DEBUG_MODE ENABLED...\n\n");
	}

    /* Set the SIGINT (Ctrl-C) signal handler for this thread to sigintHandler */
    signal(SIGINT, sigintHandler);
    
    printf("Booting up Train Safety System LocoNet Data Manager....\n\tPress Ctrl-C twice to exit\n\n");
	
	pthread_t tID;
    char cmd[256],buf[1024], loconet_addr[64];
	sprintf(loconet_addr, "%s", argv[1]);
	
    sprintf(cmd, "ssh -t pi@%s \"tail -f ~/JMRI/monitorLog.txt\"", loconet_addr);
    FILE *loco_listen = popen(cmd, "r");
    
    if (loco_listen == NULL) //Condition (cond.1) --loconet is not on
    {
        printf("Error during connection to loconet, loconet appears to be down\n");
    } 
    else 
    {
		/* create worker thread - not in an if block right now because
		we honestly don't care for packet loss, following udp design here */
        capture_log = 1;      
		pthread_create(&tID, NULL, parser , NULL);
	
        /*Here we will continously poll the monitorLog file for data
		* read each line of incoming text */
        while(fgets(buf, sizeof(buf), loco_listen) != 0) 
        {            
			if(strcmp(locodata_dump,buf) != 0) strncpy(locodata_dump, buf,1024);
			if (debug_mode) printf("INPUT PAYLOAD RECEIVED FROM %s <----> %s\n", loconet_addr,locodata_dump);
        }
		
		pthread_join(tID, NULL);
		fclose(loco_listen);
		exit(0);
    }
	
	return 1;
}




