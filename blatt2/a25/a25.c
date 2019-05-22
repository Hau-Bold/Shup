#include<errno.h>
#include<stdio.h>
#include<stdlib.h> // for getenv
#include<string.h>
#include<unistd.h>//for gethostname,getcwd
#include "utils.h"
#include "constants.h"

//!to save user
char * user=NULL;


//the buffer to read commands
char * cmdBuffer=NULL;

int argc_child=0;//count of arguments for launching child process

int main(int argc,char* argv[])
{
	fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);
//!to save host
char host[1024];//we choose size large enough, because we don't want to have the truncated host;

//!buffer for current working directory
char cwd[BUFFER_SIZE];

	user=getenv("USER");

	if(!user)
	{
		/**check for error*/
	    if(VERBOSE){
	    	    fprintf(stderr,ERROR_USER,RED,DEFAULT);
	    }
		exit(EXIT_FAILURE);
	}

        gethostname(host,sizeof(host));

	char* ptr = getcwd(cwd, sizeof(cwd));
	
	if(ptr)
	{

	 while(1)
	     {
              /*print user@host:cwdPrompt**/		     
	      printSettingsInfomation(user,host,cwd);

	      /**read entered command,if NULL, nothing happens but a new line*/
              cmdBuffer = readLine();  

              if(cmdBuffer)
              {
		/*split the entered command
		 *since we do not need the cmdbuffer again, we don't need a copy of it (strtok changes the received string)
		 * **/	
                char** arguments = split(cmdBuffer,DELIMITER,&argc_child);
                  
		if(strcmp(arguments[0],EXIT_CMD)==0)
                  { 
                   /**user wants to quit application*/
		    if(VERBOSE)
		    {
	             fprintf(stdout,"%sBye Bye...\n%s",YELLOW,DEFAULT);
		    }

               	    free(arguments);
                    free(cmdBuffer);
                    exit(EXIT_SUCCESS);
                  }
 
                //launch entered command:
                launchArguments(argc_child,arguments);
               
		free(arguments);
                free(cmdBuffer);
              }
	     }
	 }
	else
	{
		if(VERBOSE)
		{
		/*buffer size is to small to save cwd**/
                fprintf(stderr,"%sError of message: %s%s\n",RED,strerror(errno),DEFAULT);
		}
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


