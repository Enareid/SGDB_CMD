#include <stdio.h>
#include <string.h>
#include "fonctions.h"

char * schema = "";

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
go()
{
    schema[strcspn(schema, "\n")] = 0;
    printf("%s\n", schema);
}