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

int main() {

struct addrinfo hints;
struct addrinfo *bind_address;
struct sockaddr_storage client_address;
int socket_peer;
const char *message = "Hello from server";
int socket_listen;


    printf("Configuring local address...\n");
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(0, "3296", &hints, &bind_address);

    printf("Creating socket ... \n");
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (socket_listen == -1)
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address ... \n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen) == -1 )
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
    }

    socklen_t client_len = sizeof(client_address);
    char read[1024];
    int bytes_received = recvfrom(socket_listen,read, 1024, 0,(struct sockaddr*) &client_address, &client_len);
    printf("Received (%d bytes): %.*s\n", bytes_received, bytes_received,read);           
    // close(socket_listen);


    printf("============================\n");
    int bytes_sent = sendto(socket_listen, message, strlen(message), 0,(struct sockaddr*) &client_address,sizeof(client_address));
    printf("Sent %d bytes. \n", bytes_sent);
    printf("Finished\n");  
    close(socket_peer);




    return 0;
}

