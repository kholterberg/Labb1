#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 3737
#define TIMEDIFF 2208988800UL

int main() {
    int clientsocket;
    struct sockaddr_in serveraddress;
    uint32_t network_time;

    clientsocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientsocket < 0) {
        perror("Socket misslyckades");
        exit(EXIT_FAILURE);
    }

    memset(&serveraddress, 0, sizeof(serveraddress));
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    //127.0.0.1 - localhost
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(clientsocket, "", 0, 0, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    printf("req skickat till server\n");

    socklen_t server_length = sizeof(serveraddress);
    recvfrom(clientsocket, &network_time, sizeof(network_time), 0, (struct sockaddr *)&serveraddress, &server_length);

    uint32_t rfc868_time = ntohl(network_time);
    time_t unix_time = (time_t)(rfc868_time - TIMEDIFF);

    printf("tog emot tid från server\n");
    printf("%s", ctime(&unix_time));

    close(clientsocket);
    
    return 0;

}