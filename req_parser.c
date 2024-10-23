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
    char *message1 = "HELLO WORLD";
    char *message2 = "Show images in this path";
    int len_message1 = strlen(message1) + strlen("<h1></h1>");
    int len_message2 = strlen(message2) + strlen("<h1></h1>");

    char http_response[512];
    char http_res_image[512];
    /*SEND HTTP RESPONSE FOR /aivaneezy path*/
    snprintf(http_response, sizeof(http_response), "HTTP/1.1 200 OK\r\n"
                                                   "Content-Type: text/html\r\n"
                                                   "Content-Length: %d\r\n"
                                                   "Connection: close\r\n"
                                                   "\r\n"
                                                   "<h1>%s</h1>",
             len_message1, message1);

    /*SEND IMAGE FOR /image path*/

    if ((strcmp(request->method, "GET") == 0) && (strcmp(request->path, "/") == 0))
    {
        send(clfd, http_response, strlen(http_response), 0);
    }
    else if ((strcmp(request->method, "GET") == 0) && (strcmp(request->path, "/img") == 0))
    {

        /*Show image*/
        server_static_image(clfd);
    }
}

/*Printing our METHOD and PATH*/
void parse_print_req(HttpRequest *req)
{
    printf("Method: %s\n", req->method);
    printf("Path: %s\n", req->path);
}