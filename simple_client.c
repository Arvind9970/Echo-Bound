#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")


#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];
    char server_reply[BUFFER_SIZE];
    int recv_size;
    
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }
    
    // Create socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }
    
    printf("Socket created.\n");
    
    // Prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    
    // Connect to remote server
    printf("Connecting to %s:%d...\n", SERVER_IP, PORT);
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }
    
    printf("Connected to server. Type 'exit' to quit.\n");
    
    // Keep communicating with server
    while (1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        
        // Remove newline character
        message[strcspn(message, "\n")] = '\0';
        
        // Check for exit command
        if (strcmp(message, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }
        
        // Send some data
        if (send(s, message, strlen(message), 0) < 0) {
            printf("Send failed: %d\n", WSAGetLastError());
            break;
        }
        
        // Receive a reply from the server
        if ((recv_size = recv(s, server_reply, sizeof(server_reply) - 1, 0)) <= 0) {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
        
        // Null-terminate the received data
        server_reply[recv_size] = '\0';
        printf("Server reply: %s\n", server_reply);
    }
    
    // Cleanup
    closesocket(s);
    WSACleanup();
    return 0;
}
