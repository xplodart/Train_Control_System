#include "database_management.h"
#include <iostream>

using namespace std;

sqlite3* db = nullptr;


// SQLite database function
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i < argc; i++) {
        cout << azColName[i] << " : " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

// Function to execute SQL commands
void executeSQL(const char* sql) {
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
    if(rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}

// Function to open the database connection
void openDatabaseConnection() {
    int rc = sqlite3_open("trains_control.db", &db);
    if(rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }
}

// Function to close the database connection
void closeDatabaseConnection() {
    sqlite3_close(db);
}
// Function to create the tables
void createTables() {
    const char *sql = "CREATE TABLE IF NOT EXISTS trains ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "trainnumber INT NOT NULL,"
                      "start TEXT NOT NULL,"
                      "destination TEXT NOT NULL,"
                      "status TEXT NOT NULL,"
                      "distance INT,"
                      "stops INT);";
    executeSQL(sql);
}