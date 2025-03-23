#ifndef FONCTIONS_H
#define FONCTIONS_H

extern char * schema;
extern char * table;

extern void help();

extern int list_schema(PGconn *conn);

extern void schema_choice(char * line);

extern int list_table(PGconn* conn);

extern void table_choice(char * line);

extern void view_table(char * request);

#endif
