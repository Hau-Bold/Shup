#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h> //include exec family
#include<stdlib.h>  // for perror!
#include<string.h>
#include<errno.h>

/*
<summary>
~Programm erzeugt Kindprozess
~Kindprozess startet mit exec-Aufruf anderes Programm (hier: Test/./Test arguments &)

</summary
**/

#define RED "\033[1;31m"
#define DEFAULT "\x1B[37m"
#define GREEN "\033[1;32m"
#define BUFFER_SIZE 256
#define ERROR_FORK "%sERROR AT FORK\n\n%s"
#define ERROR_WAIT "%sERROR AT WAIT\n%s"
#define MSG_PARENT "Parent process\n"
#define MSG_LAUNCH_PROGRAM_NOT_POSSIBLE "%snot able to launch a program without a name\n%s"
char** getChild_argv(int argc, char* argv[]);
int main(int argc,char* argv[])
{
       char* child_argv;
       int pid = fork();
       int status =0;
	
		
         if(pid < 0)
           {
           /*fork has failed**/
           fprintf(stderr,ERROR_FORK,RED,DEFAULT);
           }
         else if(pid==0)
           {
	    if(argc > 1)
     	      {     
        	char** child_argv =getChild_argv(argc,argv);
               int val=execv(argv[1],argv);
              if(val)
              {
               fprintf(stdout,"%sError of message: %s%s\n",RED,strerror(errno),DEFAULT);
               exit(EXIT_FAILURE);
              } 
                
	      }
            else
	      {
                printf("%d\n",argc);
    	        fprintf(stderr,MSG_LAUNCH_PROGRAM_NOT_POSSIBLE,RED,DEFAULT);
	      }
            }
        else
         {
         /*parent process**/
          fprintf(stdout,"pid:%d\n",pid);
          if((pid=wait(&status))==-1)
	    {
             fprintf(stderr,ERROR_WAIT,RED,DEFAULT);
            }
         else
           {
           if(WIFEXITED(status))
             {
             fprintf(stdout,"%sson with pid %d exited with status %d\n%s",GREEN,pid,WEXITSTATUS(status),DEFAULT);
             }
           else if(WIFSIGNALED(status))
             {
             fprintf(stdout,"%sson with pid %d ended because of signal %d\n%s",GREEN,pid,WTERMSIG(status),DEFAULT);
             }
           else
            {
            fprintf(stdout,"%sson with pid %d, didn't end normally: status: %d\n%s",RED,pid,WEXITSTATUS(status),DEFAULT);
            }
          } 
	}	
}

char** getChild_argv(int argc, char* argv[])
{
   char** response  = malloc(sizeof(char*)*(argc-1));
	for(int i=0;i<argc-1;i++)
	{
	response[i]= malloc(strlen(argv[i+1]));
	strcpy(response[i],argv[i+1]);
	}
    return response;
}

/*
*to add an escape sequence to a received sring
*
*@param toEscape - the string to add the escape sequence
*
*/
void escape(char* toEscape)
{
	int index=0;
	while(toEscape[index]!=0)
		{
		index++;
		}
	toEscape[index]='\0';
}


