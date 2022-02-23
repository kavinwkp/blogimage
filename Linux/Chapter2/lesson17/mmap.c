#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

int main() {

    int fd = open("text.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    pid_t pid = fork();
    if (pid > 0) {
        // 父进程读
        wait(NULL);
        char buf[64];
        strcpy(buf, (char *)ptr);
        printf("read data: %s\n", buf);
    }
    else if (pid == 0) {
        // 子进程写
        strcpy((char *)ptr, "hello, child");
    }

    munmap(ptr, size);

    return 0;
}
