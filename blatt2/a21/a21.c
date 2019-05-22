#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
/*
<summary>
* 1)mittels fork wird identischer Prozess gestartet
* 2)Beide Prozesse geben pid sowie pid des Vaters aus und laufen daraufhin in Endlosschleife
* 3) Programm soll im Hintergrund gestartet werden:
* ~ wird Vater mittels kill -9 pid beendet, übernimmt init die Rolle des Vaters
</summary>
**/

//getting fathers pid for process: ps j pid


//kill process : kill -9 <pid>
//running in back ground : command &
//verliert Kind Vater, wird Init Prozess Ersatzvater(dieser erlöst auch alle Kindprozesse im Zustand Zombie)
#define GREEN "\033[1;32m"
#define DEFAULT "\x1B[37m"
#define RED "\033[1;31m"
#define MAGENTA "\033[1;35m"
#define ERROR_FORK "%sERROR AT FORK%s"
int main(int argc,char* argv[])
{
	fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);
	int pid=fork();
	printf("\n");
	if(pid==0)
	{
	fprintf(stdout,"%srunning sun:pid:%d, pid of father:ppid: %d%s\n",GREEN,getpid(),getppid(),DEFAULT);
	}
	else if(pid>0)
	{
	fprintf(stdout,"%srunning father:pid:%d, pid of father:ppid:%d%s\n",GREEN,getpid(),getppid(),DEFAULT);
	}
        else
        {
        fprintf(stderr,ERROR_FORK,RED,DEFAULT);
        exit(EXIT_FAILURE);
        }

	while(1);
return 0;
}

