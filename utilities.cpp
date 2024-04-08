#include "utilities.h"
#include "database_management.h"
using namespace std;

void displayTrainDetails(int trainNumber) {
    // Retrieve and display the details of the train with the given train number
    std::string selectQuery = "SELECT * FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            cout << "Train Details:" << endl;
            cout << "Train Number: " << sqlite3_column_int(statement, 1) << endl;
            cout << "Start Station: " << sqlite3_column_text(statement, 2) << endl;
            cout << "Destination Station: " << sqlite3_column_text(statement, 3) << endl;
            cout << "Distance: " << sqlite3_column_int(statement, 5) << " km" << endl;
            cout << "Stops: " << sqlite3_column_int(statement, 6) << endl;
            cout << "Status: " << sqlite3_column_text(statement, 4) << endl;
        } else {
            std::cerr << "Train with train number " << trainNumber << " not found." << std::endl;
        }
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(statement);
}

void displayAllTrains() {
    sqlite3_stmt* statement;
    std::string selectQuery = "SELECT * FROM trains;";

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        std::cout << "/////////////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
        std::cout << std::endl;
        std::cout << "=============================================================================================" << std::endl;
        std::cout << "|                                 Train Control & Management                                |" << std::endl;
        std::cout << "+------------++--------------------+--------------------+------------+--------++------------+" << std::endl;
        std::cout << "| Train No.  ||       Start        |     Destination    |  Distance  |  Stops ||   Status   |" << std::endl;
        std::cout << "+------------++--------------------+--------------------+------------+--------++------------+" << std::endl;
        while (sqlite3_step(statement) == SQLITE_ROW) {
            int trainNumber = sqlite3_column_int(statement, 1);
            const unsigned char* start = sqlite3_column_text(statement, 2);
            const unsigned char* destination = sqlite3_column_text(statement, 3);
            const unsigned char* status = sqlite3_column_text(statement, 4);
            int distance = sqlite3_column_int(statement, 5); 
            int stops = sqlite3_column_int(statement, 6); 
            std::cout << "| " << trainNumber << "        || " << start << " station"<< "          | " << destination << " station"<< "          | " << distance << " km" << "      | " << stops << "      || "  << status << "    |"  <<std::endl;
        }
        std::cout << "+------------++--------------------+--------------------+------------+--------++------------+" << std::endl;
        std::cout << std::endl;
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(statement);
}



// Validate station input
bool isValidStation(const std::string& station) {
    return station.size() == 1 && station[0] >= 'A' && station[0] <= 'E';
}

// Calculate Distance
int calculateDistance(sqlite3* db, const std::string& start, const std::string& destination) {
    if ((start == "A" && destination == "B") || (start == "B" && destination == "A")) return 10;
    else if ((start == "A" && destination == "C") || (start == "C" && destination == "A")) return 20;
    else if ((start == "A" && destination == "D") || (start == "D" && destination == "A")) return 30;
    else if ((start == "A" && destination == "E") || (start == "E" && destination == "A")) return 40;
    else if ((start == "B" && destination == "C") || (start == "C" && destination == "B")) return 10;
    else if ((start == "B" && destination == "D") || (start == "D" && destination == "B")) return 20;
    else if ((start == "B" && destination == "E") || (start == "E" && destination == "B")) return 30;
    else if ((start == "C" && destination == "D") || (start == "D" && destination == "C")) return 10;
    else if ((start == "C" && destination == "E") || (start == "E" && destination == "C")) return 20;
    else if ((start == "D" && destination == "E") || (start == "E" && destination == "D")) return 10;
    else return 0;
}


// Calculate the number of stops between start and destination stations
int calculateStops(const std::string& start, const std::string& destination) {
    // Order start and destination stations alphabetically
    std::string orderedStations = start + destination;
    std::sort(orderedStations.begin(), orderedStations.end());
    
    // Calculate the number of stops between start and destination
    int startIndex = orderedStations[0] - 'A';
    int destIndex = orderedStations[1] - 'A';
    
    // Excluding start and destination
    return std::max(destIndex - startIndex - 1, 0);
}