#include "menu.h"
#include <iostream>
#include "trains_management.h"

void displayMenu() {
    int choice;
    do {
        std::cout << std::endl;
        std::cout << "==============  MENU:  ==============" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "1. Start a train" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "2. Stop a train" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "3. Add a new train" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "4. Remove a train" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "5. Change start of a train" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "6. Change destination of a train" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                startTrain();
                break;
            case 2:
                stopTrain();
                break;
            case 3:
                addNewTrain();
                break;
            case 4:
                removeTrain();
                break;
            case 5:
                updateStartStation();
                break;
            case 6:
                updateDestination();
                break;
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    } while (choice != 0);
}
