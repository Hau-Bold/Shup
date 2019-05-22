#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
//kill process : kill -9 <pid>
//running in back ground : command &
//verliert Kind Vater, wird Init Prozess Ersatzvater(dieser erlöst auch alle Kindprozesse im Zustand Zombie)
//kill send SIGTERM by default
//sigterm: process may react (if 9 process can't react)
//

/**
Aufgabe 23 Im Programm auf Signale reagieren
Entwickeln Sie ein Programm, das auf die Signale SIGTERM und SIGINT reagiert. Das Programm soll nach dem Start eine Meldung auf stdout ausgeben, dass es gestartet wurde und wie es sich verhält, d.h. auf welche Signale es wie reagiert. Danach soll das Programm in einer Endlosschleife laufen. Das Programm soll bei Eintreffen eines der Signale SIGTERM oder SIGINT auf der Konsole ausgeben, dass das entsprechende Signal eingetroffen ist. Nachdem das dritte Mal ein Signal SIGTERM oder SIGINT eingetroffen ist, soll sich das Programm selbst beenden und als Endestatus den Wert 5 zurück geben.
*/
#define MSG_SIGTERM "SIGTERM <15> received.\n"
#define MSG_SIGINT "SIGINT <2> received.\n"

#define MSG_SIGTERM_VERBOSE "%sEnter kill %d to terminate this program if you run it in background%s\n"
#define MSG_SIGINT_VERBOSE "%sEnter ctrl + C to see that you can't kill process %d in this way if you run it in foreground.%s\n"
#define MSG_EXIT "%sOnly after the third received SIGINT or SIGTERM we'll exit, returning 5%s\n"

#define RED "\033[;31m"
#define DEFAULT "\x1B[0m"
#define MAGENTA "\033[1;35m"

void handleSignal(int signum);
int sig_num_15=0;
int sig_num_2=0;
int main(int argc,char* argv[])
{

	fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);
	int pid = getpid();
	printf("running with pid:%d\n",pid);

	fprintf(stdout,MSG_SIGTERM_VERBOSE,RED,pid,DEFAULT);
	fprintf(stdout,MSG_SIGINT_VERBOSE,RED,pid,DEFAULT);
	fprintf(stdout,MSG_EXIT,RED,DEFAULT);

        signal(SIGTERM,handleSignal);
        signal(SIGINT,handleSignal);
	for(;;);
	return 0;
}

void handleSignal(int signum)
{
	switch(signum)
	{	

	case 15:
         sig_num_15++;		
	 printf("%s\n",MSG_SIGTERM);

	if(sig_num_15==3)
	  {	
	 printf("I kill myself. Bye Bye father %d.\n",getppid());
	  exit(5);
	  }
	break;
		
        case 2:
	 sig_num_2++;
         printf(MSG_SIGINT);

	 if(sig_num_2==3)
	 {	
	 printf("I kill myself. Bye Bye father %d.\n",getppid());
	 exit(5);
	 }

         printf("you see that you can't kill me in this way!\n");
        break;

	default:
	 printf("received number: %d\n",signum);
        //do nothing
	break;
	}
}
