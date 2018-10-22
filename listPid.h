////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        listPid.h
// Other Files:      listPid.c main.c pidCheck.c pidCheck.h
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


/*
 * Look through /proc to find the list of processes belonging to the user and
 * print out any information that is needed.
 *
 * @param: s, U, S, v, c
 * flags of different options
 */
void listPid(int s, int U, int S, int v, int c);
