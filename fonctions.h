#ifndef FONCTIONS_H
#define FONCTIONS_H

extern char * schema;
extern char * table;

extern void help();

extern char* list_schema();

extern void schema_choice(char * line);

extern void list_table(char * request);

extern void table_choice(char * line);

extern void view_table(char * request);

#endif
