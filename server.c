#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
int client_count = 0;
FILE *log_file;

void log_event(const char *event) {
    if (log_file) {
        fprintf(log_file, "%s\n", event);
        fflush(log_file);
    }
}

void *handle_client(void *arg) {
    int sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    char username[50], password[50];

    // Receive username and password
    recv(sock, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%s %s", username, password);

    // Log authentication attempt
    char log_message[256]; // Increased size to prevent truncation

    snprintf(log_message, sizeof(log_message), "User authenticated: %s", username);
    log_event(log_message);

    // Create user directory if it doesn't exist
    mkdir("users", 0755);
    char filepath[100];
    snprintf(filepath, sizeof(filepath), "users/%s.txt", username);
    FILE *file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "%s\n%s\n", username, password);
        fclose(file);
    } else {
        perror("Failed to create user file");
        close(sock);
        return NULL;
    }

    // Broadcast messages to all clients
    while (1) {
        int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            perror("Client disconnected");
            break;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received string

        // Log the message being broadcasted
        snprintf(log_message, sizeof(log_message), "Broadcasting message: %s", buffer);
        log_event(log_message);

        // Broadcast to all connected clients
        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] != sock) {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
    }

    // Remove the client socket and close the connection
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == sock) {
            client_sockets[i] = client_sockets[--client_count]; // Remove client
            break;
        }
    }
    close(sock);
    return NULL;
}

int main() {
    int server_sock, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Open log file
    log_file = fopen("logs.txt", "a");
    if (!log_file) {
        perror("Failed to open log file");
        return -1;
    }

    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        return -1;
    }

    log_event("Server started and listening");

    while (1) {
        // Accept a new client
        if ((new_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Add new client to the list
        if (client_count < MAX_CLIENTS) {
            client_sockets[client_count++] = new_sock;
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, (void *)&new_sock);
        } else {
            printf("Max clients reached. Connection rejected.\n");
            close(new_sock);
        }
    }

    fclose(log_file);
    close(server_sock);
    return 0;
}
