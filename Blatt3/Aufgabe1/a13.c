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

// duration of utilze processes
//status

/*
*<summary>
@ int semget(key_t key,int nsems,int semflg)
       key - the magic key
      nsems - count of semaphores in set
     semflg - access: 
            IPC_PRIVATE: opens a private key
            IPC_CREAT: generates a new key
            IPC_EXCL: bitwise with IPC_CREAT: to make sure that a new semaphore will be generatet and not an existing with same id will be used
            if semid is existing, semget quits with errno==EEXIST
  ~returns the System V semaphore set identifier associated with tje argument key


</summary>
*/

int *currentCountOfReaders = 0; // count of readers in shared segment

/**
the buffer to save the pids of running processes;
if a process has finished, its pid will be removed from the buffer
*/

int processesInQueue = 0;
struct sembuf mutex;
int mutexId, semId, mutexKill;

struct sembuf up = {0, 1, 0};
struct sembuf down = {0, -1, 0};
struct sembuf waitForZero = {0, 0, 0};

void backUp(void);
void reader(void);

//!buffer to save pids of running processes
int *pids = NULL;
int currentBufferSize = 0;
void initBuffer(void);
void storePid(int pid, int position);
void waitForProcessesToFinish(void);

//signalhandler
void handleSignal(int signum);

int main(int argc, char *argv[])
{
  //the signalhandler
  //	signal(SIGTERM,handleSignal);
  //        signal(SIGINT,handleSignal);

  int id_shm = shmget(IPC_PRIVATE, 4096, IPC_CREAT | PERMISSION);
  if (id_shm == -1)
  {
    fprintf(stderr, "ERROR shmget");
    exit(EXIT_FAILURE);
  }
  currentCountOfReaders = (int *)shmat(id_shm, NULL, 0); //attaches the shared memory segment identified by shmid to the address space of the calling process.
  if (currentCountOfReaders == (void *)-1)
  {
    fprintf(stderr, "ERROR shmat");
    exit(EXIT_FAILURE);
  }

  /*create mutex to ensure exclusive access to filesystem for backup **/
  mutexId = semget(IPC_PRIVATE, 1, PERMISSION); //gets id
  semctl(mutexId, 0, SETVAL, 0);                //initialize it with 0

  /*create semaphore to handle request of readers to filesystem**/
  semId = semget(IPC_PRIVATE, 1, PERMISSION);
  semctl(semId, 0, SETVAL, 0);

  /*create mutex to ensure that no more readers do habe access if it is desired to kill program**/
  mutexKill = semget(IPC_PRIVATE, 1, PERMISSION);
  semctl(mutexKill, SETVAL, 0);

  // semctl(semid,0,IPC_RMID,0);clean up......
  initBuffer();
  //generate backup process:
  int pid = fork();
  if (pid < 0)
  {
    fprintf(stderr, ERROR_FORK, RED, DEFAULT);
  }
  else if (pid == 0)
  {
    //   semop(backUpId,&waitForZero,1);//if no backUp process has requested ressource, create a back up process
    backUp();
  }
  else
  {
    //store pid of backup:
    storePid(pid, processesInQueue);
  }

  srand(time(0));
  while (1)
  {
    semop(mutexKill, &waitForZero, 1);
    int countOfProcessesInNextMinute = getCountOfProcessesInNextMinute(); /*determine count of processes strting in next minute**/

    if (countOfProcessesInNextMinute > 0)
    {
      fprintf(stdout, MSG_LAUNCH_PROCESSES, DEFAULT, countOfProcessesInNextMinute, DEFAULT);

      //create readers
      for (int id = 0; id < countOfProcessesInNextMinute; id++)
      {
        pid = fork();
        if (pid < 0)
        {
          fprintf(stderr, ERROR_FORK, RED, DEFAULT);
        }
        else if (pid == 0)
        {
          int val = semop(mutexId, &waitForZero, 1); //if backUp has requestetd the filesystem, readers have to wait
          if (val < 0)
          {
            printf("%sERROR%s\n", RED, DEFAULT);
            exit(EXIT_FAILURE);
            /**child process*/
          }
          reader(); //child will terminate
        }
        else
        {
          //store pid:
          processesInQueue++;
          storePid(pid, processesInQueue);
        }
      } //end for
    }   //end if
    sleep(1);
  }
  return 0;
}

