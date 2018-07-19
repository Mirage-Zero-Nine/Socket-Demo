//
// Created by BorisMirage on 7/16/18.
//

/*
 * This is the client that send message to main server.
 * Execute by terminal command ./client after make all
 * Input message after message is shown in screen.
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
#include <time.h>

#define localhost "127.0.0.1"

#define MAIN_SERVER_TCP_PORT "25672"

/* TCP port on client is dynamic assignment.
 * Client connects to server on port 25672.
 */

int main(int argc, char *argv[]) {

    int socketDescriptor;                   // Socket descriptor to server.
    int connectStatus;
    int getAddressStatus;
    char receiveMessage[BUFSIZ];
//    char sendMessage[BUFSIZ];
    char *time_string;
    char inputMessage[BUFSIZ];
    struct addrinfo socketAddress;
    struct addrinfo *addressResult;         // Address information result.
    ssize_t receiveMessageSize;             // Socket structure
    ssize_t sendMessageByte;
    time_t current_time;

    if (argc > 2) {
        printf("Only accept one argument!");
        return -1;
    }

    while (1) {

        /* If there is only execution command as terminal input */
        if (argc == 1) {
            printf("Enter message: \n");
            scanf("%s", inputMessage);
        }

        /* Exit command and close socket. */
        if (strcmp(inputMessage, "exit") == 0) {
            close(socketDescriptor);
            return 0;
        }

        /* Structure initialization . */
        memset(&socketAddress, 0, sizeof(socketAddress));       // Empty structure.
        socketAddress.ai_family = AF_INET;
        socketAddress.ai_socktype = SOCK_STREAM;                // TCP Socket
        socketAddress.ai_flags = AI_PASSIVE;

        /* Get address information. */
        getAddressStatus = getaddrinfo(NULL, MAIN_SERVER_TCP_PORT, &socketAddress, &addressResult);
        if (getAddressStatus != 0) {
            perror("Error occurred when getting address information! \n");
            return 1;
        }

        /* Create socket. */
        socketDescriptor = socket(addressResult->ai_family, addressResult->ai_socktype, addressResult->ai_protocol);
        if (socketDescriptor < 0) {
            perror("Error occurred when creating socket! \n");
            return 1;
        }

        /* Connect socket. */
        connectStatus = connect(socketDescriptor, addressResult->ai_addr, addressResult->ai_addrlen);
        if (connectStatus < 0) {
            perror("Error occurred when connecting to socket! \n");
            return 1;
        }

        /* Send message. */
        sendMessageByte = send(socketDescriptor, inputMessage, sizeof(inputMessage), 0);   // send(): TCP connection.
        if (sendMessageByte < 0) {
            perror("Error occurred when sending message! \n");
        }

        /* Get current time */
        current_time = time(NULL);
        time_string = ctime(&current_time);

        printf("%.*s Client sent a message: \n", (int) strlen(time_string) - 1, time_string);
        printf("%s \n", inputMessage);

        /* Receive message from the server.*/
        memset(&receiveMessage, '\0', sizeof(receiveMessage));
        receiveMessageSize = recv(socketDescriptor, receiveMessage, sizeof(receiveMessage), 0);     // TCP connection.

        if (receiveMessageSize < 0) {
            perror("Error occurred when receiving message! \n");
        }

        /* Get current time */
        current_time = time(NULL);
        time_string = ctime(&current_time);

        printf("%.*s Receive one message: \n", (int) strlen(time_string) - 1, time_string);
        printf("%s \n", receiveMessage);
    }
}


