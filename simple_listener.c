#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock, client_sock;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    int client_len, bytes_received;
    
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    printf("Socket created.\n");
    
    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    // Bind
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    printf("Bind done.\n");
    
    // Listen for incoming connections
    listen(sock, 3);
    printf("Waiting for incoming connections on port %d...\n", PORT);
    
    // Accept connection
    client_len = sizeof(struct sockaddr_in);
    client_sock = accept(sock, (struct sockaddr *)&client, &client_len);
    if (client_sock == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    
    printf("Connection accepted from %s:%d\n", 
           inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    
    // Receive and send back data
    while ((bytes_received = recv(client_sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);
        
        // Echo back
        send(client_sock, buffer, bytes_received, 0);
    }
    
    if (bytes_received == 0) {
        printf("Client disconnected\n");
    } else if (bytes_received == SOCKET_ERROR) {
        printf("recv failed: %d\n", WSAGetLastError());
    }
    
    // Cleanup
    closesocket(client_sock);
    closesocket(sock);
    WSACleanup();
    return 0;
}
