#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    sigset_t set;
    sigemptyset(&set);
    int ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT no block\n");
    }
    else if (ret == 1) {
        printf("SIGINT block\n");
    }

    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT no block\n");
    }
    else if (ret == 1) {
        printf("SIGINT block\n");
    }
    
    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT no block\n");
    }
    else if (ret == 1) {
        printf("SIGQUIT block\n");
    }

    sigdelset(&set, SIGQUIT);
    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT no block\n");
    }
    else if (ret == 1) {
        printf("SIGQUIT block\n");
    }

    return 0;
}