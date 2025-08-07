#include "head.h"

char* analyze_verification_tokens(VERIFICATION_TOKEN* tokens, DAEMON* daemons_array)
{
    char* restart_result = NULL;

    char daemon_binary_path[PATH_MAX] = {0};
    pid_t daemon_pid = -1;

    int i;
    for(i = 0; i < DAEMONS_COUNT; ++i)
    {
        if(!tokens[i].is_updated)
        {
            syslog(LOG_WARNING, "Warning: daemon [%s] not found", daemons_array[i].daemon_name);

            daemon_pid = fork();

            if(daemon_pid < 0)
            {
                return strdup("fork failed");
            }


            if(daemon_pid == 0)
            {
                syslog(LOG_INFO, "Restarting daemon [%s]", daemons_array[i].daemon_name);
                restart_result = restart_daemon(daemons_array[i].daemon_name);

                if(restart_result)
                {
                    syslog(LOG_ERR, "Failed to execute daemon [%s]", daemons_array[i].daemon_name);
                    free(restart_result);
                }

                _exit(restart_result ? EXIT_FAILURE : EXIT_SUCCESS);
            }

        }
    }

    return strdup("analyze success");
}

char* restart_daemon(const char* daemon_name)
{
    char daemon_binary_path[PATH_MAX] = {0};
    pid_t daemon_pid = -1;

    if(strcmp(daemon_name, "syslog-ng") == 0)
    {
        snprintf(daemon_binary_path, PATH_MAX, "/usr/sbin/%s", daemon_name);
    }

    else snprintf(daemon_binary_path, PATH_MAX, "/usr/bin/%s", daemon_name);

    /* file existance check */
    if (access(daemon_binary_path, X_OK) != 0) {
        return strdup("daemon binary not found or not executable");
    }

    if(chdir("/") != 0)
    {
        return strdup("chdir failed");
    }

    if(setsid() == -1)
    {
        return strdup("setsid failed");
    }

    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);


    char *args[] = {(char*)daemon_name, NULL };
    char *envp[] = { NULL };

    syslog(LOG_INFO, "executing daemon [%s]...", daemon_name);
    execve(daemon_binary_path, args, envp);

    return strdup("execve failed");
}