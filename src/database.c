#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void exitOnError(PGconn *conn) {
    fprintf(stderr, "Erreur PostgreSQL : %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
}

void createdatabase(PGconn *conn){
    PGresult *res = PQexec(conn, "CREATE DATABASE myDiscord");
}

void createUserTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "username VARCHAR(20) NOT NULL,"
        "email VARCHAR(100) NOT NULL UNIQUE, "
        "password VARCHAR(255) NOT NULL, "
        "role TEXT NOT NULL"
        "salt VARCHAR(255) NOT NULL"
        ");";

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur lors de la création de la table : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `users` créée avec succès !\n");
}



void createtablemessages(PGconn *conn) {
    const char *query = 
        "CREATE TABLE IF NOT EXISTS messages ("
        "user_id INTEGER REFERENCES users(id) ON DELETE CASCADE, "
        "content TEXT NOT NULL, "
        "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");";
        
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Erreur lors de la création de la table messages : %s\n", PQerrorMessage(conn));
        PQclear(res);
        exitOnError(conn);
    }

    PQclear(res);
    printf("✅ Table `messages` créée avec succès !\n");
}

