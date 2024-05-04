// h-fil och c-fil tillsammans från början

#include <string>
#include <iostream>
#include <thread> // for sleep function
#include <chrono> // for sleep function
// #include <cctype> // för std::toupper
#include "gamelog.h"

#define DEFAULT_DELAY 250

void menuSleep(int ms = DEFAULT_DELAY) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void printMenu() {
    std::cout << "1. Print list" << std::endl;
    std::cout << "2. Add game" << std::endl;
    std::cout << "3. Remove game" << std::endl;
    std::cout << "4. Load list" << std::endl;
    std::cout << "5. Save list" << std::endl;
    std::cout << "6. Clear list" << std::endl;
    std::cout << "7. Print filtered" << std::endl;
    std::cout << "8. Quit" << std::endl;
}

void printFilterMenu() {
    std::cout << "1. Print books" << std::endl;
    std::cout << "2. Print games" << std::endl;
    std::cout << "3. Print switch games" << std::endl;
    std::cout << "4. Print PS4 games" << std::endl;
}


int main(int argc, char const *argv[]) {
    LLGames defaultList;

    defaultList.loadFromFile();

    std::string mainInput;
    int menuInput;
    std::string nameInput, platformInput, dateInput;
    int filter = 0; // for filtering print list


    bool running = true;

    while(running) {
        menuSleep();
        std::cout << std::endl;
        printMenu();
        std::cout << std::endl << "input: ";
        std::getline(std::cin, mainInput);
        menuSleep();
        try {
            menuInput = std::stoi(mainInput);
            mainInput.clear();
        } catch(std::invalid_argument& error) {
            std::cout << "Input could not be parsed. Try again." << std::endl;
            continue;
        }
        std::cout << std::endl;
        switch (menuInput) {
            case 1:
                // print list
                if(defaultList.getSize() < 1) {
                    std::cout << "list is empty." << std::endl;
                } else {
                    defaultList.printList();
                }
                break;
            case 2:
                // add game
                std::cout << "Name: ";
                std::getline(std::cin, nameInput);
                menuSleep();
                std::cout << "Platform: ";
                std::getline(std::cin, platformInput);
                menuSleep();
                std::cout << "Date (YYYYMMDD or YYYY-MM-DD): ";
                std::getline(std::cin, dateInput);
                defaultList.insertGame(nameInput, platformInput, dateInput);
                break;
            case 3:
                // remove game
                defaultList.printList(true);
                menuSleep();
                std::cout << "Which game do you want to remove?" << std::endl << "Enter index: ";
                std::getline(std::cin, mainInput);
                try {
                    menuInput = std::stoi(mainInput);
                } catch(std::invalid_argument& error) {
                    std::cout << "Invalid index. Operation cancelled." << std::endl;
                    menuSleep();
                    continue;
                }
                if (menuInput < 1 || menuInput > defaultList.getSize()) {
                    std::cout << "Index out of range." << std::endl;
                    menuSleep();
                    continue;
                }
                std::cout << "You've removed: " << defaultList.removeGame(menuInput) << std::endl;
                break;
            case 4:
                // load list
                defaultList.clearList();
                std::cout << "Enter filename (ENTER to load default list): ";
                mainInput.clear();
                std::getline(std::cin, mainInput);
                if(mainInput.empty()) {
                    defaultList.loadFromFile();
                } else {
                    defaultList.loadFromFile(mainInput);
                }
                break;
            case 5:
                // save list
                std::cout << "Enter filename (ENTER to save to default list): ";
                mainInput.clear();
                std::getline(std::cin, mainInput);
                if(mainInput.empty()) {
                    defaultList.printToFile();
                } else {
                    defaultList.printToFile(mainInput);
                }
                break;
            case 6:
                // clear list
                std::cout << "Are you SURE you want to clear list? (y/n) ";
                std::getline(std::cin, mainInput);
                if(mainInput == "y" || mainInput == "Y") {
                    int listSize = defaultList.getSize();
                    for(int i = 0; i < listSize; i++) {
                        std::cout << "Removed game: " << defaultList.removeGame(1) << std::endl;
                    }
                } else {
                    menuSleep(100);
                    std::cout << "Deletion aborted." << std::endl;
                }
                break;
            case 7:
                // print filtered
                printFilterMenu();
                std::cout << std::endl << "input: ";
                std::getline(std::cin, mainInput);
                try {
                    menuInput = std::stoi(mainInput);
                    mainInput.clear();
                } catch(std::invalid_argument& error) {
                    std::cout << "Input could not be parsed. Try again." << std::endl;
                    continue;
                }
                if (menuInput == 1)
                {
                    filter = BOOKS;
                } else if (menuInput == 2) {
                    filter = GAMES;
                } else if (menuInput == 3) {
                    filter = SWITCH;
                } else if (menuInput == 4) {
                    filter = PS4;
                }                
                else {
                    std::cout << "wrong input" << std::endl;
                }
                
                defaultList.printFiltered(filter);
                break;
            case 8:
                // quit
                std::cout << "Would you like to save list? (y/n) ";
                std::getline(std::cin, mainInput);
                if(mainInput == "y" || mainInput == "Y") {
                    defaultList.printToFile();
                    menuSleep();
                    std::cout << "List saved. Thanks and goodbye." << std::endl;
                } else {
                    menuSleep(100);
                    std::cout << "Thanks and goodbye. List not saved." << std::endl;
                }
                running = false;
                break;
            default:
                std::cout << "What was THAT?\n";
        }
    }


    return 0;
}

// int main(int argc, char const *argv[]) {
//     std::string testStr = "Löpa varg";
//     std::cout << std::to_string(lengthOfName(testStr)) << std::endl;
//     std::cout << std::endl;
//     for(int i = 0; i < testStr.length(); i++) {
//         std::cout << "char: " << testStr[i] << ", int: " << (int) testStr[i] << ", less than zero: " << std::to_string(testStr[i] < 0) << std::endl;
//     }
//     return 0;
// }
