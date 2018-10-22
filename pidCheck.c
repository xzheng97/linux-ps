////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        pidCheck.c
// Other Files:      listPid.c main.c listPid.h pidCheck.h
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
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#define buffersize 1024 // Define the buffersize to a large number

/*Check valid pid*/
int pidCheck(char *user_input){
    
    DIR * dirr;
    char way[buffersize];
    strcpy(way,"/proc/");
    strcat(way, user_input);
    dirr = opendir(way);
    // Check if the process folder is presented in /proc
    if(dirr){
        closedir(dirr);
        // Case when find out process folder, return 1
        return(1);
    }
    // Case when cannot find process folder, return 0
    return(0);
}

