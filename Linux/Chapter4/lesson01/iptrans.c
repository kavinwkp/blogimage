#include <arpa/inet.h>
#include <stdio.h>

int main() {

    char buf[] = "192.168.1.4";
    unsigned int num = 0;
    // 将点分十进制字符串转换整数
    inet_pton(AF_INET, buf, &num);

    unsigned char *p = (unsigned char *)&num;
    
    for (int i = 0; i < 4; i++) {
        printf("%d ", *(p + i));
    }
    printf("\n");

    // 整数IP转换为点分十进制字符串
    char ip[16] = "";
    const char *str = inet_ntop(AF_INET, &num, ip, 16);
    printf("ip: %s\n", ip);
    printf("str: %s\n", str);

    return 0;
}
// 192 168 1 4
// ip: 192.168.1.4
// str: 192.168.1.4