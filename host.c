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
    char username[50], password[50];

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
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Get username and password
    printf("Enter username: ");
    scanf("%49s", username); // Prevent buffer overflow
    printf("Enter password: ");
    scanf("%49s", password); // Prevent buffer overflow

    // Send username and password to server and log the attempt
    printf("Authenticating user: %s\n", username); // Log authentication attempt

    snprintf(buffer, sizeof(buffer), "%s %s", username, password);
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("Send error");
        close(sock);
        return -1;
    }

    // Send messages and log the message

    while (1) {
        printf("Enter message: "); // Prompt for message

        fgets(buffer, sizeof(buffer), stdin); // Read the message
        printf("Sending message: %s\n", buffer); // Log the message being sent

        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Send error");
            break;
        }
    }

    close(sock);
    return 0;
}
