#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>

#define SERVER_IP "127.0.0.1"  ///< Adresse IP par défaut du serveur (localhost)
#define PORT 8080              ///< Port de communication par défaut
#define BUFFER_SIZE 1024       ///< Taille maximale des messages


void init_winsock(void);

/*Crée et configure un socket client*/

SOCKET create_socket(void);

/*Établit la connexion au serveur */

void connect_to_server(SOCKET sock, struct sockaddr_in *server);


void chat(SOCKET sock);

#endif /* CLIENT_H */