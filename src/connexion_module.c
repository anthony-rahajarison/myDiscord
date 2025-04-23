#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libpq-fe.h>
#include <openssl/sha.h>

#define DB_CONN "host=localhost dbname=mydiscord user=postgres password=yourpassword"


static PGconn* connect_db() {
    PGconn* conn = PQconnectdb(DB_CONN);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }
    return conn;
}


void hash_password(const char* password, char* hashed_output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)password, strlen(password), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed_output + (i * 2), "%02x", hash[i]);
    }
    hashed_output[64] = '\0';
}


int validate_password(const char *pwd) {
    int length = strlen(pwd);
    int has_upper = 0, has_digit = 0, has_special = 0;

    if (length < 8) {
        printf("Le mot de passe doit contenir au moins 8 caractères.\n");
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (isupper(pwd[i])) has_upper = 1;
        if (isdigit(pwd[i])) has_digit = 1;
        if (!isalnum(pwd[i])) has_special = 1;
    }

    if (!has_upper) {
        printf("Le mot de passe doit contenir au moins une majuscule, un chiffre et un caractère spécial.\n");
        return 0;
    }

    if (!has_digit) {
        printf("Le mot de passe doit contenir au moins un chiffre.\n");
        return 0;
    }

    if (!has_special) {
        printf("Le mot de passe doit contenir au moins un caractère spécial.\n");
        return 0;
    }

    return 1;
}


int register_user(const char* username, const char* password) {
    if (!validate_password(password)) {
        printf("Mot de passe non conforme aux règles de sécurité.\n");
        return 0;
    }

    PGconn* conn = connect_db();
    if (!conn) return 0;

    char hashed_password[65];
    hash_password(password, hashed_password);

    const char* params[2] = { username, hashed_password };
    PGresult* res = PQexecParams(conn,
        "INSERT INTO users (username, password) VALUES ($1, $2)",
        2, NULL, params, NULL, NULL, 0);

    int success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    PQfinish(conn);
    return success;
}


int login_user(const char* username, const char* password) {
    PGconn* conn = connect_db();
    if (!conn) return 0;

    char hashed_password[65];
    hash_password(password, hashed_password);

    const char* params[2] = { username, hashed_password };
    PGresult* res = PQexecParams(conn,
        "SELECT id FROM users WHERE username = $1 AND password = $2",
        2, NULL, params, NULL, NULL, 0);

    int found = PQntuples(res) > 0;
    PQclear(res);
    PQfinish(conn);
    return found;
}
