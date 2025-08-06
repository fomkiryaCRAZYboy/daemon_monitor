#define _POSIX_C_SOURCE 200809L

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

#ifndef HEAD_H
#define HEAD_H

#define LOG_FILE          "mylogs.txt"  
#define DAEMONS_LIST_FILE "daemons.txt" /* Имя файла, в котором будут хранится имена демонов */ 
#define DAEMONS_COUNT     3 /* Количество демонов */
#define STATUS            32 /* Длина статуса демона */

/* Структура демон, в которой будет хранится информация о демоне */
typedef struct
{
    char daemon_name[NAME_MAX]; /* Имя демона */
    char daemon_status[STATUS]; /* Статус демона */
    pid_t daemon_pid; /* PID демона */
} DAEMON;


typedef struct 
{
    int daemon_idx;
    bool is_updated;
} VERIFICATION_TOKEN;



VERIFICATION_TOKEN* init_verification_tokens();
DAEMON* init_daemons_array();

void analyze_verification_tokens(VERIFICATION_TOKEN*, DAEMON*, FILE*);

void* check_daemons(DAEMON*, VERIFICATION_TOKEN[]);
void set_daemon_updated(VERIFICATION_TOKEN*, int);
char* parse(const char*);
char* get_status(FILE*);
void update_daemon_info(DAEMON* , const char* , const char*, pid_t);


#endif
