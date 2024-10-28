#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

pid_t child;
pid_t child2;

void sigHandler1(int signum, siginfo_t *null, void *pid) {
    *(int*)pid = wait(NULL);
    if (*(int*)pid == child) { 
        printf("Signal sended from Child 1 (PID=%d) \n", *(int*)pid);
    } else {
        printf("Signal sended from Child 2 (PID=%d) \n", *(int*)pid);
    }
    
    exit(0);
}

int main() {
    struct sigaction handler;
    handler.sa_sigaction = sigHandler1;
    handler.sa_flags = SA_SIGINFO;

    if ((child = fork()) == 0) { // child 1
        usleep(10000);
        kill(getppid(), SIGUSR1);
    } else if (child > 0) { // parent 

        if ((child2 = fork()) == 0) { // child 2
            usleep(10000);
            kill(getppid(), SIGUSR1);
        } else if (child2 > 0) { // parent
            sigaction(SIGUSR1, &handler, NULL);
            usleep(30000);
            wait(NULL);
            wait(NULL);
        }

    }
}
