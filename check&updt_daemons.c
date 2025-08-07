#include "head.h"


void set_daemon_updated(VERIFICATION_TOKEN* tokens, int index)
{
    tokens[index].is_updated = true;
}




char* get_status(FILE* fp)
{
    char status_line[STATUS + 20] = {0};

    fseek(fp, 0, SEEK_SET);

    while(fgets(status_line, sizeof(status_line), fp))
    {
        if(strstr(status_line, "Status:"))
        {
            break;
        }
    }

    char* parsed_status = parse(status_line);
    if(!parsed_status) return NULL;

    return parsed_status;
}





char* parse(const char* line)
{
    char* parsed_line = NULL;   
    int i;
    int j;

    i = 0;
    while(line[i] != ':') ++i;
    while(!(isalpha(line[i]))) ++i;

    parsed_line = malloc(NAME_MAX);
    if(!parsed_line)
    {
        return NULL;
    }

    j = 0;
    while(line[i] && j < NAME_MAX)
    {
        parsed_line[j++] = line[i++];
    }

    parsed_line[strcspn(parsed_line, "\n")] = '\0';

    return parsed_line;
}





void update_daemon_info(DAEMON* daemons_array, 
                         const char* daemon_name, 
                         const char* daemon_status, 
                         pid_t daemon_pid)
{
    int i;

    for(i = 0; i < DAEMONS_COUNT; ++i)
    {
        if(strcmp(daemon_name, daemons_array[i].daemon_name) == 0)
        {
            daemons_array[i].daemon_pid = daemon_pid;
            strncpy(daemons_array[i].daemon_status, daemon_status, STATUS);
        }
    }
}






void* check_daemons(DAEMON* daemons_array, VERIFICATION_TOKEN tokens[])
{
    char proc_name_line[NAME_MAX + 20] = {0};
    char* proc_name = NULL;

    char filename[PATH_MAX] = {0};
    FILE* status_file;



    DIR* proc_dir = opendir("/proc");
    if(!proc_dir)
    {
        syslog(LOG_ERR, "Failed open /proc");
        return NULL;
    }

    struct dirent* entry;
    struct stat entry_stat;

    while((entry = readdir(proc_dir)))
    {
        if(!(isdigit(entry -> d_name[0])))
        {
            continue;
        }

        snprintf(filename, PATH_MAX, "/proc/%s/status", entry -> d_name);

        if (stat(filename, &entry_stat) == -1) {
            continue;
        }

        if(!S_ISREG(entry_stat.st_mode))
        {
            continue;
        }

        status_file = fopen(filename, "r");
        if(!status_file)
        {
            closedir(proc_dir);

            return NULL;
        }



        while(fgets(proc_name_line, sizeof(proc_name_line), status_file))
        {
            if(strstr(proc_name_line, "Name:"))
            {
                break;
            }
        }

        proc_name = parse(proc_name_line);
        if(!proc_name)
        {
            fclose(status_file);
            closedir(proc_dir);

            return NULL;
        }


        for(int j = 0; j < DAEMONS_COUNT; ++j)
        {
            if(strcmp(proc_name, daemons_array[j].daemon_name) == 0)
            {
                char* proc_status = get_status(status_file);
                if(!proc_status)
                {
                    fclose(status_file);
                    closedir(proc_dir);

                    return NULL;
                }

                update_daemon_info(daemons_array, proc_name, proc_status, atoi(entry -> d_name));
                free(proc_status);

                set_daemon_updated(tokens, j);

                syslog(LOG_INFO, "[%s] update success: pid = %d", proc_name, atoi(entry -> d_name));
            }
        }

        free(proc_name);
        fclose(status_file);
    }
    
    closedir(proc_dir);

    return strdup("success");
}