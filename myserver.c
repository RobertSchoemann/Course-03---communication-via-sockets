/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int welcomeSocket, newSocket, n;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    /*---- Check if needed args are given ----*/
    /*---- In this case: portnumber ----*/
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    //printf("Portnummer: %d", atoi(argv[1]));
    serverAddr.sin_port = htons(atoi(argv[1]));  //here you could manually set a port
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //printf("Hostname %s", (char *)&serverAddr.sin_addr.s_addr);//did not print a word
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if (listen(welcomeSocket, 5) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
    while (1)
    {
        /*---- Accept call creates a new socket for the incoming connection ----*/
        addr_size = sizeof serverStorage;
        newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

        /*---- Read hellomessage from client ----*/
        bzero(buffer, 256);
        n = read(newSocket, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Received message: %s\n", buffer);

        /*---- Send message to the socket of the incoming connection ----*/
        strcpy(buffer, "Server\n");
        send(newSocket, buffer, 13, 0);
    }

    return 0;
}