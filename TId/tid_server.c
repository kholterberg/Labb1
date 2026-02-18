#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 3737
#define BUFFERSIZE 30000
//tidsskillnad mellan rfc 868 och unix time
//Jan 1 1900 och Jan 1 1970
#define TIMEDIFF 2208988800UL

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_length = sizeof(client_address);
    char buffer[BUFFERSIZE];

    //skapar socket
    //AF_INET -> ipv4 som localhost: 127.0.0.1
    //SOCK_DGRAM -> udp -> krav för uppift
    //0 -> default protokoll för ipv4 udp
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("socket funka inte");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind funka ej");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("tids server på port %d\n", PORT);

    while (1) {
        recvfrom(server_socket, buffer, BUFFERSIZE, 0, (struct sockaddr *)&client_address, &client_length);

        time_t current_time = time(NULL);

        uint32_t rfc868_time = (uint32_t)(current_time + TIMEDIFF);

        uint32_t network_time = htonl(rfc868_time);

        sendto(server_socket, &network_time, sizeof(network_time), 0, (struct sockaddr *)&client_address, client_length);

        printf("skickat till klient\n");
    }

    close(server_socket);
    return 0;
}