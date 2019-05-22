// Durations
#define COUNT_OF_DURATIONS 4

#define DURATION_0 1
#define DURATION_1 2
#define DURATION_2 3
#define DURATION_3 4

//! the period when backup returns
#define PERIOD 8

//!duration of backup
#define DURATION_BACKUP 3 //duration of backup process

// codes for color
#define GREEN "\033[1;32m"

#define RED "\033[1;31m"

#define DEFAULT "\x1B[37m"

#define YELLOW "\033[01;33m"

#define MAGENTA "\033[1;35m"

#define MEMORY_ALLOC_ERROR "%sERROR MEMORY ALLOCATION%s\n"

//!the message when child begins to read
#define MSG_CHILD_VERBOSE_START "%sReader with pid %d will read for %d minute/s%s\n"

//!the message when child has finished reading
#define MSG_CHILD_VERBOSE_FINISHED "%sReader with pid %d has finished reading after %d minute/s%s\n"

//!the message of backup process
#define MSG_BACKUP_PROCESS "%sHere is backup with pid:%d%s\n"

#define MSG_BACKUP_START_0 "%sTIME FOR BACKUP!\nI HAVE REQUESTED RESSOURCE!\n%d reader/s is/ are running!%s\n"

#define MSG_BACKUP_WAITING "%sHas waited for %d reader/s to finish%s\n"

#define MSG_BACKUP_START_1 "%sALL READERS HAVE FINISHED!\nEXECUTING BACKUP!%s\n"

#define MSG_BACKUP_FINISHED "%sFINISHED BACKUP!%s\n"

#define ERROR_FORK "%sERROR AT FORK%s\n"

#define ERROR_WAIT "%sERROR AT WAIT%s\n"

#define MSG_LAUNCH_PROCESSES "%s Lauch %d processe/s in next minute%s\n"

#define EXIT_CMD "schluss"

//!the Permission for semaphors: full access to field of semaphors
#define PERMISSION 0777

#define KEY 1234

#define BUFFER_SIZE 1

//shall storing messages be verbosed?
#define VERBOSE 0

#define MSG_SIGINT "%sSIGINT <2> received!\n I kill myself.%s\n"


