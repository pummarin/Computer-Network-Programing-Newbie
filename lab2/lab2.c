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
    struct addrinfo *servinfo;
    struct sockaddr_storage client_address;
    int socket_listen;
    char buf[MAXLINE];
    int n = 0;
    int socket_client;

    printf("Configuring local address...\n\n");
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6; //Dual Stack
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(0, "7777", &hints, &servinfo);

    printf("Creating socket ... \n");
    socket_listen = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (socket_listen == -1)
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    int option = 0;
    if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&option,
                   sizeof(option)))
    {
        fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
        return 1; //Dual Stack
    }

    printf("Binding socket to local address ... \n");
    if (bind(socket_listen, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Listening...\n");
    if (listen(socket_listen, 10) == -1)
    {
        fprintf(stderr, "liste() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Waiting for connection ...\n");

    for (;;)
    {
        socklen_t client_len = sizeof(client_address);
        socket_client = accept(socket_listen, (struct sockaddr *)&client_address,
                               &client_len);
        printf("%s\n", "Received request ...");

        while (n = recv(socket_client, buf, MAXLINE, 0) > 0)
        {
            printf("String receiced from the client: %s\n", buf);
            bzero(buf,MAXLINE);
        }
        close(socket_client);
    }
    printf("Finish\n");

    return 0;
}