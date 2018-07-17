//
// Created by BorisMirage on 7/16/18.
//

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
#include <time.h>

#define localhost "127.0.0.1"

#define MAIN_SERVER_TCP_PORT "25672"

/* TCP port on client is dynamic assignment.
 * Client connects to server on port 25672. */

int main(int argc, char *argv[]) {

    int socketDescriptor;                   // Socket descriptor to server.
    int connectStatus;
    int getAddressStatus;
    char stringFunction[10];
    char receiveMessage[BUFSIZ];
    char sendMessage[BUFSIZ];
    char *time_string;
    struct addrinfo socketAddress;
    struct addrinfo *addressResult;         // Address information result.
    ssize_t receiveMessageSize;             // Socket structure
    ssize_t sendMessageByte;
    time_t current_time;
    time_t current_time_arr;

    if (argc > 2) {
        printf("Only accept one argument!");
        return -1;
    }

    /* Structure initialization . */
    memset(&socketAddress, 0, sizeof(socketAddress));       // Empty structure.
    socketAddress.ai_family = AF_INET;
    socketAddress.ai_socktype = SOCK_STREAM;                // TCP Socket
    socketAddress.ai_flags = AI_PASSIVE;

    /* Get address information. */
    getAddressStatus = getaddrinfo(NULL, MAIN_SERVER_TCP_PORT, &socketAddress, &addressResult);
    if (getAddressStatus != 0) {
        perror("Error occurred when getting address information");
        return 1;
    }

    /* Create socket. */
    socketDescriptor = socket(addressResult->ai_family, addressResult->ai_socktype, addressResult->ai_protocol);
    if (socketDescriptor < 0) {
        perror("Error occurred when creating socket");
        return 1;
    }

    /* Connect socket. */
    connectStatus = connect(socketDescriptor, addressResult->ai_addr, addressResult->ai_addrlen);
    if (connectStatus < 0) {
        perror("Error occurred when connecting to socket");
        return 1;
    }

    printf("The client is up and running! \n");

    /* Send message to server and receive the return value. */
    memset(sendMessage, '\0', sizeof(sendMessage));     // String initialization, '\0' -> null.

    current_time = time(NULL);
    time_string = ctime(&current_time);

    if (argc == 1) {
        strcat(sendMessage, argv[0]);
    } else {
        strcat(sendMessage, time_string);
    }

    /* Send message. */
    sendMessageByte = send(socketDescriptor, sendMessage, sizeof(sendMessage), 0);      // send(): TCP connection.
    if (sendMessageByte < 0) {
        perror("Send Message Length Error. \n");                                // Check success.
    }

    printf("The client sent '%s' at %s. \n", sendMessage, time_string);

    /* Receive message from the server.*/
    memset(&receiveMessage, '\0', sizeof(receiveMessage));
    receiveMessageSize = recv(socketDescriptor, receiveMessage, sizeof(receiveMessage),
                              0);      // recv(): TCP connection.

    if (receiveMessageSize < 0) {
        perror("Error occurred when receiving message");
    }

    current_time_arr = time(NULL);
    time_string = ctime(&current_time_arr);

    printf("Receive message %s at %s", stringFunction, time_string);

    /* Close socket. */
    close(socketDescriptor);
    return 0;
}



