#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define IP "87.248.100.216" // yahoo ip adress
int init_client(int portn)
{
    int svfd;
    struct sockaddr_in server_address;
    socklen_t server_adr_size = sizeof(server_address);
    char buff[512];
    char *data;

    data = "HEAD / HTTP/1.0\r\n\r\n";

    /*1. Create a socket*/
    svfd = socket(AF_INET, SOCK_STREAM, 0);
    if (svfd < 0)
    {
        printf("Erron on socket \n");
        close(svfd);
        return -1;
    }
    printf("Socket created successfully\n");

    /*2. Specify server address*/
    server_address.sin_family = AF_INET; // IPV4
    server_address.sin_port = htons(portn);
    server_address.sin_addr.s_addr = inet_addr(IP);

    /*3. Connect to the server*/
    int c = connect(svfd, (struct sockaddr *)&server_address, server_adr_size);
    if (c < 0)
    {
        printf("Error on connect()\n");
        close(svfd);
        return -1;
    }
    write(svfd, data, strlen(data));
    memset(buff, 0, 512);
    read(svfd, buff, 511);
    close(svfd);
    printf("\n%s\n", buff);
}

int main(int argc, char *argv[])
{

    int c;
    char *port;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        return -1;
    }
    else
    {
        port = argv[1];
    }
    /*converting char port to in*/
    int p = atoi(port);
    c = init_client(p);
    if (c < 0)
    {
        printf("Error on init_client()");
        return -1;
    }
    printf("Connected with server successfully\n");
}