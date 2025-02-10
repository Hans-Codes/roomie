#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "8.222.248.88", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock); // Close the socket before exiting
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock); // Close the socket before exiting
        return -1;
    }

    // Listen for messages
    while (1) {
        int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Receive error");
            break;
        } else if (bytes_received == 0) {
            printf("Connection closed by the server.\n");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received string
        printf("%s\n", buffer); // Print the received message
    }

    close(sock); // Ensure the socket is closed before exiting
    return 0;
}