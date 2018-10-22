////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        main.c
// Other Files:      listPid.c pidCheck.c listPid.h pidCheck.h
// Semester:         CS 537 Fall 2018
//
// Author:           Youmin Han
// Email:            youmin.han@wisc.edu
// CS Login:         youmin
//
// Author:           Xianjie Zheng
// Email:            xzheng97@wisc.edu
// CS Login:         xianjie
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NULL
//
// Online sources:   NULL
//
//////////////////////////// 80 columns wide ///////////////////////////////////


#include "pidCheck.h"
#include "listPid.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>
#define buffersize 1024 // Define the buffersize to a large number



/*
 * It will read a variety of information about one or more running programs
 * (processes) on the computer, and then display that information.
 */
int main(int argc, char* argv[]) {
    char choice;
    char *token, *token1, *token2;
    FILE *file;
    // Default value of different options
    int U = 1;
    int s = 0;
    int S = 0;
    int v = 0;
    int c = 1;
    int p = 1;
    
    int pvalid = 0;   // flag to determine if user has entered -p option
    int pcounter = 0; // count how many times -p option runs
    
    // vars that store information about a process
    int stime = 0;
    int utime = 0;
    int size = 0;
    int pid = 0;
    char stat[buffersize];
    char cmdline[buffersize];
    char cmdfinal[buffersize];
    
    
    // Parse the command line arguments: -p, -s, -U, -S, -v, -c
    while ((choice = getopt(argc, argv, "p:s::U::S::v::c::")) != -1) {
        switch (choice) {
                
            case 'p':
                if( pcounter >= 1){   // Multiple -p input, print error message
                    fprintf(stderr, "Multiple -p options\n");
                    exit(EXIT_FAILURE);
                    break;
                }
                
                pvalid = 1;
                pcounter += 1;
                
                // Make sure there is an int argument after -p
                if( (optarg != NULL) && (atoi(optarg) != 0)) {
                    
                    // Check if system has the same PID in /proc
                    if( pidCheck(optarg) == 1){
                        
                        char path[buffersize];
                        strcpy(path, "/proc/");
                        strcat(path, optarg);
                        strcat(path, "/stat");
                        
                        // Open PID's stat in /proc
                        file = fopen(path, "r");
                        char statline[buffersize];
                        
                        // Read PID's stat file
                        if(fgets(statline, sizeof(statline), file)) {
                            token = strtok(statline, " ");
                            for(int i=0; i<2; i++){
                                token = strtok(NULL, " ");
                            }
                            // store stat
                            strcpy(stat, token);
                            for(int i=0; i<11; i++){
                                token = strtok(NULL, " ");
                            }
                            // store utime
                            utime = atoi(token);
                            token = strtok(NULL, " ");
                            // store stime
                            stime = atoi(token);
                        }
                        fclose(file);
                        
                        
                        char vmsize[buffersize];
                        char vmline[buffersize];
                        strcpy(vmsize, "/proc/");
                        strcat(vmsize, optarg);
                        strcat(vmsize, "/statm");
                        // Open PID's statm in /proc
                        file = fopen(vmsize, "r");
                        // Read PID's statm file
                        if(fgets(vmline, sizeof(vmline), file)) {
                            token1 = strtok(vmline, " ");
                            // Store vmemory size
                            size = atoi(token1);
                        }
                        fclose(file);
                        
                        char cmdsize[buffersize];
                        strcpy(cmdsize, "/proc/");
                        strcat(cmdsize, optarg);
                        strcat(cmdsize, "/cmdline");
                        // Open PID's cmdline in /proc
                        file = fopen(cmdsize, "r");
                        
                        // Read PID's cmdline
                        fgets(cmdline, sizeof(cmdline), file);
                        // Store PID's cmdline to cmdfinal
                        token2 = strtok(cmdline, "\0");
                        while(token2 != NULL){
                            strcat(cmdfinal, token2);
                            token2 = strtok(NULL, "\0");
                        }
                        fclose(file);
                        
                        // Store current process PID
                        pid = atoi(optarg);
                        break;
                    }
                    // Situation when cannot find PID in /proc
                    else{
                        p = 0; // change p flag
                        break;
                    }
                }
                // No integer or argument after -p
                else {
                    fprintf(stderr, "Invalid argument for p\n");
                    exit(EXIT_FAILURE);
                    break;
                }
                break;
                
            case 's':
                // -s- is valid but has no effect
                if((optarg != NULL) && (strcmp(optarg, "-") == 0)){
                    s = 0;
                    break;
                }
                // -s option
                else if (optarg == NULL){
                    s = 1;
                    break;
                }
                // other error cases
                fprintf(stderr, "Check your argument for s\n");
                exit(EXIT_FAILURE);
                break;
                
            case 'U':
                // -U- turns this option off
                if((optarg != NULL) && (strcmp(optarg, "-") == 0)){
                    U = 0;
                    break;
                }
                // -U option
                else if (optarg == NULL)
                    break;
                // Other error cases
                fprintf(stderr, "Check your argument for U\n");
                exit(EXIT_FAILURE);
                break;
                
            case 'S':
                // -S- is valid but has no effect
                if((optarg != NULL) && (strcmp(optarg, "-") == 0)){
                    S = 0;
                    break;
                }
                // -S option
                else if (optarg == NULL) {
                    S = 1;
                    break;
                }
                // Other error cases
                fprintf(stderr, "Check your argument for S\n");
                exit(EXIT_FAILURE);
                break;
                
            case 'v':
                // -v- is valid but has no effect
                if((optarg != NULL) && (strcmp(optarg, "-") == 0)){
                    v = 0;
                    break;
                }
                // -v option
                else if (optarg == NULL) {
                    v = 1;
                    break;
                }
                // Other error cases
                fprintf(stderr, "Check your argument for v\n");
                exit(EXIT_FAILURE);
                break;
                
            case 'c':
                // -c- turns this option off
                if((optarg != NULL) && (strcmp(optarg, "-") == 0)){
                    c = 0;
                    break;
                }
                // -c option
                else if (optarg == NULL)
                    break;
                // Other error cases
                fprintf(stderr, "Check your argument for c\n");
                exit(EXIT_FAILURE);
                break;
                
            default:
                // Illegal input
                fprintf(stderr, "Illegal argument for this program\n");
                exit(1);
        }
    }
    
    //edge case handler when useless arguments are entered
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
    // No argument or no pid is given
    else if(pvalid == 0 || argc == 1){
        listPid(s,U,S,v,c);
    }
    // Cannot find PID in /proc
    else if( p == 0){
        printf("Cannot find the PID in /proc. All user processes are:\n");
        listPid(s,U,S,v,c);
    }
    // Normal case
    else {
        if( p == 1){
            printf("%d: ", pid);
            if (s == 1) printf("%s ", stat);
            if (U == 1) printf("utime=%d ", utime);
            if (S == 1) printf("stime=%d ", stime);
            if (v == 1) printf("vmemory=%d ", size);
            if (c == 1) printf("[%s]", cmdfinal);
            printf("\n");
        }
    }
}
