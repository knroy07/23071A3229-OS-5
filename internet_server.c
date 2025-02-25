#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>

#define BUFFER_SIZE 1024
#define INFO_FILE "server_info.txt"

// Function to get the local IP address
void get_local_ip(char *ip_buffer) {
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            if (strcmp(ifa->ifa_name, "lo") != 0) {  // Ignore localhost
                strcpy(ip_buffer, inet_ntoa(addr->sin_addr));
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char ip_address[INET_ADDRSTRLEN];

    // 1. Create the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server settings
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = 0;  // Let OS assign a free port

    // 3. Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Get assigned port
    socklen_t len = sizeof(server_addr);
    getsockname(server_fd, (struct sockaddr *)&server_addr, &len);
    int assigned_port = ntohs(server_addr.sin_port);

    // 5. Get the local IP address
    get_local_ip(ip_address);

    // 6. Save server info (IP & Port) to a file
    FILE *file = fopen(INFO_FILE, "w");
    if (file) {
        fprintf(file, "%s\n%d\n", ip_address, assigned_port);
        fclose(file);
    }
    printf("Server running at IP: %s, Port: %d\n", ip_address, assigned_port);

    // 7. Listen for connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a client to connect...\n");

    // 8. Accept a client connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // 9. Chat loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = read(client_fd, buffer, BUFFER_SIZE);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Client: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Closing connection...\n");
            break;
        }

        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(client_fd, buffer, strlen(buffer));
    }

    close(client_fd);
    close(server_fd);
    remove(INFO_FILE);  // Clean up file after server stops
    return 0;
}
