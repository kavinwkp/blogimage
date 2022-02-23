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
            sleep(1);

            int st;
            // int ret = waitpid(-1, &st, 0);
            int ret = waitpid(-1, &st, WNOHANG);
            

            if (ret == -1) break;   // 没有子进程了
            else if (ret == 0) continue;    // 还有子进程
            else if (ret > 0) {     // 回收了某个子进程
                if (WIFEXITED(st)) {
                    printf("exit state code=%d\n", WEXITSTATUS(st));
                }
                if (WIFSIGNALED(st)) {
                    printf("killed by signal: %d\n", WTERMSIG(st));
                }
                printf("child die, pid=%d\n", ret);
            }

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
