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
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    int max_socket = socket_listen;

    printf("Waiting for connection ...\n");

     // SELECT
    for (;;)
    {
        fd_set reads;
        reads = master;
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
        {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        int i;
        for (i = 1; i <= max_socket; ++i)
        {
            if (FD_ISSET(i, &reads))
            {
                if (i == socket_listen)
                {
                    printf("[... Received request from a new client ...]\n\n");
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    int socket_client = accept(socket_listen,
                                               (struct sockaddr *)&client_address,
                                               &client_len);
                    if (socket_client == -1)
                    {
                        fprintf(stderr, "accept() failed. (%d)\n",
                                GETSOCKETERRNO());
                        return 1;
                    }
                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                        max_socket = socket_client;
                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address,

                                client_len,
                                address_buffer, sizeof(address_buffer), 0, 0,
                                NI_NUMERICHOST);

                    printf("New connection from %s\n\n", address_buffer);
                }
                else
                {
                    char read[1024];
                    // RECEIVED FROM CLIENT
                    int bytes_received = recv(i, read, 1024, 0);
                    printf("Msg received from the client (Socket = %d): %s\n\n", i, read);

                    // SEND TO CLIENT
                    send(i, "ACK from server", strlen("ACK from server"), 0);
                    strtok(read, "\n");
                    // IF CLOSE
                    if (strcmp(read, "close") == 0)
                    {
                        FD_CLR(i, &master);
                        close(i);
                        printf("Closing this socket client (Socket = %d)\n\n", i);
                    }

                    bzero(read, 1024);
                }
            }
        }
    }

        // for (;;)
        // {
        //     socklen_t client_len = sizeof(client_address);
        //     socket_client = accept(socket_listen, (struct sockaddr *)&client_address,
        //                            &client_len);
        //     printf("%s\n", "Received request ...");

        //     while (n = recv(socket_client, buf, MAXLINE, 0) > 0)
        //     {
        //         printf("String receiced from the client: %s\n", buf);
        //         bzero(buf, MAXLINE);
        //     }
        //     close(socket_client);
        // }
        // printf("Finish\n");
    
    printf("Closing listening socket.. \n");
    close(socket_listen);
    printf("Finished. \n");
    return 0;
}