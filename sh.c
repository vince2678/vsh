#include "helpers.h"
#include "sh.h"

#include <sys/types.h>
#include <sys/wait.h> /* waitpid */
#include <fcntl.h> /* fd manipulation */
#include <unistd.h> /* exec{l,le,v,vp,vpe}, dup, dup2 */
#include <stdio.h> /* fprintf, fread, fdopen, fopen, ... */
#include <string.h> /* perror */

int exec(char *args)
{
    int pipefd[2];
    pid_t cpid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

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
        //close(pipefd[FD_READ]);
        //dup2(pipefd[FD_WRITE], STDOUT_FILENO);

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

void sh_loop()
{
    char args[MAX_CMDLINE_LENGTH];
    char hostname[24];
    gethostname(hostname, 24);
    printf("%s@%s > ", getlogin(), hostname);
    fgets(args, MAX_CMDLINE_LENGTH, stdin);

    // strip newline at end
    if (args[strlen(args) - 1] == '\n')
    {
        args[strlen(args) - 1] = 0;
    }

    int retval = exec (args);
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