void reader(void)
{
  semop(mutexId, &waitForZero, 1);  //request: is backUp using ressource?
                                    /*when I run, I must signal that I'm using the file system**/
  semop(semId, &up, 1);             //inkement (1=# der operation)
  int duration = getNextDuration(); //how long will I run?
  (*currentCountOfReaders)++;       //I'm running, so I increment the cout of readers
  fprintf(stdout, MSG_CHILD_VERBOSE_START, GREEN, getpid(), duration, DEFAULT);
  sleep(duration);
  (*currentCountOfReaders)--; //I have finished reading, so I dekrement
  /*I must signal that I do not need the filesystem any more time**/
  semop(semId, &down, 1);
  fprintf(stdout, MSG_CHILD_VERBOSE_FINISHED, RED, getpid(), duration, DEFAULT);
  exit(EXIT_SUCCESS);
}

void backUp(void)
{
  while (1)
  {
    fprintf(stdout, MSG_BACKUP_PROCESS, YELLOW, getpid(), DEFAULT);
    sleep(PERIOD);
    semop(mutexId, &up, 1); //backUp requests access to filesystem
    int runningReaders = *currentCountOfReaders;
    fprintf(stdout, MSG_BACKUP_START_0, YELLOW, *currentCountOfReaders, DEFAULT);
    //backUp waits until readers have finished
    semop(semId, &waitForZero, 1);
    if (runningReaders > 0)
    {
      fprintf(stdout, MSG_BACKUP_WAITING, YELLOW, runningReaders, DEFAULT);
    }
    printf("%sNow %d readers are running%s\n", MAGENTA, *currentCountOfReaders, DEFAULT);

    fprintf(stdout, MSG_BACKUP_START_1, YELLOW, DEFAULT);
    sleep(DURATION_BACKUP);
    fprintf(stdout, MSG_BACKUP_FINISHED, YELLOW, DEFAULT);
    semop(mutexId, &down, 1); //backUp has finished using filesystem
  }
}

void initBuffer(void)
{
  if (pids)
  {
    free(pids);
    processesInQueue = 0;
    currentBufferSize = BUFFER_SIZE;
  }
  pids = (int *)malloc(BUFFER_SIZE * sizeof(int));
  if (!pids)
  {
    fprintf(stderr, MEMORY_ALLOC_ERROR, RED, DEFAULT);
    exit(EXIT_FAILURE); // from stdlib.h implemented in a25.c
  }
}
void storePid(int pid, int position)
{
  if (position == currentBufferSize)
  {
    if (VERBOSE)
    {
      fprintf(stdout, "%sREALLOCATE%s\n", YELLOW, DEFAULT);
    }
    currentBufferSize += BUFFER_SIZE;
    pids = realloc(pids, currentBufferSize * sizeof(int));
  }
  if (VERBOSE)
  {
    fprintf(stdout, "storing %d at %d\n", pid, position);
  }
  pids[position] = pid;
}

void waitForProcessesToFinish(void)
{
  int status;
  for (int id = 1; id < 10; id++)
  {
    printf("now we are killing\n");
    /**pids to examine*/
    if ((pids[id] = wait(&status)) == -1)
    {
      fprintf(stderr, ERROR_WAIT, RED, DEFAULT);
    }
    else
    {
      if (WIFEXITED(status))
      { //son terminated normally, that means calling exit(2) or exit(3)
        fprintf(stdout, "%sson with pid %d exited with status %d%s\n", GREEN, pids[id], WEXITSTATUS(status), DEFAULT);
      }
      else if (WIFSIGNALED(status))
      { //son was ended by signal
        fprintf(stdout, "%sson with pid %d ended because of signal: %d%s\n", GREEN, pids[id], WTERMSIG(status), DEFAULT);
      }
      else
      {
        fprintf(stdout, "%sson with pid %d didn't end normally: status: %d%s\n", RED, pids[id], WEXITSTATUS(status), DEFAULT);
      }
    }
  } //end for
}

void handleSignal(int signum)
{
  switch (signum)
  {
  case 2:

    fprintf(stdout, MSG_SIGINT, RED, DEFAULT);
    //first: block readers:
    semop(mutexKill, &up, 1);
    //second: wait until back up process has finished
    semop(mutexId, &waitForZero, 1);
    //third: block incoming readers::forbit forking
    waitForProcessesToFinish();
    //kill back up:
    kill(pids[0], SIGKILL);

    free(pids);
    break;

  default:
    //do nothing
    break;
  }
}
