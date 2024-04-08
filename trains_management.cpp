#include "trains_management.h"

using namespace std;

void startTrain() {
    int trainNumber;

    cout << "Enter Train Number: ";
    cin >> trainNumber;
    cin.ignore();

    displayTrainDetails(trainNumber);

    // Check if the train exists and its status is not "Running"
    std::string selectQuery = "SELECT status FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            const unsigned char* currentStatus = sqlite3_column_text(statement, 0);
            std::string status = std::string(reinterpret_cast<const char*>(currentStatus));

            if (status != "Running") {
                char confirmation;
                cout << "Do you want to start train number " << trainNumber << " (y/n)? ";
                cin >> confirmation;
                cin.ignore();

                confirmation = tolower(confirmation);

                // Update the status of the train to "Running"
                if (confirmation == 'y') {
                    std::string updateQuery = "UPDATE trains SET status = 'Running' WHERE trainnumber = " + std::to_string(trainNumber) + ";";
                    executeSQL(updateQuery.c_str());
                     displayAllTrains();
                    cout << "Train number " << trainNumber << " has been started." << endl;
                } else {
                    displayAllTrains();
                    cout << "Operation cancelled." << endl;
                }
            } else {
                displayAllTrains();
                cout << "The train is already running." << endl;
            }
        } else {
            displayAllTrains();
            std::cerr << "Train with train number " << trainNumber << " not found." << std::endl;
        }
    } else {
        displayAllTrains();
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(statement);
}

void stopTrain() {
    int trainNumber;

    cout << "Enter Train Number: ";
    cin >> trainNumber;
    cin.ignore();

    displayTrainDetails(trainNumber);

    // Check if the train exists and its status is "Running"
    std::string selectQuery = "SELECT status FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            const unsigned char* currentStatus = sqlite3_column_text(statement, 0);
            std::string status = std::string(reinterpret_cast<const char*>(currentStatus));

            if (status == "Running") {
                char confirmation;
                cout << "Do you want to stop train number " << trainNumber << " (y/n)? ";
                cin >> confirmation;
                cin.ignore();

                confirmation = tolower(confirmation);

                // Update the status of the train to "Stopped"
                if (confirmation == 'y') {
                    std::string updateQuery = "UPDATE trains SET status = 'Stopped' WHERE trainnumber = " + std::to_string(trainNumber) + ";";
                    executeSQL(updateQuery.c_str());
                     displayAllTrains();
                    cout << "Train number " << trainNumber << " has been stopped." << endl;
                } else {
                    displayAllTrains();
                    cout << "Operation cancelled." << endl;
                }
            } else {
                displayAllTrains();
                cout << "The train is already stopped." << endl;
            }
        } else {
            displayAllTrains();
            std::cerr << "Train with train number " << trainNumber << " not found." << std::endl;
        }
    } else {
        displayAllTrains();
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(statement);  
}


// Add a new train
void addNewTrain() {
    int trainNumber;
    string start, destination, status;

       // Validate train number input
    do {
        cout << "Enter Train Number (three digits): ";
        cin >> trainNumber;
        if (cin.fail() || trainNumber < 100 || trainNumber > 999) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid train number. Please enter a three-digit number." << endl;
        }
    } while (trainNumber < 100 || trainNumber > 999);

    cin.ignore();

 do {
        cout << "Enter Start Station (A-E): ";
        getline(cin, start);
        transform(start.begin(), start.end(), start.begin(), ::toupper);
        if (!isValidStation(start)) {
            cout << "Invalid station. Please enter a station from A to E." << endl;
        }
    } while (!isValidStation(start));

    do {
        cout << "Enter Destination Station (A-E): ";
        getline(cin, destination);
        transform(destination.begin(), destination.end(), destination.begin(), ::toupper);
        if (!isValidStation(destination)) {
            cout << "Invalid station. Please enter a station from A to E." << endl;
        }
    } while (!isValidStation(destination));

    // Check if start and destination are the same
    if (start == destination) {
        displayAllTrains();
        cout << "Start and destination stations cannot be the same. Please try again." << endl;
        return;
    }
    // Ask the user for status (r/R for Running, s/S for Stopped)
    cout << "Enter Status (r/Running, s/Stopped): ";
    char statusChoice;
    cin >> statusChoice;
    cin.ignore();

    if (statusChoice == 'r' || statusChoice == 'R') {
        status = "Running";
    } else if (statusChoice == 's' || statusChoice == 'S') {
        status = "Stopped";
    } else {
        cerr << "Invalid status choice." << endl;
        status = "Stopped";
    }

    // Calculate Distance based on start and destination stations
    int distance = calculateDistance(nullptr, start, destination);

    // Calculate the number of stops between start and destination stations
    int stops = calculateStops(start, destination);

    // Insert the new train into the database
    string sql = "INSERT INTO trains (trainnumber, start, destination, status, Distance, stops) VALUES (" + to_string(trainNumber) + ", '" + start + "', '" + destination + "', '" + status + "', " + to_string(distance) + ", " + to_string(stops) + ");";
    executeSQL(sql.c_str());

    displayAllTrains();
    cout << "Train number " << trainNumber << " was added." << endl;
}

