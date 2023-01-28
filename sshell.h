// sshell.h
// Contains header files 
// and important constants
// used in sshell.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <dirent.h>

#include "token.h"
#include "linked_list.h"
#include "generalList.h"

#define CMDLINE_MAX 512
#define TOKEN_LENGTH_MAX 32 
#define MAX_ARGUMENTS 16
