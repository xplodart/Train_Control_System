#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <algorithm>
#include "sqlite3.h"

void displayTrainDetails(int trainNumber);
void displayAllTrains();
bool isValidStation(const std::string& station);
int calculateDistance(sqlite3* db, const std::string& start, const std::string& destination);
int calculateStops(const std::string& start, const std::string& destination);

#endif