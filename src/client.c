// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> 

#define SERVER_IP "127.0.0.1"  //  (localhost)
#define PORT 8080              // Port de connexion du serveur
#define BUFFER_SIZE 1024       // Taille du buffer pour messages

// Initialise la bibliothèque WinSock
extern void init_winsock();

// Crée un socket client
extern SOCKET create_socket();

// Connecte le socket au serveur
void connect_to_server(SOCKET sock, struct sockaddr_in *server) {
    server->sin_addr.s_addr = inet_addr(SERVER_IP); // Convertit IP string vers format binaire
    server->sin_family = AF_INET;                    // Famille IPv4
    server->sin_port = htons(PORT);                  // Port (conversion en Big Endian)

    if (connect(sock, (struct sockaddr *)server, sizeof(*server)) < 0) {
        printf("Connexion échouée.\n");
        closesocket(sock);
        WSACleanup();
        exit(1);
    }

    printf("🟢 Connecté au serveur !\n");
}

// Fonction principale de chat : envoyer/recevoir des messages
void chat(SOCKET sock) {
    char message[BUFFER_SIZE];   // Message à envoyer
    char response[BUFFER_SIZE];  // Réponse du serveur
    int recv_size;

    while (1) {
        printf("✉️  Message à envoyer (\"exit\" pour quitter) : ");
        fgets(message, BUFFER_SIZE, stdin); // Lire message depuis l'utilisateur

        // Retirer le retour à la ligne '\n'
        message[strcspn(message, "\n")] = 0;

        if (strcmp(message, "exit") == 0) break; // Quitter si "exit"

        // Envoi du message au serveur
        if (send(sock, message, strlen(message), 0) < 0) {
            printf("Erreur lors de l'envoi du message.\n");
            break;
        }

        // Réception de la réponse du serveur
        recv_size = recv(sock, response, BUFFER_SIZE - 1, 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Erreur de réception\n");
            break;
        }

        response[recv_size] = '\0'; // Fin de chaîne
        printf("📬 Réponse du serveur : %s\n", response);
    }
}

// // Fonction main : initialisation + appel des fonctions de connexion/chat
// int main() {
// SOCKET sock;
// struct sockaddr_in server;

// init_winsock();              // Initialiser WinSock
// sock = create_socket();      // Créer socket
// connect_to_server(sock, &server); // Se connecter au serveur
// chat(sock);                  // Lancer la session de chat

// closesocket(sock);           // Fermer le socket
// WSACleanup();                // Nettoyer WinSock
// return 0;
// }
