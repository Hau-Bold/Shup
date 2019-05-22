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

#define MSG_SIGTERM "SIGTERM <15> received.\n"
#define MSG_SIGINT "SIGINT <2> received.\n"

void handleSignal(int signum);

#define RED "\033[;31m"
#define DEFAULT "\x1B[0m"

int main(int argc,char* argv[])
{
	int pid = getpid();
        printf(RED);
	printf("running with pid:%d\n",pid);
	printf("enter kill %d to terminate this program\n",pid);
	printf("enter ctrl + C to see that you can't kill process %d in this way.\n",pid);
        printf(DEFAULT);
        signal(SIGTERM,handleSignal);
        signal(SIGINT,handleSignal);
	for(;;);
	return 0;
}

void handleSignal(int signum)
{
printf(RED);
	switch(signum)
	{	

	case 15:
	 printf("%s\n",MSG_SIGTERM);
	 printf("I kill myself. Bye Bye father %d.\n",getppid());
         printf(DEFAULT);
         exit(0);
	break;
		
        case 2:
         printf(RED);
         printf(MSG_SIGINT);
         printf("you see that you can't kill me in this way!\n");
        break;

	default:
	 printf("received number: %d\n",signum);
        //do nothing
	break;
	}
        printf(DEFAULT);
}
