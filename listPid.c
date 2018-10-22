////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        listPid.c
// Other Files:      main.c pidCheck.c listPid.h pidCheck.h
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
 * Look through /proc to find the list of processes belonging to the user and
 * print out any information that is needed.
 */
void listPid(int s, int U, int S, int v, int c){
    char *token, *cmdtoken, *token1;
    FILE *file, *file1, *file2, *file3;
    // vars used to read directory
    struct dirent *entry;
    DIR *dir;
    // vars that store information about process
    char cmdline[buffersize];
    char cmdfinal[buffersize];
    int utime = 0;
    int size = 0;
    char stat[buffersize];
    
    // Open directoy failed
    if ((dir = opendir("/proc")) == NULL){
        perror("opendir() error");
        return;
    }
    else {
        // Read all the folders in /proc
        while ((entry = readdir(dir)) != NULL){
            // Filter out non-process folders
            if(atoi(entry->d_name) != 0){
                
                char path[buffersize];
                strcpy(path, "/proc/");
                strcat(path, entry->d_name);
                strcat(path, "/status");
                
                // Open the PID's status
                file = fopen(path, "r");
                
                char line[buffersize];
                int i = 0; // Counter for line
                // Read PID's status by lines
                while (fgets(line, sizeof(line), file)) {
                    i++;
                    if(i == 9 )  // UID is located at line 9
                    {
                        strtok(line, "\t");
                        // Store process's UID
                        token = strtok(NULL, "\t");
                        
                        // Compare UID with user UID
                        if((unsigned int)atoi(token) == (unsigned int)getuid()){
                            char path[buffersize];
                            strcpy(path, "/proc/");
                            strcat(path, entry->d_name);
                            strcat(path, "/stat");
                            
                            // Open PID's stat file
                            file1 = fopen(path, "r");
                            char statline[buffersize];
                            // Read PID's stat
                            fgets(statline, sizeof(statline), file1);
                            token = strtok(statline, " ");
                            for(int i=0; i<2; i++){
                                token = strtok(NULL, " ");
                            }
                            // Store stat
                            strcpy(stat, token);
                            for(int i=0; i<11; i++){
                                token = strtok(NULL, " ");
                            }
                            // Stroe utime
                            utime = atoi(token);
                            // Store stime
                            token = strtok(NULL, " ");
                            fclose(file1);
                            
                            char vmsize[buffersize];
                            char vmline[buffersize];
                            strcpy(vmsize, "/proc/");
                            strcat(vmsize, entry->d_name);
                            strcat(vmsize, "/statm");
                            // Open PID's statm file
                            file3 = fopen(vmsize, "r");
                            // Read first line of PID's statm
                            fgets(vmline, sizeof(vmline), file3);
                            token1 = strtok(vmline, " ");
                            // Store the vmemory size
                            size = atoi(token1);
                            fclose(file3);
                            
                            char cmdsize[buffersize];
                            strcpy(cmdsize, "/proc/");
                            strcat(cmdsize, entry->d_name);
                            strcat(cmdsize, "/cmdline");
                            // Open PID's cmdline
                            file2 = fopen(cmdsize, "r");
                            // Read PID's cmdline
                            fgets(cmdline, sizeof(cmdline), file2);
                            cmdtoken = strtok(cmdline, "\0");
                            strcpy(cmdfinal,"");
                            while(cmdtoken != NULL){
                                // Store cmdline into cmdfinal
                                strcat(cmdfinal, cmdtoken);
                                cmdtoken = strtok(NULL, "\0");
                            }
                            fclose(file2);
                            
                            // Print out the required information by flags
                            printf("%s: ",entry->d_name);
                            if (s == 1) printf("%s ", stat);
                            if (U == 1) printf("utime=%d ", utime);
                            if (S == 1) printf("stime=%s ", token);
                            if (v == 1) printf("vmemory=%d ", size);
                            if (c == 1) printf("[%s]", cmdfinal);
                            printf("\n");
                            strcpy(cmdfinal,"");
                        }
                        break;
                    }
                }
                fclose(file);
            }
        }
        closedir(dir);
        return;
    }
}


