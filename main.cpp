#include <iostream>
#include "database_management.h"
#include "utilities.h"
#include "trains_management.h"
#include "menu.h"


int main() {
    openDatabaseConnection();
    createTables();
    displayAllTrains();
    displayMenu();

 closeDatabaseConnection();
    return 0;
}