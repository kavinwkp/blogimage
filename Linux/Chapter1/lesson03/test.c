#include <stdio.h>


int main() {
    
#ifdef DEBUG
    printf("debug\n");
#endif
    printf("hello\n");
    return 0;
}
