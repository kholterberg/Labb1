#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFERSIZE 30000

const char *get_mime_type(const char *filename) { //funktion för att hitta filtyper
    const char *type = strrchr(filename, '.');
    if(!type) return "text/plain";
    if(strcmp(type, ".html") == 0) return "text/html";
    if(strcmp(type, ".jpg") == 0 || strcmp(type, ".jpeg") == 0) return "image/jpeg";
    //if(strcmp(type, ".css") == 0) return "text/css";
    //if(strcmp(type, ".png") == 0) return "image/png";
    //if(strcmp(type, ".gif") == 0) return "image/gif";
    return "application/octet-stream";
}

void send_file(int socket, const char *filepath){
    FILE *file = fopen(filepath, "rb");

    if(file == NULL){
        char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        write(socket,not_found, strlen(not_found));
        return;
    }

//kollar storleken på filen
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char header[1024];
    sprintf(header, "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "\r\n",
            get_mime_type(filepath), fileSize);

    write(socket, header, strlen(header));

    char *file_buffer = malloc(fileSize);
    fread(file_buffer, 1, fileSize, file);
    write(socket, file_buffer, fileSize);

    free(file_buffer);
    fclose(file);
}

int main(){
    int server, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFERSIZE];

    if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Sockket funkade inte");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server, (struct sockaddr *)&address, sizeof(address)) < 0){ 
        perror("Koppling misslyckades");
        exit(EXIT_FAILURE);
    }

    if(listen(server, 10) < 0){
        perror("Lyssnare misslyckades");
        exit(EXIT_FAILURE);
    }

    printf("Servern körs på port: %d\n", PORT);

    while(1){
        if((client_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
            perror("Accept misslyckades");
            continue;
        }

        read(client_socket, buffer, BUFFERSIZE);

        char method[10], url[100], protocol[10];
        sscanf(buffer, "%s %s %s", method, url, protocol);

        if(strlen(url) > 0){
            char filepath[200];

            if(strcmp(url, "/") == 0){
                sprintf(filepath, "sample_website/index.html");
            }else{
                sprintf(filepath, "sample_website%s", url);
            }

            send_file(client_socket, filepath);
        }

        close(client_socket);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}