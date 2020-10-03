#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 4444

int main() {

    int sockfd, ret;
    struct sockaddr_in serverAddr;
    socklen_t addSize;

    int newSocket;
    struct sockaddr_in newAddr;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "error in connection\n");
        exit(1);
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        fprintf(stderr, "error in binding\n");
        exit(1);
    }

    printf("Bind to port %d\n", PORT);

    if(listen(sockfd, 10) == 0) {
        printf("Listening ... \n");
    }else {
        printf("error in binding");
    }

    while (1) {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addSize);
        if(newSocket < 0) {
            exit(1);
        }

        printf("Connection accepted");

        if(fork() == 0) {
            close(sockfd);

            while(1) {
                recv(newSocket, buffer, 1024, 0);
                if (strcmp(buffer, "exit") == 0) {
                    printf("Disconnected");
                    break;
                } else {
                    printf("Client: %s\n", buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }

    close(newSocket);

    return 0;

}