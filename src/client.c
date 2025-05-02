#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> 

#define SERVER_IP "127.0.0.1"  //  (localhost)
#define PORT 8080 // Port de connexion du serveur
#define BUFFER_SIZE 2048  // Taille largement suffisante
#define PREFIX "CLIENT:"
#define PREFIX_LEN 8  // Longueur de "CLIENT"


// Initialise la bibliothèque WinSock
extern void init_winsock();

// Crée un socket client
extern SOCKET create_socket();

// Connecte le socket au serveur
void connect_to_server(SOCKET sock, struct sockaddr_in *server) {
    server->sin_addr.s_addr = inet_addr(SERVER_IP);
    server->sin_family = AF_INET;
    server->sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)server, sizeof(*server)) < 0) {
        printf("Connexion échouée.\n");
        closesocket(sock);
        WSACleanup();
        exit(1);
    }
    printf("🟢 Connecté au serveur !\n");
}


void chat(SOCKET sock) {
    char message[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char wrapped_msg[BUFFER_SIZE];
    int recv_size;
    
    while (1) {
        printf("Message à envoyer (\"exit\" pour quitter) : ");
        fgets(message, BUFFER_SIZE - PREFIX_LEN - 1, stdin); 
        
        message[strcspn(message, "\n")] = 0;
        
        if (strcmp(message, "exit") == 0) break;

        // Solution ultime - garantie sans warning
        int written = snprintf(wrapped_msg, BUFFER_SIZE, PREFIX "%s", message);
        if (written < 0 || written >= BUFFER_SIZE) {
            printf("Erreur: problème de formatage du message\n");
            continue;
        }

        if (send(sock, wrapped_msg, strlen(wrapped_msg), 0) < 0) {
            printf("Erreur lors de l'envoi\n");
            break;
        }
        
        recv_size = recv(sock, response, BUFFER_SIZE - 1, 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Erreur de réception\n");
            break;
        }
        
        response[recv_size] = '\0';
        printf("Réponse du serveur : %s\n", response);
    }
}
    
