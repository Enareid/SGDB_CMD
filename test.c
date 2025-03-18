#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "fonctions.h"

#define MAX_INPUT 100

void 
get_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "Erreur de lecture.\n");
        exit(EXIT_FAILURE);
    }
    buffer[strcspn(buffer, "\n")] = 0;  // Supprime le retour à la ligne
}

int 
main() {
    char host[MAX_INPUT], dbname[MAX_INPUT], user[MAX_INPUT], password[MAX_INPUT];

    get_input("Hôte : ", host, MAX_INPUT);
    get_input("Nom de la base de données : ", dbname, MAX_INPUT);
    get_input("Utilisateur : ", user, MAX_INPUT);
    get_input("Mot de passe : ", password, MAX_INPUT);

    char conninfo[4 * MAX_INPUT + 30];  // Buffer pour la chaîne de connexion
    snprintf(conninfo, sizeof(conninfo), "host=%s dbname=%s user=%s password=%s", host, dbname, user, password);

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Erreur de connexion : %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    printf("Connexion réussie à la base de données !\n");

    while (1) {
        printf("(SGDB) %s : ", schema);
        char cmd[256];
        fgets(cmd, sizeof(cmd), stdin);
        if (strcmp(cmd, "help\n") == 0){
            help();
        }
        if (strcmp(cmd, "list schema\n") == 0) {
            char * exec = list_schema();
            PGresult *res = PQexec(conn, exec);
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                fprintf(stderr, "Requête échouée : %s\n", PQerrorMessage(conn));
                PQclear(res);
                continue;
            }
            int n = PQntuples(res);
            for (int i = 0; i < n; i++) {
                printf("%s\n", PQgetvalue(res, i, 0));
            }
            PQclear(res);
        }
        if (strncmp(cmd, "go ", 3) == 0) {
            go(cmd);
        }
        if (strcmp(cmd, "list table\n") == 0){
            char buffer[200];
            list_table(buffer);
            PGresult *res = PQexec(conn, buffer);
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                fprintf(stderr, "Requête échouée : %s\n", PQerrorMessage(conn));
                PQclear(res);
                continue;
            }
            int n = PQntuples(res);
            for (int i = 0; i < n; i++) {
                printf("%s\n", PQgetvalue(res, i, 0));
            }
            PQclear(res);
        }
        if (strcmp(cmd, "exit\n") == 0) {
            free(schema);
            break;
        }
    }

    PQfinish(conn);
    return EXIT_SUCCESS;
}
