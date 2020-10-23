#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main()
{
    struct sockaddr_in sin;
    struct sockaddr_storage client_address;
    struct sockaddr_in *sin_storage;
    char ipv4[INET_ADDRSTRLEN];

    printf("My IP: 111.112.221.222\n");
    inet_pton(AF_INET, "111.112.221.222", &(sin.sin_addr));
    printf("Original: 0x%x\n", sin.sin_addr);

    printf("Using inet_hton: 0x%x\n", htonl(inet_addr("111.112.221.222")),htonl(sin.sin_addr.s_addr));
    printf("Using inet_hton: 0x%x\n", ntohl(inet_addr("111.112.221.222")),ntohl(sin.sin_addr.s_addr));
    return 0;
}