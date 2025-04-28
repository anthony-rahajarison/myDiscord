#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>


#pragma comment(lib, "ws2_32.lib")  

void connectToServer(const char *ip, int port) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[1024];


    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erreur lors de l'initialisation de Winsock\n");
        return;
    }


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Erreur de création de socket\n");
        WSACleanup();
        return;
    }


    server.sin_family = AF_INET;
    server.sin_port = htons(port);  
    server.sin_addr.s_addr = inet_addr(ip);  


    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("La connexion au serveur a échoué\n");
        closesocket(sock);
        WSACleanup();
        return;
    }


    printf("Connecté au serveur. Entrez un message: ");
    fgets(message, sizeof(message), stdin);


    send(sock, message, strlen(message), 0);


    closesocket(sock);
    WSACleanup();
}
