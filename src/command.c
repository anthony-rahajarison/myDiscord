#include <stdio.h>
#include <string.h>


void handle_command(int client_socket, const char *message);
void create_channel(const char *name);
void ban_user(const char *username);
void join_channel(int client_socket, const char *channel);

void handle_command(int client_socket, const char *message) {
    if (strncmp(message, "/create ", 8) == 0) {
        const char *channel_name = message + 8;
        create_channel(channel_name);

    } else if (strncmp(message, "/ban ", 5) == 0) {
        const char *username = message + 5;
        ban_user(username);

    } else if (strncmp(message, "/join ", 6) == 0) {
        const char *channel = message + 6;
        join_channel(client_socket, channel);
        
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

void join_channel(int client_socket, const char *channel) {
    printf("Client %d rejoint le salon : %s\n", client_socket, channel);
}
