#ifndef TRAINS_MANAGEMENT_H
#define TRAINS_MANAGEMENT_H

#include <iostream>
#include <string>
#include <limits>
#include "utilities.h"
#include "database_management.h"

void startTrain();
void stopTrain();
void addNewTrain();
void removeTrain();
void updateStartStation();
void updateDestination();

#endif
