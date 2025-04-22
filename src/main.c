#include <stdio.h>
#include <libpq-fe.h>

void createUserTable(PGconn *conn);
void exitOnError(PGconn *conn);

int main() {
    const char *conninfo = "host=localhost dbname=myDiscord user=postgres password=Benjmax13011.";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        exitOnError(conn);
    }

    printf("✅ Connexion à PostgreSQL réussie !\n");

    createUserTable(conn);

    PQfinish(conn);
    return 0;
}
