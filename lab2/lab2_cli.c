#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define GETSOCKETERRNO() (errno)
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_ADDRESS6 "::1"
#define MAXLINE 4096

#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    struct addrinfo hints;
    struct addrinfo *server_addr;
    struct sockaddr_storage server_from;
    int socket_peer;
    char sendline[MAXLINE], recvline[MAXLINE];
    char buffer[1024];

    printf("Configuring remote address...\n");
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(SERVER_ADDRESS6, "7777", &hints, &server_addr))
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Creating socket ... \n");
    socket_peer = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
    if (socket_peer == -1)
    {
        fprintf(stderr, "socket() faoled. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Sercer address is: ");
    char address_buffer[100];
    char port_buffer[100];
    getnameinfo(server_addr->ai_addr, server_addr->ai_addrlen, address_buffer, sizeof(address_buffer), port_buffer, sizeof(port_buffer), NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s\nPort is: %s\n", address_buffer, port_buffer);

    if (connect(socket_peer, server_addr->ai_addr,server_addr->ai_addrlen) == -1)
    {
        fprintf(stderr, "connect() faoled. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    printf("connected\n");

    while (fgets(sendline, MAXLINE, stdin) != NULL)
    {
        send(socket_peer, sendline, strlen(sendline), 0);
        printf("String: ");
        fputs(recvline, stdout);
    }
    close(socket_peer);
    printf("Finished.\n");

    return 0;
}