void removeTrain() {
    int trainNumber;

    cout << "Enter Train Number to remove: ";
    cin >> trainNumber;
    cin.ignore();

    displayTrainDetails(trainNumber);

    // Ask for confirmation if the train exists
    std::string selectQuery = "SELECT * FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            char confirm;
            cout << "Are you sure you want to remove this train? (y/n): ";
            cin >> confirm;
            cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                // Execute SQL command to delete the train
                std::string deleteQuery = "DELETE FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";
                executeSQL(deleteQuery.c_str());
                displayAllTrains();
                cout << "Train " << trainNumber << " removed successfully." << endl;
            } else {
                displayAllTrains();
                cout << "Operation canceled. Train " << trainNumber << " not removed." << endl;
            }
        }
    } else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(statement);
}

void updateStartStation() {
    int trainNumber;
    string newStart;

    cout << "Enter Train Number: ";
    cin >> trainNumber;
    cin.ignore();

   do {
        cout << "Enter New Start Station (A-E): ";
        getline(cin, newStart);
        transform(newStart.begin(), newStart.end(), newStart.begin(), ::toupper);
        if (!isValidStation(newStart)) {
            cout << "Invalid station. Please enter a station from A to E." << endl;
        }
    } while (!isValidStation(newStart));

    // Retrieve the current destination station for the given train number
    std::string selectQuery = "SELECT destination FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";

    sqlite3_stmt* statement;

    // Prepare and execute select statement
    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            const unsigned char* currentDestination = sqlite3_column_text(statement, 0);

            // Check if newStart is the same as the current destination
            if (newStart == std::string(reinterpret_cast<const char*>(currentDestination))) {
                displayAllTrains();
                cout << "New start station cannot be the same as the current destination station. Please try again." << endl;
                return;
            }
            
            // Calculate the new Distance based on the new start and current destination stations
            int newDistance = calculateDistance(db, newStart, std::string(reinterpret_cast<const char*>(currentDestination)));
            
            // Calculate the new number of stops based on the new start and current destination stations
            int newStops = calculateStops(newStart, std::string(reinterpret_cast<const char*>(currentDestination)));
            
            // Update the start station, distance, and stops in the database
            std::string updateQuery = "UPDATE trains SET start = '" + newStart + "', distance = " + std::to_string(newDistance) + ", stops = " + std::to_string(newStops) + " WHERE trainnumber = " + std::to_string(trainNumber) + ";";
            executeSQL(updateQuery.c_str());{
                displayAllTrains();
                cout << "Train start station was updated successfully." << endl;
            }
            
        } else {
            displayAllTrains();
            std::cerr << "Train with train number " << trainNumber << " not found." << std::endl;
        }
    } else {
        displayAllTrains();
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(statement);
}

void updateDestination() {
    int trainNumber;
    string newDestination;

    cout << "Enter Train Number: ";
    cin >> trainNumber;
    cin.ignore();

      do {
        cout << "Enter New Destination Station (A-E): ";
        getline(cin, newDestination);
        transform(newDestination.begin(), newDestination.end(), newDestination.begin(), ::toupper);
        if (!isValidStation(newDestination)) {
            cout << "Invalid station. Please enter a station from A to E." << endl;
        }
    } while (!isValidStation(newDestination));

    transform(newDestination.begin(), newDestination.end(), newDestination.begin(), ::toupper);

    // Retrieve the current start station for the given train number
    std::string selectQuery = "SELECT start FROM trains WHERE trainnumber = " + std::to_string(trainNumber) + ";";

    sqlite3_stmt* statement;

    // Prepare and execute select statement
    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            const unsigned char* currentStart = sqlite3_column_text(statement, 0);
            
            // Check if newDestination is the same as the current start station
            if (newDestination == std::string(reinterpret_cast<const char*>(currentStart))) {
                displayAllTrains();
                cout << "New destination station cannot be the same as the current start station. Please try again." << endl;
                return;
            }
            
            // Calculate the new distance based on the current start and new destination stations
            int newDistance = calculateDistance(db, std::string(reinterpret_cast<const char*>(currentStart)), newDestination);
            
            // Calculate the new number of stops based on the current start and new destination stations
            int newStops = calculateStops(std::string(reinterpret_cast<const char*>(currentStart)), newDestination);
            
            // Update the destination station, distance, and stops in the database
            std::string updateQuery = "UPDATE trains SET destination = '" + newDestination + "', distance = " + std::to_string(newDistance) + ", stops = " + std::to_string(newStops) + " WHERE trainnumber = " + std::to_string(trainNumber) + ";";
            executeSQL(updateQuery.c_str());{
                displayAllTrains();
                cout << "Train destination station was updated successfully." << endl;
            }
            
        } else {
            displayAllTrains();
            std::cerr << "Train with train number " << trainNumber << " not found." << std::endl;
        }
    } else {
        displayAllTrains();
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(statement);
}