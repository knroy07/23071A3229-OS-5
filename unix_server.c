#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // 1. Create a UNIX domain socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Remove existing socket file and configure address
    unlink(SOCKET_PATH);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 3. Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Listen for client connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: Waiting for a connection...\n");

    // 5. Accept a client connection
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server: Client connected.\n");

    // 6. Chat loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = read(client_fd, buffer, BUFFER_SIZE);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Client: %s", buffer);

        // Exit if client sends "exit"
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting chat...\n");
            break;
        }

        // Send a response
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(client_fd, buffer, strlen(buffer));
    }

    // 7. Cleanup
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
