#include "server.h"

// Function to accept client connections
int client_accept(int srvfd)
{
    int clfd;
    struct sockaddr_in client_address;
    socklen_t client_adr_size = sizeof(client_address);
    memset(&client_address, 0, sizeof(client_address));

    clfd = accept(srvfd, (struct sockaddr *)&client_address, &client_adr_size);
    if (clfd < 0)
    {
        printf("Error on accept()\n");
        return -1;
    }
    printf("Client connected at IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    return clfd;
}