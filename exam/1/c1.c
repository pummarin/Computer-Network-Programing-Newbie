#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define GETSOCKETERRNO() (errno)
#define SERVER_ADDRESS "127.0.0.1"

#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    struct addrinfo hints;
    struct addrinfo *server_addr;
    struct sockaddr_storage server_address;
    int socket_peer;
    const char *message = "PUNNARUN PHIAWSOONGNERN";
    const char *news = "Hi again";

    printf("Configuring remote address...\n");
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(SERVER_ADDRESS, "8080", &hints, &server_addr))
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

    printf("Sending: %s\n", message);
    int bytes_sent = sendto(socket_peer, message, strlen(message), 0, server_addr->ai_addr, server_addr->ai_addrlen);
    printf("Sent %d bytes. \n", bytes_sent);
    freeaddrinfo(server_addr);
    // close(socket_peer);

    printf("============================\n");
    socklen_t server_len = sizeof(server_address);
    char read[1024];
    int bytes_received = recvfrom(socket_peer, (char *)read, 1024, 0, (struct sockaddr *)&server_address, &server_len);

    // char r1[1024];
    read[bytes_received] = '\0';
    printf("Received (%d bytes): %.*s\n", bytes_received, bytes_received, read);
    close(socket_peer);
    // sprintf(r1,"%s",read);
    
    // memset(&hints, 0, sizeof(hints));
    // hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(SERVER_ADDRESS, read, &hints, &server_addr))
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
    printf("Sending: %s\n", news);
    sendto(socket_peer,news,strlen(news),0,server_addr->ai_addr,server_addr->ai_addrlen);
    printf("Finished.\n");

    close(socket_peer);

    return 0;
}
