#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h> //include exec family
#include<stdlib.h>  // for perror!
#include<string.h>
#include<errno.h>

/*
Aufgabe A24:
Anderes Programm starten
Entwickeln Sie ein Programm "Starten", das einen Kindprozess erzeugt, der dann sofort mit einem exec-Systemaufruf ein komplett anderes Programm P-neu (etwa eine einfache „Hallo Welt!“-Anwendung) ausführt, während der Elternprozess auf das Ende des Kindprozesses wartet.
Nach Ende des Kindprozesses soll der Elternprozess die PID des Kindprozesses und dessen Endestatus ausgeben. Testen Sie Ihr Programm mit
a. einem existierenden P-neu, welches keine Aufrufparameter benötigt.
b. einem nicht existierenden P-neu.
c. einem existierenden P-neu, welches Aufrufparameter verarbeitet. Die Aufrufparameter von P-neu sollen dabei die Aufrufparameter vom Programm "Starten" sein.
Behandeln Sie entsprechende Fehlermeldungen des exec-Systemaufruf~Programm erzeugt Kindprozess
**/

#define RED "\033[1;31m"
#define DEFAULT "\x1B[37m"
#define GREEN "\033[1;32m"
#define MAGENTA "\033[1;35m"

#define BUFFER_SIZE 256
#define ERROR_FORK "%sERROR AT FORK\n\n%s"
#define ERROR_WAIT "%sERROR AT WAIT\n%s"
#define MSG_PARENT "Parent process\n"
#define MSG_LAUNCH_PROGRAM_NOT_POSSIBLE "%snot able to launch a program without a name\n%s"


int main(int argc,char* argv[])
{
	fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);
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
	       char* new_argv[argc];
               for(int id=0;id < argc-1;id++)
               {
		       new_argv[id]=argv[id+1];
	       }	       
	       new_argv[argc-1]=(char*)NULL;

               int val=execv(new_argv[0],new_argv);
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
	      exit(EXIT_SUCCESS);
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

