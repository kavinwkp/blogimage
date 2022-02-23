#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    int ret = access("fifo1", F_OK);
    if (ret == -1) {
        printf("make fifo\n");
        int ret = mkfifo("fifo1", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    
    return 0;
}