#include "server.h"

void server_static_image(int clfd)
{
    FILE *fp = fopen("gojo.jpg", "rb");

    if (fp == NULL)
    {
        char *error_message = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n"
                              "Content-Length: 23\r\n"
                              "Connection: close\r\n"
                              "\r\n"
                              "<h1>File not found</h1>";

        send(clfd, error_message, strlen(error_message), 0);
    }

    /*GET image size*/
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    void *data = malloc(file_size);
    if (data == NULL)
    {
        printf("Error in malloc()\n");
        fclose(fp);
    }
    memset(data, 0, sizeof(data));
    fread(data, 1, file_size, fp);
    fclose(fp);

    /*Prepare the HTTP response headers*/
    char http_reader[256];
    snprintf(http_reader, sizeof(http_reader),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: image/jpg\r\n" // image type
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "\r\n",
             file_size);

    /*send the http header*/
    send(clfd, http_reader, strlen(http_reader), 0);

    // Send the image data
    send(clfd, data, file_size, 0);

    // Free the allocated memory
    free(data);

    // Close the client connection
    close(clfd);
}