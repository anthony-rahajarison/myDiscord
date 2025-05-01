#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <libpq-fe.h>
#include "server.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

// Initialisation de WinSock
void init_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Échec de l'initialisation de WinSock. Code erreur : %d\n", WSAGetLastError());
        exit(1);
    }
}

// Création d'une socket
SOCKET create_socket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Erreur de socket : %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return sock;
}

// Fonction pour lier le serveur à un port et à une adresse IP
void bind_server(SOCKET sock, struct sockaddr_in *server) {
    // bind le socket au port et à l'adresse du serveur
    if (bind(sock, (struct sockaddr *)server, sizeof(*server)) == SOCKET_ERROR) {
        printf("Erreur de bind : %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(1); // On arrête l'exécution si bind échoue
    }
}

// Fonction pour enregistrer le message dans la base de données
void save_message_to_db(PGconn *conn, int channel_id, int sender_id, const char *message) {
    const char *query = "INSERT INTO messages (channel_id, sender_id, content) VALUES ($1, $2, $3)";
    const char *params[3] = { (char *)&channel_id, (char *)&sender_id, message };
    
    PGresult *res = PQexecParams(conn, query, 3, NULL, params, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Erreur lors de l'enregistrement du message : %s\n", PQerrorMessage(conn));
    } else {
        printf(" Message enregistré dans la base de données\n");
    }
    PQclear(res);
}

// Fonction pour récupérer et envoyer les anciens messages à partir de la base de données
void send_past_messages(SOCKET client_sock, PGconn *conn, int channel_id) {
    const char *query = "SELECT content FROM messages WHERE channel_id = $1 ORDER BY timestamp ASC";
    PGresult *res = PQexecParams(conn, query, 1, NULL, (const char **)&channel_id, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("Erreur lors de la récupération des messages : %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        const char *message = PQgetvalue(res, i, 0);
        send(client_sock, message, strlen(message), 0);
        send(client_sock, "\n", 1, 0); // Ajout d'un saut de ligne entre les messages
    }

    PQclear(res);
}

// Fonction pour gérer la discussion avec le client
void handle_chat(SOCKET client_sock, PGconn *conn, int channel_id) {
    // Envoyer les messages passés à partir de la base de données
    send_past_messages(client_sock, conn, channel_id);

    char message[BUFFER_SIZE];
    int recv_size;

    while (1) {
        recv_size = recv(client_sock, message, BUFFER_SIZE, 0);
        if (recv_size <= 0) {
            break;
        }

        message[recv_size] = '\0'; // Ajout d'un caractère nul pour la chaîne de caractères
        printf("Message reçu : %s\n", message);

        // Sauvegarder le message dans la base de données
        save_message_to_db(conn, channel_id, 1, message);  // 1 étant l'ID de l'expéditeur (pour l'exemple)

        // Envoyer un accusé de réception au client
        send(client_sock, "Message reçu et sauvegardé\n", 25, 0);
    }
}

// Fonction qui démarre le serveur de chat
void start_chat_server() {
    SOCKET sock;
    struct sockaddr_in server, client;
    int client_size = sizeof(client);
    PGconn *conn = PQconnectdb("dbname=myDiscord user=postgres password=yourpassword");

    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Connexion à la base de données échouée : %s\n", PQerrorMessage(conn));
        exit(1);
    }

    init_winsock();
    sock = create_socket();

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  // Écoute sur toutes les interfaces réseau
    server.sin_port = htons(PORT);        // Spécifie le port

    // Appel de la fonction bind_server() au lieu du code de bind directement
    bind_server(sock, &server);

    if (listen(sock, 3) == SOCKET_ERROR) {
        printf("Erreur de listen : %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        PQfinish(conn);
        exit(1);
    }

    printf("Serveur à l'écoute sur %s:%d...\n", SERVER_IP, PORT);

    while (1) {
        SOCKET client_sock = accept(sock, (struct sockaddr *)&client, &client_size);
        if (client_sock == INVALID_SOCKET) {
            printf("Erreur d'acceptation de la connexion : %d\n", WSAGetLastError());
            continue;
        }

        printf("Nouvelle connexion depuis %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Appel de la fonction handle_chat() pour gérer la discussion
        handle_chat(client_sock, conn, 1); // 1 est l'ID du canal pour cet exemple

        closesocket(client_sock);
    }

    closesocket(sock);
    WSACleanup();
    PQfinish(conn);
}
