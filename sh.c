#include "sh.h"
#include <sys/types.h> //fork?
#include <sys/wait.h> /* waitpid */
#include <fcntl.h> //File desc. constants
#include <unistd.h> //exec{l,le,v,vp,vpe}, dup, dup2
#include <stdio.h> //fprintf, fread, fdopen, fopen, etc
#include <string.h> /* perror */

//TODO: multi-threads?
//TODO: fork?
//fork, pipe, fnctl, wait, 
//environ, envp
//dup{2} for fds
//pipe

int exec(char *args)
{
    //int retval;
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

    //TODO: Set up pipes, exec process
    //use dup2 to replace stdin/stdout in child

    return 0;
}

void sh_loop()
{
    char args[1024];
    char hostname[24];
    gethostname(hostname, 24);
    printf("%s@%s > ", getlogin(), hostname);
    fgets(args, 1024, stdin);

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