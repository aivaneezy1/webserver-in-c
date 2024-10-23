#include "server.h"

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
