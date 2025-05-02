#ifndef USER_H
#define USER_H

typedef enum {
    ROLE_MEMBER,
    ROLE_MODERATOR,
    ROLE_ADMIN
} Role;

Role get_role_from_string(const char *role_str);

#endif /* USER_H */