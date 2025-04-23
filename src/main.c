#include <stdio.h>
#include <libpq-fe.h>

void createUserTable(PGconn *conn);
void exitOnError(PGconn *conn);
void handle_command(int client_socket, const char *message);

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

    handle_command(1, "/create général");
    handle_command(2, "/ban bob");
    handle_command(3, "/join musique");
    handle_command(4, "/fly");  

    return 0;
}
