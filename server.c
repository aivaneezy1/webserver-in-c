#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define F fflush(stdout);

typedef struct HttpRequest
{
    char method[8];
    char path[256];
} HttpRequest;

// Function to initialize the server
int server_init(int portn)
{
    int srvfd;
    struct sockaddr_in server_adress;

    srvfd = socket(AF_INET, SOCK_STREAM, 0);
    if (srvfd < 0)
    {
        printf("Error on creating socket()\n");
        return -1;
    }
    printf("Socket() fd =%d\n", srvfd);

    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(portn);
    server_adress.sin_addr.s_addr = INADDR_ANY;

    if (bind(srvfd, (struct sockaddr *)&server_adress, sizeof(server_adress)) < 0)
    {
        printf("Error on bind()\n");
        close(srvfd);
        return -1;
    }

    if (listen(srvfd, 5) < 0)
    {
        printf("Error on listen()\n");
        close(srvfd);
        return -1;
    }

    F;
    return srvfd;
}

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

HttpRequest *parse_request(char buffer[1024])
{
    /*parsing method*/
    HttpRequest *req = malloc(sizeof(HttpRequest));
    memset(req, 0, sizeof(req));
    char *p;
    int i = 0;
    int j = 0;
    for (p = buffer; *p != ' '; *p++)
    {
        req->method[i++] = *p;
    }
    req->method[i] = '\0';

    /*parsing path*/
    if (*p == ' ')
    {
        p++;
    }

    while (*p != ' ' && *p != '\0')
    {
        req->path[j++] = *p;
        p++;
    }

    req->path[j] = '\0';

    return req;
}

/*Sending a HTTP response if it matches the method and path.*/
void http_response(int clfd, HttpRequest *request)
{
    // Send HTTP response
    const char *http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 22\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>HELLO WORLD</h1>";
    if ((strcmp(request->method, "GET") == 0) && (strcmp(request->path, "/aivaneezy") == 0))
    {
        send(clfd, http_response, strlen(http_response), 0);
    }
}

/*Printing our METHOD and PATH*/
void parse_print_req(HttpRequest *req)
{
    printf("Method: %s\n", req->method);
    printf("Path: %s\n", req->path);
}

// Function to send a simple HTTP response to the client
void client_connect(int clfd)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(clfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0)
    {
        printf("Error on recv().\n");
        close(clfd);
        return;
    }
    printf("%s\n", buffer);

    /*Parsing the http request. Return method and path*/
    HttpRequest *request = parse_request(buffer);
    /*printting the request*/
    if (request)
    {
        parse_print_req(request);
    }

    // Send HTTP response
    http_response(clfd, request);

    //  Close the client connection after sending the response
    close(clfd);
}

int main(int argc, char *argv[])
{
    int srvfd, clfd;
    char *port;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return -1;
    }

    port = argv[1];
    int p = atoi(port);

    srvfd = server_init(p);
    if (srvfd < 0)
    {
        printf("Error initializing server\n");
        return -1;
    }

    printf("Listening on port %d...\n", p);

    // Main server loop to accept and handle client connections
    while (1)
    {
        clfd = client_accept(srvfd);
        if (clfd < 0)
        {
            printf("Error on client_accept()\n");
            continue;
        }

        // Fork a child process to handle the client connection
        if (fork() == 0)
        {
            // Child process handles the client
            client_connect(clfd);

            exit(0);
        }
        // Parent process closes the client socket
        close(clfd);
    }

    return 0;
}
