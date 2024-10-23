
#include "server.h"

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
