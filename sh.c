#include "sh.h"
#include <sys/types.h> //fork?
#include <fcntl.h> //File desc. constants
#include <unistd.h> //exec{l,le,v,vp,vpe}, dup, dup2
#include <stdio.h> //fprintf, fread, fdopen, fopen, etc

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
        return cpid;
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
}

int main(int argc, char **argv)
{
    sh_loop();
    return 0;
}