#include "head.h"


/*
    daemonize the process, call initialization functions.

    in the main loop: check daemons, analyze the result and restart the daemon if necessary
*/

int main(void)
{
    pid_t monitor_pid = fork();

    if(monitor_pid < 0)
    {
        _exit(EXIT_FAILURE);
    }

    if(monitor_pid > 0)
    {
        _exit(EXIT_SUCCESS);
    }

    if(chdir("/") != 0)
    {
        _exit(EXIT_FAILURE);
    } 

    pid_t monitor_session_id = setsid();
    if(monitor_session_id == -1)
    { 
        _exit(EXIT_FAILURE);
    }


    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);


    openlog("deamon-monitor", LOG_NDELAY | LOG_CONS | LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "---Starting Monitor Deamon---");


    syslog(LOG_DEBUG, "Initializing daemons array...");
    DAEMON* daemons_array = init_daemons_array();
    if(!daemons_array)
    {
        syslog(LOG_ERR, "Failed to init daemons array");
        _exit(EXIT_FAILURE);
    }


    char* check_result = NULL;
    char* analyze_result = NULL;

    syslog(LOG_DEBUG, "Starting main loop...");

    for( ; ; )
    {
        syslog(LOG_DEBUG, "Initializing verification tokens...");
        VERIFICATION_TOKEN* verification_tokens = init_verification_tokens();
        if(!verification_tokens)
        {
            syslog(LOG_ERR, "Failed to init verification tokens array");
            _exit(EXIT_FAILURE);
        }

        syslog(LOG_DEBUG, "Call check_daemons()...");
        
        check_result = check_daemons(daemons_array, verification_tokens);
        if(!check_result)
        {
            syslog(LOG_ERR, "Detected error while checking daemons");

            free(verification_tokens);
            _exit(EXIT_FAILURE);
        }
        
        syslog(LOG_INFO, "Daemons check result: %s\n", check_result);



        analyze_result = analyze_verification_tokens(verification_tokens, daemons_array);
        if(strcmp(analyze_result, "analyze success") != 0)
        {
            syslog(LOG_INFO, "Detected error while analyze verification tokens");

            free(verification_tokens);
            free(check_result);

            _exit(EXIT_FAILURE);
        }

        free(verification_tokens);
        free(check_result);
        free(analyze_result);
        
        sleep(5);
    }


    free(daemons_array);
}