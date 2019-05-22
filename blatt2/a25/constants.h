#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__


//! size of buffer
#define BUFFER_SIZE 256

//! the prompt of root
#define PROMPT_ROOT "#"

//!the promt of default user
#define PROMPT_OTHER "~$"

// codes for color
#define GREEN "\033[1;32m"

#define RED "\033[1;31m"

#define MAGENTA "\033[1;35m"

#define DEFAULT "\x1B[37m"

#define YELLOW "\033[01;33m"

//!memory allocation error message
#define MEMORY_ALLOC_ERROR "%serror at memory allocation%s\n"

//!fork error message
#define ERROR_FORK "%sERROR AT FORK%s\n"

//wait error message
#define ERROR_WAIT "%sERROR AT WAIT%s\n"

//!Error message when user could not be found
#define ERROR_USER "%sSomething went wrong when trying to identify user!%s\n"

#define EXIT_CMD "schluss"

//!name of root
#define ROOT "root"

//!the delimiter to split a string
#define DELIMITER " "

//!if this flag is true, the shell prints informations like status of child after execv call
#define VERBOSE 1
#endif
