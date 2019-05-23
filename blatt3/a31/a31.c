#include <stdio.h>

#include <errno.h>
#include <string.h>
#include <sys/wait.h>
//includes for synchronisation
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
//include for shared memory
#include <sys/shm.h>

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//signal
#include <signal.h>

#include "constants.h"
#include "utils.h"

/*
Aufgabe A31: Klassen von Akteuren mit gleichzeitigem Zugriff - Backup
In einem Computersystem (z. B. Desktop-PC oder Notebook) läuft periodisch mit der Periode p ein Backup-Prozess für die relevanten Teile des Dateisystems. Der Backup-Vorgang arbeitet folgendermaßen:
Wenn der Backup-Prozess läuft, dürfen keine anderen Prozesse (Nutzprozesse) auf das Dateisystem zugreifen und aus Konsistenzgründen keine Dateien geöffnet haben.
Wenn das Backup zu einem Zeitpunkt t wieder ansteht, dürfen Nutzprozesse, welche im Moment Dateien geöffnet haben, noch zu Ende laufen. Solange wird der Backup-Prozesse blockiert.
Wenn nach dem Zeitpunkt t neue Nutzprozesse auf Dateien zugreifen wollen, dann wird diesen neuen Nutzprozessen der Zugriff verweigert. Diese Nutzprozesse warten dann solange, bis der Backup-Prozess beendet ist und greifen danach auf das Dateisystem zu.
Es wird angenommen, dass die Nutzprozesse, welche das Dateisystem verwenden, kurz laufen in Bezug auf die Backup-Periode p. Der Zeitpunkt t für einen nächsten Backup-Vorgang wird aus der Zeit des Beendens des letzten Backup-Prozesses und Addition der Periode p berechnet.
Entwickeln und implementieren Sie für diese Problemstellung unter Linux als C-Programm eine Lösung zur koordinierten Durchführung des Backup-Vorgangs. Implementieren Sie den Backup-Prozess und jeden Nutzprozess als eigenen Prozess. Für die Nutzprozesse soll nur das Verhalten bzgl. der Synchronisation implementiert werden, d.h. im Rahmen dieser Aufgabe haben alle Nutzprozesse das gleiche Verhalten (den gleichen Quellcode). Alle Prozesse geben zu jeder Zeit in geeigneter Weise ihren Zustand aus.
**/

/**4 semaphores*/
int readerId;//to count readers that are accessing filesystem concurrently
int readerCounterId;//to ensure each reader is incrementing the counter atomar
int backUpId;//to ensure that back up process is having exclusive access to file system
int mutexExitId;//to ensure that no more readers will be generated when it is desired to exit application

/*2 shared memories**/
int *readerCounter = 0;//to count the readers using the file system same time
int *pid_BackUp = 0;//to hold the pid of backUp process
int id_shm_Reader,id_shm_BackUp;//corresponding ids

/**semaphore operations*/
struct sembuf up = {0, 1, 0};
struct sembuf down = {0, -1, 0};
struct sembuf waitForZero = {0, 0, 0};

//!holds the durations of processes
int* durations;

/**setting at initialization
 *~loads semaphores
 *~loads shared memory:
 *~prints common information
 * */
void onInit(void);

/*clean up for exiting application
 *called when SIGINT is received
 *~incoming readers are blocked
 *~when backup has finished, wait until last readers have finished
 *~KIll backup
 *clean up semaphores and shared memory
 * **/
void onExit(void);

void backUp(void);

void reader(int duration);


/**
 *to influence the count of reader depending on operation
 *
 * @param operation - the operation to influence the counter:
 *                    +: incremenst the counter
 *                    -: decrements the counter
 *
 * */
void influenceReaderCounter(char operation);

//int pid_BackUp=0;

//signalhandler
void handleSignalBackUp(int signum);

int main(int argc, char *argv[])
{
	onInit();
  
  //generate backup process:
  int pid = fork();
  if (pid < 0)
  {
    fprintf(stderr, ERROR_FORK, RED, DEFAULT);
  }
  else if (pid == 0)
  {
    signal(SIGINT,handleSignalBackUp);
    backUp();
  }
  else
  {
    //store pid of backup:
   *pid_BackUp=pid;
    printf("%d is stored",*pid_BackUp);
  }

  while (1)
  {
    semop(mutexExitId, &waitForZero, 1);

    /*determine count of processes strting in next minute**/
    int countOfProcessesInNextMinute = getNextCountOfProcesses();

    if (countOfProcessesInNextMinute > 0)
    {
      fprintf(stdout, MSG_LAUNCH_PROCESSES, DEFAULT, countOfProcessesInNextMinute, DEFAULT);

      //create readers
      for (int id = 0; id < countOfProcessesInNextMinute; id++)
      {
	      //before a new reader-process may run,it must know its duration 
              int duration =durations[rand()%COUNT_OF_DURATIONS]; 
	      pid = fork();
	      
	      if (pid < 0)
              {
               fprintf(stderr, ERROR_FORK, RED, DEFAULT);
              }
              else if (pid == 0)
              {
              	/**child process*/
               	int val = semop(backUpId, &waitForZero, 1); //if backUp has requestetd the filesystem, readers have to wait
               	if (val < 0)
               	{
               	fprintf(stderr,ERROR_SEMAPHORE,RED,DEFAULT);
               	exit(EXIT_FAILURE);
               	}

              	reader(duration); 
	      }
              else
              {
		      //do nothing, reader will  terminate
              }
      } //end for
    }   //end if
    /*wait for the next minute**/
    sleep(1);
  }
  return 0;
}

