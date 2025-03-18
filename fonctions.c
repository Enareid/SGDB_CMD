#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include "fonctions.h"

char * schema = "";
char * table = "";

void
help()
{
    printf("help -- Affiche les commandes disponibles\n");
}

int
list_schema(PGconn* conn)
{
    char * string = "SELECT schema_name FROM information_schema.schemata;";
    PGresult *res = PQexec(conn, string);
    if (PQresultStatus(res) == PGRES_EMPTY_QUERY) {
        fprintf(stderr, "Requête échouée : %s\n", PQerrorMessage(conn));
        PQclear(res);
        return EXIT_FAILURE;
    }
    int n = PQntuples(res);
    if(n==0){
        printf("Erreur dans la requete\n");
        PQclear(res);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        printf("%s\n", PQgetvalue(res, i, 0));
    }
    PQclear(res);
    return EXIT_SUCCESS;
}

int
list_table(PGconn* conn)
{
    char buffer[200];
    sprintf(buffer, "SELECT table_name FROM information_schema.tables WHERE table_schema = '%s';", schema);
    PGresult *res = PQexec(conn, buffer);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Requête échouée : %s\n", PQerrorMessage(conn));
        PQclear(res);
        return EXIT_FAILURE;
    }
    int n = PQntuples(res);
    if(n==0){
        printf("Erreur dans la requete\n");
        PQclear(res);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        printf("%s\n", PQgetvalue(res, i, 0));
    }
    PQclear(res);
    return EXIT_SUCCESS;
}

void
schema_choice(char * line)
{
    const char * separator = " ";
    char * test  = strtok(line, separator);
    test = strtok(NULL, separator);
    schema = malloc(strlen(test) + 1);
    strcpy(schema, test);
    schema[strlen(test) - 1] = '\0';
}

void
table_choice(char * line)
{
    const char * separator = " ";
    char * test  = strtok(line, separator);
    test = strtok(NULL, separator);
    table = malloc(strlen(test) + 1);
    strcpy(table, test);
    table[strlen(test) - 1] = '\0';
}

void
view_table(char * request)
{
    sprintf(request, "SELECT * FROM %s;", table);
}