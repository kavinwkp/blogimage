#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>


int main() {

    // int ret = mkdir("aaa", 0777);
    // if (ret == -1) {
    //     perror("mkdir");
    //     return -1;
    // }

    int ret = rename("aaa", "bbb");
    if (ret == -1) {
        perror("mkdir");
        return -1;
    }
    return 0;
}
