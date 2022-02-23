/*
使用内存映射实现文件拷贝功能
1. 对原始文件进行内存映射
2. 创建一个新文件（拓展该文件）
3. 把新文件的数据映射到内存中
4. 内存拷贝
5. 释放资源
*/



#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

int main() {
    // 1. 对原始文件进行内存映射
    int fd = open("hello.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(0);
    }
    // 原始文件大小
    int len = lseek(fd, 0, SEEK_END);

    // 2. 创建一个新文件（拓展该文件）
    int fd1 = open("copy.txt", O_CREAT | O_RDWR, 0664);
    if (fd1 == -1) {
        perror("open");
        exit(0);
    }

    // 拓展
    truncate("copy.txt", len);
    write(fd1, " ", 1);
    // 3. 把新文件的数据映射到内存中
    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void *ptr1 = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    if (ptr1 == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }
    // 内存拷贝
    memcpy(ptr1, ptr, len);
    munmap(ptr, len);
    munmap(ptr1, len);
    close(fd1);
    close(fd);
    return 0;
}