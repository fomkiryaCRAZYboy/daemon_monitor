#include "head.h"


int main(void)
{
    /* инициализация файла логов */
    FILE* log_file_fp = fopen(LOG_FILE, "w");
    if(!log_file_fp)
    {
        perror("fopen err");
        exit(EXIT_FAILURE);
    }

    DAEMON* daemons_array = init_daemons_array();
    if(!daemons_array)
    {
        fputs("failed to init daemons array\n", log_file_fp);
        exit(EXIT_FAILURE);
    }

    fputs("successfull daemons array initialization\n", log_file_fp);


    for(int i = 0; i < 3; ++i)
    {
        VERIFICATION_TOKEN* verification_tokens = init_verification_tokens();
        if(!verification_tokens)
        {
            fputs("failed to init verification tokens array\n", log_file_fp);
            exit(EXIT_FAILURE);
        }

        fputs("successfull verification tokens initialization\n", log_file_fp);


        if(check_daemons(daemons_array, verification_tokens) == NULL)
        {
            fputs("detected error while checking daemons\n", log_file_fp);
            exit(EXIT_FAILURE);
        }
        analyze_verification_tokens(verification_tokens, daemons_array, log_file_fp);

        free(verification_tokens);
        
        sleep(5);
    }


    free(daemons_array);
    fclose(log_file_fp);

    exit(EXIT_SUCCESS);
}