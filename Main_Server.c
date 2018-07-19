//
// Created by BorisMirage on 7/16/18.
//

/*
 * Main server.
 * This server is responsible for receiving message from client and relay it to server a, b, c.
 * Plus, it will receive message from server a, b, c and send back to client.
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

#define MAIN_SERVER_UDP_PORT "24672"
#define MAIN_SERVER_TCP_PORT "25672"


int main(void) {
    int socketDescriptorTCP;
    int socketDescriptorUDP;
    int listenStatus;
    int TCPBindStatus;
    int UDPBindStatus;
    int acceptClientDescriptor;
    int getAddressStatus;
    char clientMessage[BUFSIZ];
    char receiverBuffer[BUFSIZ];
    char sendDataServerA[BUFSIZ], sendDataServerB[BUFSIZ], sendDataServerC[BUFSIZ];
    char dataClientA[BUFSIZ], dataClientB[BUFSIZ], dataClientC[BUFSIZ];
    char sendClient[BUFSIZ];
    char *time_string;
    time_t current_time;
    struct addrinfo socketAddressTCP, *resultTCP;
    struct addrinfo socketAddressUDP, *resultUDP;
    struct sockaddr_in addressServerA, addressServerB, addressServerC, addressClient;
    socklen_t socketAddressLengthAWS = sizeof(struct sockaddr_in);
    socklen_t addressLengthServerA = sizeof(addressServerA);
    socklen_t addressLengthServerB = sizeof(addressServerB);
    socklen_t addressLengthServerC = sizeof(addressServerC);
    ssize_t receiveClientBytes;
    ssize_t sendClientLength;
    ssize_t sendServerA, sendServerB, sendServerC;
    ssize_t receiveServerA, receiveServerB, receiveServerC;

    /* Create client TCP socket. */
    memset(&socketAddressTCP, 0, sizeof(socketAddressTCP));
    socketAddressTCP.ai_family = AF_INET;
    socketAddressTCP.ai_socktype = SOCK_STREAM;
    socketAddressTCP.ai_flags = AI_PASSIVE;
    getAddressStatus = getaddrinfo(NULL, MAIN_SERVER_TCP_PORT, &socketAddressTCP, &resultTCP);
    if (getAddressStatus != 0) {
        perror("Error occurred when getting address information! \n");             // Return 0 on success.
        return 1;
    }
    socketDescriptorTCP = socket(resultTCP->ai_family, resultTCP->ai_socktype, 0);
    if (socketDescriptorTCP < 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(socketDescriptorTCP));
        perror("Error occurred when set up TCP socket! \n");
        return 1;
    }

    /* Bind client TCP socket. */
    TCPBindStatus = bind(socketDescriptorTCP, resultTCP->ai_addr, resultTCP->ai_addrlen);       // Bind TCP socket.
    if (TCPBindStatus < 0) {
        perror("Error occurred when binding TCP socket! \n");
        return 1;
    }

    /* UDP socket setup and bind. */
    memset(&socketAddressUDP, 0, sizeof(socketAddressUDP));
    socketAddressUDP.ai_family = AF_INET;
    socketAddressUDP.ai_socktype = SOCK_DGRAM;
    socketAddressUDP.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, MAIN_SERVER_UDP_PORT, &socketAddressUDP, &resultUDP);
    socketDescriptorUDP = socket(resultUDP->ai_family, resultUDP->ai_socktype, resultUDP->ai_protocol);
    if (socketDescriptorUDP < 0) {
        perror("Error occurred when creating UDP socket! \n");
        return 1;
    }
    UDPBindStatus = bind(socketDescriptorUDP, resultUDP->ai_addr, resultUDP->ai_addrlen);
    if (UDPBindStatus < 0) {
        perror("Error occurred when binding UDP socket! \n");
        return 1;
    }

    /* Server A */
    memset((char *) &addressServerA, 0, sizeof(addressServerA));
    addressServerA.sin_family = AF_INET;
    addressServerA.sin_port = htons(21672);     // Server A UDP port.
    if (inet_pton(AF_INET, localhost, &addressServerA.sin_addr) < 0) {
        perror("Error occurred when converting server A address information! \n");
    }

    /* Server B */
    memset((char *) &addressServerB, 0, sizeof(addressServerB));
    addressServerB.sin_family = AF_INET;
    addressServerB.sin_port = htons(22672);     // Server B UDP port.
    if (inet_pton(AF_INET, localhost, &addressServerB.sin_addr) < 0) {
        perror("Error occurred when converting server B address information! \n");
    }

    /* Server C */
    memset((char *) &addressServerC, 0, sizeof(addressServerC));
    addressServerC.sin_family = AF_INET;
    addressServerC.sin_port = htons(23672);     // Server C UDP port.
    if (inet_pton(AF_INET, localhost, &addressServerC.sin_addr) < 0) {
        perror("Error occurred when converting server C address information! \n");
    }

    printf("Main Server online. \n");

    /* Listen to TCP traffic. */
    listenStatus = listen(socketDescriptorTCP, 1);
    if (listenStatus < 0) {
        perror("listen");
        perror("Error occurred when listening TCP traffic! \n");
        return 1;
    }

    /* Retrieve the locally-bound name of the specified socket and store it in the structure. */
    int getSocket;
    getSocket = getsockname(socketDescriptorTCP, (struct sockaddr *) &socketAddressTCP,
                            (socklen_t *) &socketAddressTCP);

    if (getSocket == -1) {
        perror("Error occurred when getting socket! \n");
        exit(1);
    }

    while (1) {

        /* Accept data from client. */
        acceptClientDescriptor = accept(socketDescriptorTCP, (struct sockaddr *) &addressClient,
                                        &socketAddressLengthAWS);
        if (acceptClientDescriptor < 0) {
            perror("Error occurred when accept data from client! \n");
            exit(1);
        }

        /* Receive data from client. */
        receiveClientBytes = recv(acceptClientDescriptor, receiverBuffer, sizeof(receiverBuffer), 0);
        if (receiveClientBytes == -1) {
            perror("Error occurred when receiving data! \n");
            return 1;
        }

        /* If recv = 0, remote side has closed the connection. */
        if (receiveClientBytes == 0) {
            close(socketDescriptorTCP);
            return 0;
        }


        // TODO: Rewrite message sent from main server and distribute server a, b, c.


        /* After data received, send them to server A, B, C. */
        if (receiveClientBytes > 0) {

            /* Get current time */
            current_time = time(NULL);
            time_string = ctime(&current_time);

//            printf("Receive message '%s' from the client using TCP over port 25672 at %s. \n", message, time_string);
            printf("%.*s Receive one message from Client: \n", (int) strlen(time_string) - 1, time_string);
            printf("%s \n", receiverBuffer);
            strcat(clientMessage, receiverBuffer);

            /* Message initialization. */
            memset(sendDataServerA, '\0', sizeof(sendDataServerA));
            memset(sendDataServerB, '\0', sizeof(sendDataServerB));
            memset(sendDataServerC, '\0', sizeof(sendDataServerC));
            strcat(sendDataServerA, clientMessage);
            strcat(sendDataServerB, clientMessage);
            strcat(sendDataServerC, clientMessage);

            /* Relay client message to Server A */
            sendServerA = sendto(socketDescriptorUDP, sendDataServerA, sizeof(sendDataServerA), 0,
                                 (struct sockaddr *) &addressServerA,
                                 sizeof(addressServerA));
            if (sendServerA < 0) {
                perror("Error occurred when send message to server A! \n");
            } else {

                /* Get current time */
                current_time = time(NULL);
                time_string = ctime(&current_time);

                printf("%.*s Send one message to Server A. \n", (int) strlen(time_string) - 1, time_string);
                printf("%s \n", clientMessage);
            }

            /* Relay client message to Server B. */
            sendServerB = sendto(socketDescriptorUDP, sendDataServerB, sizeof(sendDataServerB), 0,
                                 (struct sockaddr *) &addressServerB,
                                 sizeof(addressServerB));
            if (sendServerB < 0) {
                perror("Error occurred when send message to server B! \n");
            } else {

                /* Get current time */
                current_time = time(NULL);
                time_string = ctime(&current_time);

                printf("%.*s Send one message to Server B. \n", (int) strlen(time_string) - 1, time_string);
                printf("%s \n", clientMessage);

            }

            /* Relay client message to Server C. */
            sendServerC = sendto(socketDescriptorUDP, sendDataServerC, sizeof(sendDataServerC), 0,
                                 (struct sockaddr *) &addressServerC,
                                 sizeof(addressServerC));
            if (sendServerC < 0) {
                printf("Error occurred when send message to server C! \n");
            } else {
                /* Get current time */
                current_time = time(NULL);
                time_string = ctime(&current_time);

                printf("%.*s Send one message to Server C. \n", (int) strlen(time_string) - 1, time_string);
                printf("%s \n", clientMessage);
            }

            /* Receive message from server A */
            receiveServerA = recvfrom(socketDescriptorUDP, dataClientA, sizeof(dataClientA), 0,
                                      (struct sockaddr *) &addressServerA, &addressLengthServerA);
            if (receiveServerA == -1) {
                perror("Error occurred when receive message from server A! \n");
            } else {
//                printf("Message '%s' received from Server A using UDP over port 24672. \n", dataClientA);

                /* Get current time */
                current_time = time(NULL);
                time_string = ctime(&current_time);

                printf("%.*s Received one client message from Server A. \n", (int) strlen(time_string) - 1,
                       time_string);
                printf("%s \n", dataClientA);
            }

            /* Receive message from server B */
            receiveServerB = recvfrom(socketDescriptorUDP, dataClientB, sizeof(dataClientB), 0,
                                      (struct sockaddr *) &addressServerB, &addressLengthServerB);
            if (receiveServerB == -1) {
                perror("Error occurred when receive client message from server B. \n");
            } else {

                /* Get current time */
                current_time = time(NULL);
                time_string = ctime(&current_time);

                printf("%.*s Received one client message from Server B. \n", (int) strlen(time_string) - 1,
                       time_string);
                printf("%s \n", dataClientB);
            }

            /* Receive message from server C */
            receiveServerC = recvfrom(socketDescriptorUDP, dataClientC, sizeof(dataClientC), 0,
                                      (struct sockaddr *) &addressServerC, &addressLengthServerC);
            if (receiveServerC == -1) {
                printf("Error occurred when receive message from server C! \n");
            } else {

                /* Get current time */
                current_time = time(NULL);
                time_string = ctime(&current_time);

                printf("%.*s Received one message from Server C. \n", (int) strlen(time_string) - 1, time_string);
                printf("%s \n", dataClientC);
            }

            memset(sendClient, '\0', sizeof(sendClient));
//            current_time = time(NULL);
//            time_string = ctime(&current_time);

            strcat(sendClient, "All server received message. \n");

            sendClientLength = send(acceptClientDescriptor, sendClient, sizeof(sendClient), 0);
            if (sendClientLength < 0) {
                perror("Error occurred when send message back to client! \n");
                return 1;
            } else {

                printf("%.*s Send message to client: \n", (int) strlen(time_string) - 1, time_string);
                printf("%s \n", sendClient);
            }
        }
    }
}

