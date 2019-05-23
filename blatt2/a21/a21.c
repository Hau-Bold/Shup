#include<stdio.h>
#include<sys/types.h>//fork
#include<unistd.h>//fork
#include<stdlib.h>//EXIT_FAILURE.....

/**
Aufgabe A21: Abspaltung von Prozessen
Schreiben Sie ein C-Programm, das mittels des fork()-Systemaufrufs einen weiteren identi-schen Prozess startet. Beide Prozesse sollen dann ihre Prozessnummer sowie die Prozessnummer des Vaterprozesses ausgeben (getpid()- bzw. getppid()-Systemaufrufe) und darauf in eine Endlosschleife laufen.
Starten Sie Ihr Programm im Hintergrund und beobachten Sie den Prozesszustand mit dem ps-Kommando über die Kommando-Shell.

Beenden Sie dann die Prozesse mit dem kill-Kommando (kill -9 <pid>) von der Shell aus, wobei Sie nach Beendigung des ersten Prozesses wiederum den Prozesszustand mit ps beobachten.
Wiederholen Sie den Ablauf nochmals, wobei Sie nun die beiden Prozesse in umgekehrter Reihenfolge beenden.
*/



#define GREEN "\033[1;32m"
#define DEFAULT "\x1B[37m"
#define RED "\033[1;31m"
#define MAGENTA "\033[1;35m"
#define ERROR_FORK "%sERROR AT FORK%s"

#define MSG_CHILD "%srunning child: pid:%d, pid of parent:ppid: %d%s\n"
#define MSG_PARENT "%srunning parent:pid:%d, pid of parent:ppid:%d%s\n"

int main(int argc,char* argv[])
{
	fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);
	int pid=fork();
	printf("\n");
	if(pid==0)
	{
	fprintf(stdout,MSG_CHILD,GREEN,getpid(),getppid(),DEFAULT);
	}
	else if(pid>0)
	{
	fprintf(stdout,MSG_PARENT,GREEN,getpid(),getppid(),DEFAULT);
	}
        else
        {
        fprintf(stderr,ERROR_FORK,RED,DEFAULT);
        exit(EXIT_FAILURE);
        }

	while(1);
return 0;
}

