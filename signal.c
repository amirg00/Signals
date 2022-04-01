#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_handler(int signum)
{
    int i=5;
    int j=0;
    int k;
	
    switch (signum) {
        case SIGCHLD:
            printf("I am in sighandler1\n");
            fflush(stdout);
            raise(SIGUSR1);

        case SIGUSR1:
            printf("I am in sighandler2\n");
            fflush(stdout);
            k=i/j;
            printf("lalala\n");
            fflush(stdout);
            break;

        case SIGFPE:
            printf("I am in sighandler3\n");
            fflush(stdout);

            int fd[2];
            int status;

            // Set the pipe
            if (pipe(fd) < 0)
            {
                perror("pipe error\n");
                exit(1);
            }

            // Generate pipe signal
            switch (fork())
            {
                case -1: /* error */
                fprintf(stderr, "fork error\n");
                exit(1);

                case  0: /* child */
                close(fd[1]);
                close(fd[0]);  /* closing read end of pipe */
                exit(0);

                default: /* parent */
                wait(&status);
                close(fd[0]);
                write(fd[1], "SIGNAL_PIPE\n", 12);
            }
			
        case SIGPIPE:
            printf("I am in sighandler4\n");
            fflush(stdout);
            kill(getpid(), SIGUSR2);
    }
}

void sig_handler2(int signum)
{
    if (signum == SIGUSR2){
        printf("I am in sighandler5\n");
        fflush(stdout);
        exit(1);
    }
}


int main()
{
    int status;
    signal (SIGCHLD, sig_handler);
    signal (SIGUSR1, sig_handler);
    signal (SIGFPE, sig_handler);
    signal (SIGPIPE, sig_handler);
    signal (SIGUSR2, sig_handler2);
    if (!(fork())) { exit(1);}
    wait(&status);
}