#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/chat_socket"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // 1. Create a UNIX domain socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set up the server address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 3. Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client: Connected to server.\n");

    // 4. Chat loop
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(sock_fd, buffer, strlen(buffer));

        // Exit if "exit" is sent
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting chat...\n");
            break;
        }

        // Read server response
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
