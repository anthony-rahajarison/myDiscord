#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <libpq-fe.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

/* Initialisation de WinSock */
void init_winsock(void);

/* Création d'une socket */
SOCKET create_socket(void);

/* Configuration du serveur */
void bind_server(SOCKET sock, struct sockaddr_in *server);

/* Gestion de la base de données */
void save_message_to_db(PGconn *conn, int channel_id, int sender_id, const char *message);
void send_past_messages(SOCKET client_sock, PGconn *conn, int channel_id);

/* Gestion des connexions clients */
void handle_chat(SOCKET client_sock, PGconn *conn, int channel_id);

/* Fonction principale du serveur */
void start_chat_server(void);

#endif /* SERVER_H */