#include "server.h"

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