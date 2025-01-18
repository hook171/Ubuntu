#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include "sqlite3.h"

extern sqlite3* db;

void init_db();
void write_temperature(float temp);
void close_db();

#endif // DB_HANDLER_H