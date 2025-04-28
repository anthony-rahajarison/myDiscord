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

void handle_command(User *user, const char *message);
void create_channel(const char *name);
void ban_user(const char *username);
void delete_channel(const char *channel);

void handle_command(User *user, const char *message) {
    if (strncmp(message, "/create ", 8) == 0) {
        if (user->role == ROLE_MODERATOR || user->role == ROLE_ADMIN) {
            const char *channel_name = message + 8;
            create_channel(channel_name);
        } else {
            printf("%s n'a pas le droit de créer un salon.\n", user->username);
        }
    } else if (strncmp(message, "/ban ", 5) == 0) {
        if (user->role == ROLE_MODERATOR || user->role == ROLE_ADMIN) {
            const char *username = message + 5;
            ban_user(username);
        } else {
            printf("%s n'a pas le droit de bannir un utilisateur.\n", user->username);
        }
    } else if (strncmp(message , "/delete ", 7) == 0) {
        if (user->role == ROLE_ADMIN) { 
            const char *channel_name = message + 7;
            delete_channel(channel_name);
        } else {
            printf("%s n'a pas le droit de supprimer un salon.\n", user->username);
        }
    } else {
        printf("Commande inconnue : %s\n", message);
    }
}


void create_channel(const char *name) {
    printf("Salon créé : %s\n", name);
}

void ban_user(const char *username) {
    printf("Utilisateur banni : %s\n", username);
}


void delete_channel(const char *channel) {
    printf("Salon supprimé : %s\n", channel);
}
