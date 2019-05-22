#ifndef __UTILS_H__
#define __UTILS_H__

#include<errno.h> //set but execv
#include<stdio.h>//for fprintf,getchar,
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

#include "constants.h"

/*
 *prints user@host:cwd:promt
 *
 * @param user - the user
 *
 * @param host - the host
 *
 * @param cwd - the current working directory
 * 
 * */
void printSettingsInfomation(char* user, char* host,char* cwd);

/*
* yields the promt in dependency of user
**/
char* getPrompt(char* user);

/*
*to add an escape sequence to a received sring
*
*@param toEscape - the string to add the escape sequence
*
**/
void escape(char* toEscape);

/*
*read a line and saves it in a buffer
*
*@return the buffer
*
**/
char* readLine(void);

 /**splits a string by passed delimiters
 *
 * @param string - the received string which is desired to be splitted
 *
 * @param delimiter - the delimiter
 *
 * @param ctArgs - parameter is passed as pointer to hold the derived count of arguments
 *
 * @return -  the string splitted by delimeter as an array of char arrays
 */
char** split(char* string,char* delimiter,int* ctArgs);


char* getPathFromTree(char* arg);

/*
 *launches the received command
 *
 * @param argc - count of arguments
 *
 * @param argv - the argument values
 *
 * **/
void  launchArguments(int argc,char* argv[]);

#endif
