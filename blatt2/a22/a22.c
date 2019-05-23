#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

/**
 * Aufgabe A22: Warten und beenden von Kindprozessen
 * Ändern Sie das Programm aus Aufgabe A21 so, dass
 * - Sie es mit vier verschiedenen Zeichenketten als Übergabeparametern starten.
 * Der Elternprozess soll
 * - einmal das erste Übergabeargument ausgeben und dann
 * - drei Kindprozesse starten, die ihrerseits das zweite, dritte bzw. vierte Übergabeargument
 * ausgeben.
 * Kindprozess 1 und 2 laufen wie in Aufgabe A21 in einer Endlosschleife.
 * Der dritte Kindprozess ist so abzuändern, dass er
 * - eine Sekunde wartet (sleep()-Systemaufruf benutzen),
 * - sich dann selbst regulär beendet (exit()-Systemaufruf oder verlassen der main()-
 * Funktion mit return) und
 * - als Returnwert 2 zurück gibt.
 * Der Elternprozess soll nach Starten aller Kindprozesse in der nachfolgend angegebenen
 * Reihenfolge tun:
 * 1. mittels des sleep()-Systemaufrufs 2 Sekunden warten,
 * 2. den ersten Kindprozess mit dem Signal SIGTERM (numerisch 15) beenden (kill()-
 * Systemaufruf benutzen),
 * 3. den zweiten und dritten Prozess mit dem Signal SIGKILL (numerisch 9) beenden
 * 4. und dann erst den jeweiligen Endestatus der Kindprozesse abholen und ausgeben (verwenden
 * Sie dabei den wait()-Systemaufruf) und
 * 5. sich abschließend selbst beenden.
 * Untersuchen und vergleichen Sie die Endestatuswerte der Kindprozesse.
*/

#define RED "\033[1;31m"  
#define DEFAULT "\x1B[37m"  //white
#define GREEN "\033[1;32m"
#define MAGENTA "\033[1;35m"
#define ERROR_LAUNCH "%sYou have to enter 4 different strings as command line arguments!%s\n"
#define ERROR_FORK "%sERROR AT FORK%s"
#define ERROR_WAIT "%sERROR AT WAIT%s"
#define COUNT_OF_CHILDS 3
int main(int argc,char* argv[])
{
	fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);
        if(argc!=5)
	  {
           fprintf(stderr,ERROR_LAUNCH,RED,DEFAULT);
           exit(EXIT_FAILURE);
	  }	
	printf("\n");
        fprintf(stdout,"%sparent: %d parameter:%s%s\n",GREEN,getpid(),argv[1],DEFAULT);

	int* pids = (int*)malloc(COUNT_OF_CHILDS*sizeof(int));//to store pids of childs
        int pid,status;
	//son 1 & 2: running in endless loop
	for(int i=0;i<2;i++)
	{
		pid = fork();
		if(pid==0)
		{
		/*child**/
		fprintf(stdout,"%sson: %d from father %d, parameter:%s%s\n",GREEN,getpid(),getppid(),argv[i+2],DEFAULT);
                while(1);
		}
		else if(pid>0)
		{
	         /**parent*/
		pids[i]=pid;
		}
                else
                {
                fprintf(stderr,ERROR_FORK,RED,DEFAULT);
		}
	}

//son 3: waiting one second and then terminates itself (called exit(2))
        pid = fork();
	if(pid==0)
	{
	fprintf(stdout,"%sson: %d from father %d, parameter:%s%s\n",GREEN,getpid(),getppid(),argv[4],DEFAULT);
        fprintf(stdout,"%sWaiting one second!%s\n",GREEN,DEFAULT);
	sleep(1);
        fprintf(stdout,"%sI have finished!%s\n",GREEN,DEFAULT);
	exit(2);
	}
	else if(pid > 0)
	{
	/*parent**/
	pids[2]=pid;
	sleep(2);//1)sleeping two seconds
        /*2)**/
	fprintf(stdout,"%sterminate <15> first son with pid %d%s\n",GREEN,pids[0],DEFAULT);
	kill(pids[0],SIGTERM);
	fprintf(stdout,"%sterminate <9> second son with pid %d%s\n",GREEN,pids[1],DEFAULT);
	kill(pids[1],SIGKILL);
	fprintf(stdout,"%sterminate <9> third son with pid %d%s\n",GREEN,pids[2],DEFAULT);
	kill(pids[2],SIGKILL);

	/*4) make sure childs are deads:**/
	for(int i=0;i <3;i++)
	{
         if((pids[i]=wait(&status))==-1)
	   {
           fprintf(stderr,ERROR_WAIT,RED,DEFAULT);
           } 
           else
	      {
              if(WIFEXITED(status))
                { //son terminated normally, that means calling exit(2) or exit(3)
                fprintf(stdout,"%sson with pid %d exited with status %d%s\n",GREEN,pids[i],WEXITSTATUS(status),DEFAULT);
                }
              else if(WIFSIGNALED(status)) 
                     {//son was ended by signal
                     fprintf(stdout,"%sson with pid %d ended because of signal: %d%s\n",GREEN,pids[i],WTERMSIG(status),DEFAULT);
                     }
             else
                { 
                 fprintf(stdout,"%sson with pid %d didn't end normally: status: %d%s\n",RED,pids[i],WEXITSTATUS(status),DEFAULT);
                }
              }

	}	

        free(pids);
        exit(EXIT_SUCCESS);
	}
        else
	{
        fprintf(stderr,ERROR_FORK,RED,DEFAULT);
	}


return 0;
}
