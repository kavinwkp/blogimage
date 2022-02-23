#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getFileNum(const char* path);

int main(int argc, char* argv[]) {

    // 读取某个目录下文件的个数
    if (argc < 2) {
        printf("%s path\n", argv[0]);
        return -1;
    }

    printf("%d\n", argc);

    int num = getFileNum(argv[1]);

    printf("file: %d\n", num);

    return 0;
}


// 递归查找文件
int getFileNum(const char* path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    struct dirent *ptr;

    int total = 0;


    while ((ptr = readdir(dir)) != NULL) {
        char *dname = ptr->d_name;

        if (strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) continue;

        if (ptr->d_type == DT_DIR) {
            char newPath[256];
            sprintf(newPath, "%s/%s", path, dname);
            total += getFileNum(newPath);
        }

        if (ptr->d_type == DT_REG) {
            total++;
        }

    }

    closedir(dir);
    return total;
}