void onInit(void)
{
   fprintf(stdout,"%sDiese Lösung wurde erstellt von Michael Krasser%s\n",MAGENTA,DEFAULT);


   /**init durations*/
   durations = (int*)malloc(COUNT_OF_DURATIONS*sizeof(int));
   durations[0]=DURATION_0;
   durations[1]=DURATION_1;
   durations[2]=DURATION_2;
   durations[COUNT_OF_DURATIONS-1]=DURATION_3;
  
   /**setting seed*/
   srand(time(NULL));   

  /**initialize the shared memory to count the readers using the filesystem at same time*/
   id_shm_Reader = shmget(IPC_PRIVATE, 4096, IPC_CREAT | PERMISSION);
   if (id_shm_Reader == -1)
   {
    fprintf(stderr,ERROR_SHARED_MEMORY,RED,DEFAULT);
    exit(EXIT_FAILURE);
   }
   else
   {
	   fprintf(stdout,INFO_SHARED_MEMORY,BOLD_BLUE,id_shm_Reader,DEFAULT);

   }

  //attaches the shared memory segment identified by shmid to the address space of the calling process.
  readerCounter = (int *)shmat(id_shm_Reader, NULL, 0); 
  if (readerCounter == (void *)-1)
  {
    fprintf(stderr, ERROR_SHARED_MEMORY_ATTACHING,RED,DEFAULT);
    exit(EXIT_FAILURE);
  }
  else
  {
	  fprintf(stdout,INFO_SHARED_MEMORY_ATTACHED,BOLD_BLUE,DEFAULT);
  }

  /**initialize the shared memory to tore the pid of backup*/
  id_shm_BackUp = shmget(IPC_PRIVATE, 4096, IPC_CREAT | PERMISSION);
   if (id_shm_BackUp == -1)
   {
    fprintf(stderr,ERROR_SHARED_MEMORY,RED,DEFAULT);
    exit(EXIT_FAILURE);
   }
   else
   {
	   fprintf(stdout,INFO_SHARED_MEMORY,BOLD_BLUE,id_shm_BackUp,DEFAULT);

   }   
  //attaches the shared memory segment identified by shmid to the address space of the calling process.
  pid_BackUp= (int *)shmat(id_shm_BackUp, NULL, 0); 
  if (pid_BackUp == (void *)-1)
  {
    fprintf(stderr, ERROR_SHARED_MEMORY_ATTACHING,RED,DEFAULT);
    exit(EXIT_FAILURE);
  }
  else
  {
	  fprintf(stdout,INFO_SHARED_MEMORY_ATTACHED,BOLD_BLUE,DEFAULT);
  }
   
   /*create mutex to ensure exclusive access to filesystem for backup **/
  backUpId = semget(IPC_PRIVATE, 1, PERMISSION); //gets id
  if(backUpId==-1)
  {
	   fprintf(stderr,ERROR_SEMAPHORE,RED,DEFAULT);
  }
  else
  {
	   fprintf(stdout,INFO_SEMAPHORE,BOLD_BLUE,backUpId,DEFAULT);
  }
  semctl(backUpId, 0, SETVAL, 0);                //initialize it with 0

  /*create semaphore to handle request of readers to filesystem**/
  readerId = semget(IPC_PRIVATE, 1, PERMISSION);
  if(readerId==-1)
  {
	   fprintf(stderr,ERROR_SEMAPHORE,RED,DEFAULT);
  }
  else
  {
	   fprintf(stdout,INFO_SEMAPHORE,BOLD_BLUE,readerId,DEFAULT);
  }

  semctl(readerId, 0, SETVAL, 0);

  /**create semaphore to handle request to readerCounter*/
  readerCounterId = semget(IPC_PRIVATE, 1, PERMISSION);
  if(readerCounterId==-1)
  {
	   fprintf(stderr,ERROR_SEMAPHORE,RED,DEFAULT);
  }
  else
  {
	   fprintf(stdout,INFO_SEMAPHORE,BOLD_BLUE,readerCounterId,DEFAULT);
  }
  semctl(readerCounterId, 0, SETVAL, 0);

  /*create mutex to ensure that no more readers do have access if it is desired to terminate program**/
  mutexExitId = semget(IPC_PRIVATE, 1, PERMISSION);
  if(mutexExitId==-1)
  {
	   fprintf(stderr,ERROR_SEMAPHORE,RED,DEFAULT);
  }
  else
  {
	   fprintf(stdout,INFO_SEMAPHORE,BOLD_BLUE,mutexExitId,DEFAULT);
  }

  semctl(mutexExitId, SETVAL, 0);
}


