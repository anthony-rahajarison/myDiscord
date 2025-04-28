#include <string.h>
#include <stdio.h>
#include "user.h"

Role get_role_from_string(const char *role_str) {
    if (strcmp(role_str, "member") == 0) {
        return ROLE_MEMBER;
    } else if (strcmp(role_str, "moderator") == 0) {
        return ROLE_MODERATOR;
    } else if (strcmp(role_str, "admin") == 0) {
        return ROLE_ADMIN;
    } else {
        printf("Erreur : rôle inconnu '%s'\n", role_str);
        return ROLE_MEMBER; 
    }
}
