#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>


extern void exitOnError(PGconn *conn);

void createDatabase(PGconn *conn) {
    PGresult *res = PQexec(conn, "CREATE DATABASE myDiscord");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur lors de la création de la base : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }
    PQclear(res);
}

void createRolesTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS roles ("
        "id SERIAL PRIMARY KEY, "
        "name VARCHAR(20) UNIQUE NOT NULL"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table roles : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `roles` créée\n");
}

void createUserTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "username VARCHAR(20) NOT NULL, "
        "email VARCHAR(100) NOT NULL UNIQUE, "
        "password VARCHAR(255) NOT NULL, "
        "salt VARCHAR(255) NOT NULL, "
        "role_id INTEGER REFERENCES roles(id)"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table users : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `users` créée\n");
}

void createChannelsTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS channels ("
        "id SERIAL PRIMARY KEY, "
        "name VARCHAR(100) NOT NULL UNIQUE, "
        "is_private BOOLEAN DEFAULT FALSE, "
        "owner_id INTEGER REFERENCES users(id)"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table channels : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `channels` créée\n");
}

void createChannelMembersTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS channel_members ("
        "id SERIAL PRIMARY KEY, "
        "user_id INTEGER REFERENCES users(id) ON DELETE CASCADE, "
        "channel_id INTEGER REFERENCES channels(id) ON DELETE CASCADE"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table channel_members : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `channel_members` créée\n");
}

void createMessagesTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS messages ("
        "id SERIAL PRIMARY KEY, "
        "channel_id INTEGER REFERENCES channels(id) ON DELETE CASCADE, "
        "sender_id INTEGER REFERENCES users(id), "
        "content TEXT NOT NULL, "
        "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "is_private BOOLEAN DEFAULT FALSE, "
        "encrypted BOOLEAN DEFAULT FALSE"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table messages : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }
    PQclear(res);
    printf("✅ Table `messages` créée avec succès !\n");
}


void createReactionsTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS reactions ("
        "id SERIAL PRIMARY KEY, "
        "message_id INTEGER REFERENCES messages(id) ON DELETE CASCADE, "
        "user_id INTEGER REFERENCES users(id), "
        "emoji VARCHAR(10)"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table reactions : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `reactions` créée\n");
}

void createPrivateMessagesTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS private_messages ("
        "id SERIAL PRIMARY KEY, "
        "sender_id INTEGER REFERENCES users(id), "
        "receiver_id INTEGER REFERENCES users(id), "
        "content TEXT NOT NULL, "
        "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
        "encrypted BOOLEAN DEFAULT TRUE"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur table private_messages : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `private_messages` créée\n");
}
