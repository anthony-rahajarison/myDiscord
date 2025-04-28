#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include "server.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 10
SOCKET clients[MAX_CLIENTS];
int client_count = 0;
HANDLE mutex;


unsigned __stdcall handle_client(void *client_socket_ptr) {
    SOCKET client_socket = *(SOCKET *)client_socket_ptr;
    char buffer[1024];
    int bytes;

    while ((bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        printf("Message reçu : %s\n", buffer);


        WaitForSingleObject(mutex, INFINITE);
        for (int i = 0; i < client_count; ++i) {
            if (clients[i] != client_socket) {
                send(clients[i], buffer, bytes, 0);
            }
        }
        ReleaseMutex(mutex);
    }

    WaitForSingleObject(mutex, INFINITE); 
    for (int i = 0; i < client_count; ++i) {
        if (clients[i] == client_socket) {
            clients[i] = clients[client_count - 1]; 
            client_count--;
            break;
        }
    }
    ReleaseMutex(mutex);

    closesocket(client_socket);
    return 0;
}

void start_server(int port) {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);


    WSAStartup(MAKEWORD(2, 2), &wsa);


    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server, sizeof(server));
    listen(server_socket, MAX_CLIENTS);

    printf("Serveur en écoute sur le port %d\n", port);


    mutex = CreateMutex(NULL, FALSE, NULL);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);
        if (client_count < MAX_CLIENTS) {

            WaitForSingleObject(mutex, INFINITE);
            clients[client_count++] = client_socket;
            ReleaseMutex(mutex);


            _beginthreadex(NULL, 0, handle_client, (void*)&clients[client_count - 1], 0, NULL);
        } else {
            printf("Nombre max de clients atteint.\n");
            closesocket(client_socket);
        }
    }


    closesocket(server_socket);
    WSACleanup();
}
