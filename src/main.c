#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <winsock2.h>
#include "server.h"

#define PORT 8080
#define MAX_CLIENTS 10


void handleClient(SOCKET client_sock, PGconn *conn);
void run_server(PGconn *conn);
void storeMessage(PGconn *conn, int channel_id, int sender_id, const char *content, int is_private, int encrypted);
void exitOnError(PGconn *conn);

void exitOnError(PGconn *conn) {
    fprintf(stderr, "Erreur PostgreSQL : %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erreur lors de l'initialisation de Winsock\n");
        exit(1);
    }

    // Connexion à la base de données PostgreSQL
    PGconn *conn = PQconnectdb("user=postgres password=ton_mot_de_passe dbname=myDiscord host=localhost");
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Impossible de se connecter à la base de données : %s", PQerrorMessage(conn));
        PQfinish(conn);
        WSACleanup();
        exit(1);
    }


    run_server(conn);

    // Exemple d'ajout de message (hors de la boucle de gestion des clients)
    int channel_id = 1;     
    int sender_id = 2;      // ID de l'utilisateur qui envoie le message
    const char *content = "Hello, bienvenue sur MyDiscord!";  
    int is_private = 0;     
    int encrypted = 0;      

    // Ajouter un message à la base de données
    storeMessage(conn, channel_id, sender_id, content, is_private, encrypted);

    // Fermer la connexion à la base de données après la fin du programme
    PQfinish(conn);

    WSACleanup();

    return 0;
}

// Fonction pour gérer la discussion avec le client
void handleClient(SOCKET client_sock, PGconn *conn) {
    char buffer[1024];
    int recv_size;

    // Envoi d'un message d'accueil
    send(client_sock, "Bienvenue sur MyDiscord!\n", 24, 0);

    while (1) {
        // Réception d'un message du client
        recv_size = recv(client_sock, buffer, sizeof(buffer), 0);
        if (recv_size <= 0) {
            printf("Déconnexion du client.\n");
            break;
        }

        buffer[recv_size] = '\0'; // Terminer la chaîne de caractères

        // Affichage du message reçu
        printf("Message reçu : %s\n", buffer);

        // Exemple d'insertion du message dans la base de données
        storeMessage(conn, 1, 2, buffer, 0, 0);  // 1 est l'ID du canal, 2 est l'ID de l'utilisateur

        // Réponse au client
        send(client_sock, "Message reçu et sauvegardé.\n", 27, 0);
    }

    closesocket(client_sock);
}

// Fonction pour démarrer le serveur
void run_server(PGconn *conn) {
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

        // Création du socket serveur
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == INVALID_SOCKET) {
            perror("Erreur lors de la création du socket");
            exit(1);
        }

    // Configuration de l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Lier le socket à l'adresse
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors du binding");
        exit(1);
    }

    // Écouter les connexions
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Erreur lors de l'écoute");
        exit(1);
    }

    printf("Serveur en écoute sur le port %d...\n", PORT);

    // Accepter et gérer les connexions entrantes
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET){  
            perror("Erreur lors de l'acceptation de la connexion");
            continue;
        }

        printf("Un client est connecté.\n");

        // Gérer la communication avec le client
        handleClient(client_socket, conn);
    }

    closesocket(server_socket);
}

// Fonction pour enregistrer un message dans la base de données
void storeMessage(PGconn *conn, int channel_id, int sender_id, const char *content, int is_private, int encrypted) {
    char channel_id_str[12], sender_id_str[12], is_private_str[2], encrypted_str[2];
    snprintf(channel_id_str, sizeof(channel_id_str), "%d", channel_id);
    snprintf(sender_id_str, sizeof(sender_id_str), "%d", sender_id);
    snprintf(is_private_str, sizeof(is_private_str), "%d", is_private);
    snprintf(encrypted_str, sizeof(encrypted_str), "%d", encrypted);

    const char *params[5] = { 
        channel_id_str, 
        sender_id_str, 
        content, 
        is_private_str, 
        encrypted_str
    };

    PGresult *res = PQexecParams(conn, "INSERT INTO messages (channel_id, sender_id, content, is_private, encrypted) VALUES ($1, $2, $3, $4, $5)", 5, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Erreur lors de l'enregistrement du message : %s\n", PQerrorMessage(conn));
    } else {
        printf("Message enregistré dans la base de données\n");
    }

    PQclear(res);
}
