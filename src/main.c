#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <libpq-fe.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048
#define DB_CONN_STRING "user=postgres password=mdp dbname=myDiscord host=localhost"

typedef struct {
    SOCKET socket;
    int authenticated;
    int user_id;
} Client;

/* Déclarations des fonctions */
extern void init_winsock();


int main() {
    // WSADATA wsa;
    SOCKET server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    PGconn *db_conn;
    Client clients[MAX_CLIENTS] = {0};
    int current_clients = 0;
    fd_set readfds;
    int max_sd, activity, i;

    /* Initialisation */
    init_winsock();
    server_sock = create_socket();
    db_conn = connect_to_database();

    /* Configuration du serveur */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("bind failed");
        cleanup(server_sock, db_conn);
        return 1;
    }

    if (listen(server_sock, MAX_CLIENTS) == SOCKET_ERROR) {
        perror("listen failed");
        cleanup(server_sock, db_conn);
        return 1;
    }

    printf("Serveur démarré sur le port %d\n", PORT);

    /* Boucle principale */
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_sock, &readfds);
        max_sd = server_sock;

        /* Ajouter les sockets clients */
        for (i = 0; i < current_clients; i++) {
            if (clients[i].socket > 0) {
                FD_SET(clients[i].socket, &readfds);
            }
            if (clients[i].socket > max_sd) {
                max_sd = clients[i].socket;
            }
        }

        /* Attendre une activité sur un socket */
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity == SOCKET_ERROR) {
            perror("select error");
            continue;
        }

        /* Nouvelle connexion */
        if (FD_ISSET(server_sock, &readfds)) {
            client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
            if (client_sock == INVALID_SOCKET) {
                perror("accept failed");
                continue;
            }

            if (current_clients < MAX_CLIENTS) {
                clients[current_clients].socket = client_sock;
                clients[current_clients].authenticated = 0;
                current_clients++;
                printf("Nouveau client connecté (%d/%d)\n", current_clients, MAX_CLIENTS);
                send(client_sock, "Bienvenue sur MyDiscord! Veuillez vous authentifier.\n", 52, 0);
            } else {
                send(client_sock, "Le serveur est complet. Réessayez plus tard.\n", 47, 0);
                closesocket(client_sock);
            }
        }

        /* Vérifier les clients existants */
        for (i = 0; i < current_clients; i++) {
            if (FD_ISSET(clients[i].socket, &readfds)) {
                handle_client(clients[i].socket, db_conn);
            }
        }
    }

    cleanup(server_sock, db_conn);
    return 0;
}

/* Implémentations des fonctions */
void init_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        exit(1);
    }
}

SOCKET create_server_socket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return sock;
}

PGconn* connect_to_database() {
    PGconn *conn = PQconnectdb(DB_CONN_STRING);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        WSACleanup();
        exit(1);
    }
    return conn;
}

void handle_client(SOCKET client_sock, PGconn *db_conn) {
    char buffer[BUFFER_SIZE];
    int recv_size;

    recv_size = recv(client_sock, buffer, BUFFER_SIZE, 0);
    if (recv_size <= 0) {
        printf("Client déconnecté\n");
        closesocket(client_sock);
        return;
    }

    buffer[recv_size] = '\0';
    printf("Message reçu: %s\n", buffer);

    /* Traitement des commandes de base */
    if (strncmp(buffer, "/quit", 5) == 0) {
        send(client_sock, "Déconnexion...\n", 15, 0);
        closesocket(client_sock);
        return;
    }

    /* Exemple: Enregistrement du message en base */
    const char *query = "INSERT INTO messages(content) VALUES($1)";
    const char *params[1] = {buffer};
    PGresult *res = PQexecParams(db_conn, query, 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur DB: %s\n", PQerrorMessage(db_conn));
    } else {
        send(client_sock, "Message enregistré\n", 19, 0);
    }
    PQclear(res);
}

void cleanup(SOCKET server_sock, PGconn *db_conn) {
    closesocket(server_sock);
    PQfinish(db_conn);
    WSACleanup();
}