#include <stdio.h>
#include "user.h"

int main() {

    const char *role_str = "moderator";  
    

    Role role = get_role_from_string(role_str);
    

    switch(role) {
        case ROLE_MEMBER:
            printf("Rôle : Membre\n");
            break;
        case ROLE_MODERATOR:
            printf("Rôle : Modérateur\n");
            break;
        case ROLE_ADMIN:
            printf("Rôle : Administrateur\n");
            break;
        default:
            printf("Rôle inconnu\n");
    }

    return 0;
}
