// Вспомогательный функционал для основных функций надзирателя

#include "head.h"

// Функция инициализации демона начальными значениями
DAEMON* init_daemons_array()
{
    DAEMON* deamons_array = malloc(sizeof(DAEMON) * DAEMONS_COUNT);
    if(!deamons_array)
    {
        return NULL;
    }

    FILE* fp;
    char line[PATH_MAX];
    int i = 0;

    fp = fopen(DAEMONS_LIST_FILE, "r");
    if (!fp){
        return NULL;
    }

    while(fgets(line, sizeof(line), fp) && i < DAEMONS_COUNT)
    {
        line[strcspn(line, "\n")] = '\0';

        strncpy(deamons_array[i].daemon_name, line, sizeof(deamons_array[i].daemon_name) - 1);
        strncpy(deamons_array[i].daemon_status, "none", sizeof(deamons_array[i].daemon_status) - 1);
        deamons_array[i].daemon_pid = -1;

        i++;
    }

    fclose(fp);

    return deamons_array;
}


VERIFICATION_TOKEN* init_verification_tokens()
{
    int i;

    VERIFICATION_TOKEN* tokens = malloc(sizeof(VERIFICATION_TOKEN) * DAEMONS_COUNT);
    if(!tokens)
    {
        return NULL;
    }

    for(i = 0; i < DAEMONS_COUNT; ++i)
    {
        tokens[i].daemon_idx = i;
        tokens[i].is_updated = false;
    }

    return tokens;
}
