#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    pid_t pid;
    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) break;
    }

    if (pid > 0) {
        // 父进程
        while (1) {
            printf("I am parent, pid=%d\n", getpid());

            // int ret = wait(NULL);
            int st;
            int ret = wait(&st);
            if (WIFEXITED(st)) {
                printf("exit state code=%d\n", WEXITSTATUS(st));
            }
            if (WIFSIGNALED(st)) {
                printf("killed by signal: %d\n", WTERMSIG(st));
            }

            if (ret == -1) break;
            printf("child die, pid=%d\n", ret);

            sleep(1);
        }
        
    }
    else if (pid == 0) {
        while (1) {
            printf("I am child, pid=%d\n", getpid());
            sleep(1);
        }
        exit(0);
    }

    return 0;
}
