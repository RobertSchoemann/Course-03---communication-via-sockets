/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    struct hostent *server;

    /*---- Check if needed args are given ----*/
    /*---- In this case: hostname and portnumber ----*/
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Get server by hostname ----*/
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    //printf("Portnummer: %d", atoi(argv[2]));
    serverAddr.sin_port = htons(atoi(argv[2])); //here you could manually set a port


    /* Set IP address to localhost */
    //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memmove((char *)&serverAddr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);


    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
    /*---- Send message to the server ----*/
    send(clientSocket, "CLIENT\n", 6, 0);

    /*---- Read the message from the server into the buffer ----*/
    recv(clientSocket, buffer, 1024, 0);

    /*---- Print the received message ----*/
    printf("Received message: %s", buffer);

    return 0;
}