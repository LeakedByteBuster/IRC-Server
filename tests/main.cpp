// #include <iostream>
// #include "Client.hpp"
// #include "Server.hpp"

// int main() {

//     /* ..... */
//     std::cout << "Write your tests !" << std::endl;
//     /* ..... */

// }

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a local address and port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Use getsockname to retrieve the local address
    struct sockaddr_in localAddr;
    socklen_t localAddrLen = sizeof(localAddr);

    if (getsockname(sockfd, (struct sockaddr*)&localAddr, &localAddrLen) == -1) {
        perror("Error in getsockname");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Display the local address information
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(localAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
    printf("Local address: %s:%d\n", ipStr, ntohs(localAddr.sin_port));

    // Close the socket
    close(sockfd);

    return 0;
}
