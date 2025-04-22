#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void exitOnError(PGconn *conn) {
    fprintf(stderr, "Erreur PostgreSQL : %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
}


void createUserTable(PGconn *conn) {
    const char *query =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "fname VARCHAR(50) NOT NULL, "
        "email VARCHAR(100) NOT NULL UNIQUE, "
        "password VARCHAR(255) NOT NULL, "
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
