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

void affichage(char * schema,char * table){
    printf("(SGDB) ");
    if(strcmp(schema,"")!=0){
        printf("%s> ",schema);
    }
    if(strcmp(table,"")!=0){
        printf("%s> ",table);
    }
}

int 
read_connection_file(const char *filename, char *host, char *dbname, char *user, char *password) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strncmp(line, "host=", 5) == 0)
            strcpy(host, line + 5);
        else if (strncmp(line, "dbname=", 7) == 0)
            strcpy(dbname, line + 7);
        else if (strncmp(line, "user=", 5) == 0)
            strcpy(user, line + 5);
        else if (strncmp(line, "password=", 9) == 0)
            strcpy(password, line + 9);
    }

    fclose(file);
    return 1;
}



int 
main(int argc, char * argv[]) {
    int r;
    char host[MAX_INPUT] = "";
    char dbname[MAX_INPUT] = "";
    char user[MAX_INPUT] = "";
    char password[MAX_INPUT] = "";

    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        r = read_connection_file(argv[2], host, dbname, user, password);
        if (!r) {
            return r;
        }
    } else if (argc == 1) {
        get_input("Hôte : ", host, MAX_INPUT);
        get_input("Nom de la base de données : ", dbname, MAX_INPUT);
        get_input("Utilisateur : ", user, MAX_INPUT);
        get_input("Mot de passe : ", password, MAX_INPUT);
    } else {
        fprintf(stderr, "Usage: %s [-f filename]\n", argv[0]);
        return EXIT_FAILURE;
    }

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
        affichage(schema,table);
        char cmd[256];
        fgets(cmd, sizeof(cmd), stdin);
        if (strcmp(cmd, "help\n") == 0){
            help();
        }
        if (strcmp(cmd, "list schema\n") == 0) {
            r = list_schema(conn);
        }
        if (strncmp(cmd, "schema ", 7) == 0) {
            schema_choice(cmd);
        }
        if (strncmp(cmd, "table ", 6) == 0) {
            table_choice(cmd);
        }
        if (strcmp(cmd, "view\n") == 0) {
            char buffer[200];
            view_table(buffer);
            PGresult *res = PQexec(conn, buffer);
            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                fprintf(stderr, "Requête échouée : %s\n", PQerrorMessage(conn));
                PQclear(res);
                continue;
            }
            int rows = PQntuples(res);
            int cols = PQnfields(res);
            for (int i = 0; i < cols; i++) {
                printf("%s\t", PQfname(res, i));
            }
            printf("\n");
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    printf("%s\t", PQgetvalue(res, i, j));
                }
                printf("\n");
            }
            PQclear(res);
        }
        if (strcmp(cmd, "list table\n") == 0){
            r = list_table(conn);
        }
        if (strcmp(cmd, "exit\n") == 0) {
            free(schema);
            break;
        }
    }

    PQfinish(conn);
    return EXIT_SUCCESS;
}
