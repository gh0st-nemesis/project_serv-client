#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define SAVE_DIRECTORY "./"
#define SAVE_FILE "reception.tar.gz"

void initialize_server(int *server_sock) {
    *server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);
    bind(*server_sock, (struct sockaddr *)&server, sizeof(server));
    listen(*server_sock, 3);
}

void accept_connection(int server_sock, int *client_sock) {
    socklen_t c = sizeof(struct sockaddr_in);
    *client_sock = accept(server_sock, NULL, &c);
}

void receive_file(int client_sock) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(SAVE_FILE, "wb");
    size_t read_size;

    while ((read_size = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, read_size, file);
    }

    fclose(file);
}

void close_sockets(int client_sock, int server_sock) {
    close(client_sock);
    close(server_sock);
}

int main() {
    int server_sock, client_sock;

    initialize_server(&server_sock);
    printf("Server listening on port %d\n", SERVER_PORT);

    accept_connection(server_sock, &client_sock);
    printf("Connection accepted\n");

    receive_file(client_sock);
    printf("File received and saved as %s\n", SAVE_FILE);

    close_sockets(client_sock, server_sock);
    return 0;
}