void reader(int duration)
{
  /*wait until backup has finished**/
  semop(backUpId, &waitForZero, 1);

  
  /*I'm running to use the file system**/
  semop(readerId, &up, 1);             

  influenceReaderCounter(INCREMENT);
   //reader-process is doing it's job
  fprintf(stdout, MSG_CHILD_VERBOSE_START, GREEN, getpid(), duration, DEFAULT);
  sleep(duration);

  influenceReaderCounter(DECREMENT);

  /*reader-process has done it's job**/
  fprintf(stdout, MSG_CHILD_VERBOSE_FINISHED, RED, getpid(), duration, DEFAULT);
  semop(readerId, &down, 1);
  exit(EXIT_SUCCESS);
}

void influenceReaderCounter(char operation)
{
  /*wait until reader counter  may be incremented**/
  semop(readerCounterId, &waitForZero, 1);

  /**request ressource*/
  semop(readerCounterId,&up,1);

  /*influence reader counter**/
  switch(operation)
  {
	  case INCREMENT:
		   (*readerCounter)++;   
		   break;
          case DECREMENT:
		  (*readerCounter)--;
		 break;
	  default:
	  fprintf(stdout,"Not implemented yet: %c",operation);
          break;
  }

  /**release ressource*/
  semop(readerCounterId,&down,1);
}

void backUp(void)
{
  while (1)
  {
    fprintf(stdout, MSG_BACKUP_PROCESS, YELLOW, getpid(), DEFAULT);
    sleep(PERIOD);
    semop(backUpId, &up, 1); //backUp requests access to filesystem
    int runningReaders = *readerCounter;
    fprintf(stdout, MSG_BACKUP_START_0, YELLOW, *readerCounter, DEFAULT);
    //backUp waits until readers have finished
    semop(readerId, &waitForZero, 1);
    if (runningReaders > 0)
    {
      fprintf(stdout, MSG_BACKUP_WAITING, YELLOW, runningReaders, DEFAULT);
    }
    printf("%sNow %d readers are running%s\n", MAGENTA, *readerCounter, DEFAULT);

    fprintf(stdout, MSG_BACKUP_START_1, YELLOW, DEFAULT);
    sleep(DURATION_BACKUP);
    fprintf(stdout, MSG_BACKUP_FINISHED, YELLOW, DEFAULT);
    semop(backUpId, &down, 1); //backUp has finished using filesystem
  }
}

void handleSignalBackUp(int signum)
{
  switch (signum)
  {
  case 2:
	  onExit();

       break;

  default:
    //do nothing
    break;
  }
}


void onExit(void)
{
    fprintf(stdout, MSG_SIGINT, RED, DEFAULT);

    /**first: block new readers:*/
    semop(mutexExitId, &up, 1);
    /**second: wait until back up process has finished*/
    semop(backUpId, &waitForZero, 1);
    
        printf("backup %d is done\n",*pid_BackUp);
    
    /**third: wait until unblocked readers have finished */
    fprintf(stdout,"%sOn Exit I'm waiting  for %d readers to finish%s\n" , YELLOW, *readerCounter, DEFAULT);
    int runningReaders = *readerCounter;
    printf("readerid:%d\n",readerId);
    semop(readerId,&waitForZero, 1);
    fprintf(stdout,"%sOn Exit I waited for %d readers to finish%s\n" , YELLOW, runningReaders, DEFAULT);
    
    printf("%sNow %d readers are running%s\n", MAGENTA, *readerCounter, DEFAULT);
    
    free(durations);
    semctl(backUpId,0,IPC_RMID,0);//clean up backUp Semapore
    semctl(readerId,0,IPC_RMID,0);//clean up backUp Semapore
    semctl(readerCounterId,0,IPC_RMID,0);//clean up backUp Semapore
    semctl(mutexExitId,0,IPC_RMID,0);//clean up backUp Semapore

    semctl(id_shm_Reader,0,IPC_RMID);
    semctl(id_shm_BackUp,0,IPC_RMID);
    
    //kill back up:
    kill(*pid_BackUp, SIGKILL);

}
