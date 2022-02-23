#include <unistd.h>
#include <stdio.h>

int main() {

    char buf[128] = {0};
    getcwd(buf, sizeof(buf));
    printf("pwd: %s\n", buf);

    return 0;
}