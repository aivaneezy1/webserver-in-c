#include "server.h"

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