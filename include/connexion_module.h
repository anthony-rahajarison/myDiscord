#ifndef CONNEXION_MODULE_H
#define CONNEXION_MODULE_H

#include <libpq-fe.h>
#include <stddef.h>

/*Établit une connexion à la base de données
 */
PGconn* connect_db(void);

/*Hache un mot de passe (algorithme simple pour l'exemple)*/

void hash_password(const char* password, char* hashed_output);

/*Valide la complexité d'un mot de passe */
int validate_password(const char *pwd);

/* Enregistre un nouvel utilisateur*/
int register_user(const char* username, const char* password);

/* Authentifie un utilisateur */
int login_user(const char* username, const char* password);

#endif 