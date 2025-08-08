#define _POSIX_C_SOURCE 202412L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>

#ifndef HEAD_H
#define HEAD_H

/* file with a list of required daemons */
#define DAEMONS_LIST_FILE "/home/fomchik/programming/daemon_monitor/daemons.txt" 

#define DAEMONS_COUNT     3 
#define STATUS            32 


/* the structure describes the state of the daemon */
typedef struct
{
    char daemon_name[NAME_MAX]; 
    char daemon_status[STATUS]; 
    pid_t daemon_pid; 
} DAEMON;


/* the token stores the daemon index in the array and the updated flag */
typedef struct 
{
    int daemon_idx;
    bool is_updated;
} VERIFICATION_TOKEN;

/* initialization of verification tokens */
VERIFICATION_TOKEN* init_verification_tokens();

/* initialization of the daemon array */
DAEMON* init_daemons_array();

char* analyze_verification_tokens(VERIFICATION_TOKEN*, DAEMON*);
char* restart_daemon(const char*);

void* check_daemons(DAEMON*, VERIFICATION_TOKEN[]);
void set_daemon_updated(VERIFICATION_TOKEN*, int);
char* parse(const char*);
/*  */
char* get_status(FILE*);
void update_daemon_info(DAEMON* , const char* , const char*, pid_t);


#endif
