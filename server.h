#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define F fflush(stdout);

typedef struct
{
    char method[8];
    char path[256];
} HttpRequest;

// Initializze server
int server_init(int portn);
/*Function to accept client connections*/
int client_accept(int srvfd);
HttpRequest *parse_request(char buffer[1024]);
void http_response(int clfd, HttpRequest *request);
void parse_print_req(HttpRequest *req);
void client_connect(int clfd);
void server_static_image(int clfd);

#endif