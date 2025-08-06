#include "head.h"

void analyze_verification_tokens(VERIFICATION_TOKEN* tokens, DAEMON* daemons_array, FILE* fp)
{
    int i;
    for(i = 0; i < DAEMONS_COUNT; ++i)
    {
        if(!tokens[i].is_updated)
        {
            fprintf(fp, "\nfailed to update daemon: %s\n", daemons_array[i].daemon_name);
        }
    }
}