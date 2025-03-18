#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fonctions.h"

char * schema = "";
char * table = "";

void
help()
{
    printf("help -- Affiche les commandes disponibles\n");
}

char *
list_schema()
{
    char * string = "SELECT schema_name FROM information_schema.schemata;";
    return string;
}

void
list_table(char * request)
{
    sprintf(request, "SELECT table_name FROM information_schema.tables WHERE table_schema = '%s';", schema);
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