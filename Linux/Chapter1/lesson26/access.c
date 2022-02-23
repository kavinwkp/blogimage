#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main() {

    // int ret = access("a.txt", F_OK);
    // if (ret == -1) {
    //     perror("access");
    //     return -1;
    // }

    int ret = chmod("a.txt", 0775);
    if (ret == -1) {
        perror("chmod");
        return -1;
    }

    return 0;
}