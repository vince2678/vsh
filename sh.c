#include "helpers.h"
#include "sh.h"

#include <sys/types.h>
#include <sys/wait.h> /* waitpid */
#include <fcntl.h> /* fd manipulation */
#include <unistd.h> /* exec{l,le,v,vp,vpe}, dup, dup2 */
#include <stdio.h> /* fprintf, fread, fdopen, fopen, ... */
#include <string.h> /* perror */

const int MAX_CMDLINE_LENGTH = 1024;

int exec(char *args)
{
#if 0
    int pipefd[2];
#endif
    pid_t cpid;

#if 0
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
#endif

    cpid = fork();

    if (cpid < 0)
    {
        perror("fork");
        return -1;
    }
    else if (cpid > 0)
    {
        int wstatus;

        waitpid(cpid, &wstatus, 0);

        if (WIFEXITED(wstatus))
            return WEXITSTATUS(wstatus);
        else //TODO: possibly do more checks here
            return 1;
    }
    else
    {
#if 0
        close(pipefd[PIPE_READ]);
        dup2(pipefd[PIPE_WRITE], STDOUT_FILENO);
#endif

        char **argv = strtostrv(args, ' ');

        if (argv && argv[0] && strlen(argv[0]) > 0)
        {
            execvp(argv[0], argv);
            perror("execvp");
            return -1;
        }
    }

    return 0;
}

char *read_cmdline()
{
    char *args, c;
    int i;

    args = calloc(MAX_CMDLINE_LENGTH + 1, sizeof(char));

    if (args == NULL)
    {
        perror("calloc");
        return NULL;
    }

    i = 0;

    while(i < MAX_CMDLINE_LENGTH)
    {
        c = fgetc(stdin);

        if (c == EOF)
            break;
        else if (c == '\n')
        {
            if (args[i-1] == '\\')
            {
                /* overwrite previous '\' on next iter.
                 and print prompt to enter text */
                i = i - 1;
                printf("> ");
                continue;
            }
            break; /* else is superfluous */
        }
        args[i] = c;
        i = i + 1;
    }
    /* null terminate string */
    args[i] = 0;

    return args;
}

void sh_loop()
{
    char hostname[STR_SIZE];
    gethostname(hostname, STR_SIZE);
    printf("%s@%s > ", getlogin(), hostname);

    char *cmdline = read_cmdline();
    int retval = exec(cmdline);
    free(cmdline);

    fprintf(stderr, "Exit status: %i\n", retval);
}

int main(int argc, char **argv)
{
    while(1)
    {
        sh_loop();
    }
    return 0;
}
