#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include "sqlite3.h"

extern sqlite3* db;

// SQLite database function
int callback(void *NotUsed, int argc, char **argv, char **azColName);

// Function to execute SQL commands
void executeSQL(const char* sql);

// Function to open the database connection
void openDatabaseConnection();

// Function to close the database connection
void closeDatabaseConnection();

// Function to create the tables
void createTables();

#endif // DATABASE_MANAGEMENT_H