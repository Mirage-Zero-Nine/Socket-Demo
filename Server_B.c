//
// Created by BorisMirage on 7/16/18.
//

/*
 * Server B.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>

#define localhost "127.0.0.1"

#define UDP_PORT_SERVER_B "22672"

int main() {

    int socketDescriptor;
    int bindStatus;
    int addressStatus;
    char receiveBuffer[BUFSIZ];     // BUFSIZ = 1024, defined in stdio.h
    char *time_string;
    struct addrinfo socketAddress, *addressResult;
    struct sockaddr_storage socketAddressStorage;
    time_t current_time;
    ssize_t receiveBytes;
    ssize_t sendStatus;
    socklen_t sizeOfStructure = sizeof(socketAddressStorage);

    /* Obtain information of host and check. */
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.ai_family = AF_INET;
    socketAddress.ai_socktype = SOCK_DGRAM;
    socketAddress.ai_flags = 0;
    addressStatus = getaddrinfo(NULL, UDP_PORT_SERVER_B, &socketAddress, &addressResult);
    if (addressStatus != 0) {
        perror("Error occurred when get the information of address! \n");
        return 1;
    }

    /* Create and bind socket. */
    socketDescriptor = socket(addressResult->ai_family, addressResult->ai_socktype, addressResult->ai_flags);
    if (socketDescriptor < 0) {
        perror("Error occurred when creating socket! \n");
        return 1;
    }
    bindStatus = bind(socketDescriptor, addressResult->ai_addr, addressResult->ai_addrlen);
    if (bindStatus < 0) {
        perror("Error occurred when when binding! \n");
        return 1;
    }

    printf("Server B is up and running using UDP on port 22672. \n");

    while (1) {

        /* Receive message */
        memset(receiveBuffer, '\0', sizeof(receiveBuffer));
        receiveBytes = recvfrom(socketDescriptor, receiveBuffer, sizeof receiveBuffer, 0,
                                (struct sockaddr *) &socketAddressStorage, &sizeOfStructure);
        if (receiveBytes == -1) {
            perror("Error occurred when receiving messages from Main Server! \n");
            return 1;
        }

        /* Get current time */
        current_time = time(NULL);
        time_string = ctime(&current_time);

        /* Send confirm message back */
        printf("Server B received message '%s' at '%s. \n", receiveBuffer, time_string);
        sendStatus = sendto(socketDescriptor, time_string, sizeof(time_string), 0,
                            (struct sockaddr *) &socketAddressStorage, sizeOfStructure);
        if (sendStatus < 0) {
            perror("Error occurred when sending message! \n");
            return 1;
        } else {
            printf("Server B has sent message. \n");
        }
    }
}