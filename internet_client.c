#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define INFO_FILE "server_info.txt"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char server_ip[INET_ADDRSTRLEN];
    int server_port;

    // 1. Read server info (IP & Port) from file
    FILE *file = fopen(INFO_FILE, "r");
    if (!file) {
        perror("Cannot find server info file. Start server first!");
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%s\n%d", server_ip, &server_port);
    fclose(file);

    printf("Connecting to server at %s:%d\n", server_ip, server_port);

    // 2. Create the socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 3. Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", server_ip, server_port);

    // 5. Chat loop
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(sock_fd, buffer, strlen(buffer));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting chat...\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int n = read(sock_fd, buffer, BUFFER_SIZE);
        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }

    close(sock_fd);
    return 0;
}
