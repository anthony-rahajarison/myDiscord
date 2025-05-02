#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <string.h>


typedef enum {
    ROLE_MEMBER,     
    ROLE_MODERATOR,  
    ROLE_ADMIN       
} Role;


typedef struct {
    int socket;      
    char username[50]; 
    Role role;       
} User;

/* Traite une commande reçue d'un utilisateur */
void handle_command(User *user, const char *message);

/* Crée un nouveau salon */
void create_channel(const char *name);

/* Bannit un utilisateur */
void ban_user(const char *username);

/* Supprime un salon existant*/
void delete_channel(const char *channel);

#endif 