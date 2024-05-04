
#include <string>
#include <iostream>
#include <fstream>

#include "gamelog.h"

#define PRINT_TABLENGTH 4

const std::string LLGames::TEXTFILE = "spelade_spel.txt";


std::string toUpper(std::string str) {
    std::string uppercaseStr;
    for(char c : str) {
        uppercaseStr.append(1, std::toupper(c));
    }
    return uppercaseStr;
}

std::string getSpacing(int nr) {
    std::string spaceString = "";
    for(int i = 0; i < nr; i++) {
        spaceString += " ";
    }
    return spaceString;
}


std::string LLGames::Node::checkPlatform(std::string platform) {
    std::string replacement = toUpper(platform);
    if(replacement == "PC") {
        return "PC";
    } else if(replacement == "SWITCH") {
        return "SWITCH";
    } else if (replacement == "PS4") {
        return "PS4";
    } else if (replacement == "BOOK") {
        return "BOOK";
    } else {
        return "OTHER";
    }
}

LLGames::Node::Node (std::string name, std::string platform, int date[], Node* next){
    data.name = name;
    data.finishDate.year = date[0];
    data.finishDate.month = date[1];
    data.finishDate.day = date[2];
    data.platform = checkPlatform(platform);
    this->next = next;
}

std::string LLGames::Node::getName() const {return this->data.name;}

std::string LLGames::Node::getPlatform() const {return this->data.platform;}

date_t LLGames::Node::getFinishDate() const {return this->data.finishDate;}

LLGames::Node* LLGames::Node::getNext() const {return next;}

void LLGames::Node::setNext(Node* newNext) {this->next = newNext;}

std::string LLGames::Node::dateToStr() {
    std::string dateAsString = std::to_string(data.finishDate.year) + "-";
    if (data.finishDate.month < 10) {
        dateAsString += "0" + std::to_string(data.finishDate.month) + "-";
    } else {
        dateAsString += std::to_string(data.finishDate.month) + "-";
    }
    if (data.finishDate.day < 10) {
        dateAsString += "0" + std::to_string(data.finishDate.day);
    } else {
        dateAsString += std::to_string(data.finishDate.day);
    }
    return dateAsString;
}


// virtual LLGames::~Node () {next = nullptr;}

// TODO: see that date.length() == 10 implies YYYY-MM-DD
bool LLGames::parseDate(std::string date, int numberDate[]) {
    if(date.length() != 8 && date.length() != 10) {
        std::cout << "Wrong format or illegal finish date. Format is YYYYMMDD or YYYY-MM-DD." << std::endl;
        return false;
    }
    if(date.length() == 8) {
        try {
            numberDate[0] = std::stoi(date.substr(0, 4));
            numberDate[1] = std::stoi(date.substr(4, 2));
            numberDate[2] = std::stoi(date.substr(6, 2));
        } catch (std::invalid_argument& error) {
            std::cout << "Wrong format or illegal finish date. Format is YYYYMMDD or YYYY-MM-DD." << std::endl;
            return false;
        }
    } else {
        try {
            numberDate[0] = std::stoi(date.substr(0, 4));
            numberDate[1] = std::stoi(date.substr(5, 2));
            numberDate[2] = std::stoi(date.substr(8, 2));
        } catch (std::invalid_argument& error) {
            std::cout << "Wrong format or illegal finish date. Format is YYYYMMDD or YYYY-MM-DD." << std::endl;
            return false;
        }
    }
    if(numberDate[0] < 1000 || numberDate[1] < 0 || numberDate[1] > 12 || numberDate[2] < 0 || numberDate[2] > 31) {
        return false;
    } else if (numberDate[1] == 0 && numberDate[2] != 0) {
        std::cout << "If month is \"0\", day must be \"0\"." << std::endl;
        return false;
    }

    return true;
}

unsigned int LLGames::Node::getRankNr () {
    return 420*this->data.finishDate.year + 32*this->data.finishDate.month + this->data.finishDate.day;
}

int LLGames::Node::nodeCompare(Node* other) {
    int dateCompare = this->getRankNr() - other->getRankNr();
    if(dateCompare != 0) {
        return dateCompare;
    } else {
        int nameCompare = toUpper(this->data.name).compare(toUpper(other->data.name));
        return nameCompare;
    }
}

// TODO: ensure month = 0 implies day = 0
void LLGames::insertGame(std::string name, std::string platform, std::string date) {
    int numberDate[3];
    bool parseResult = parseDate(date, numberDate);
    if (!parseResult) {
        std::cout << "Parsing error. Insertion failed." << std::endl;
        return;
    }

    Node* newGameNode = new Node(name, platform, numberDate, nullptr);

    if (first == nullptr) {
        this->first = newGameNode;
        this->size++;
        if(lengthOfName(name) > longestName) {
            longestName = lengthOfName(name);
        }
    } else {
        Node* currentNode = this->first;
        Node* prevNode = currentNode;
        int compareResult;
        bool inserted = false;
        while (currentNode != nullptr && !inserted) {
            compareResult = currentNode->nodeCompare(newGameNode);
            if(compareResult < 0) {
                prevNode = currentNode;
                currentNode = currentNode->getNext();
                if(currentNode == nullptr) {
                    prevNode->setNext(newGameNode);
                    this->size++;
                    inserted = true;
                    if(lengthOfName(name) > longestName) {
                        longestName = lengthOfName(name);
                    }
                }
            } else if (compareResult > 0) {
                newGameNode->setNext(currentNode);
                if(currentNode == this->first) {
                    this->first = newGameNode;
                } else {
                    prevNode->setNext(newGameNode);
                }
                this->size++;
                inserted = true;
                if(lengthOfName(name) > longestName) {
                    longestName = lengthOfName(name);
                }
            } else {
                std::cout << "du har försökt lägga till samma spel igen. F OFF" << std::endl;
                inserted = true;
            }
        }
        return;
    }
}

std::string LLGames::gameAsStr(Node* game) {
    std::string gameStr;
    gameStr.append(game->getName());
    gameStr.append(getSpacing(PRINT_TABLENGTH));
    gameStr.append(game->getPlatform());
    gameStr.append(getSpacing(longestPlatform+PRINT_TABLENGTH-game->getPlatform().length()));
    gameStr.append(game->dateToStr());
    return gameStr;
}

// return the game as string!?
std::string LLGames::removeGame(int index) {
    if(index < 1 || index > this->size) {
        std::cout << "Invalid index." << std:: endl;
        return "ERROR";
    }

    index--;
    Node* current = this->first;
    Node* prevNode;
    if(index == 0) {
        this->first = this->first->getNext();
    } else {
        for(int i = 0; i < index; i++) {
            prevNode = current;
            current = current->getNext();
        }
        prevNode->setNext(current->getNext());
    }

    std::string gameStr = gameAsStr(current);

    delete current;
    this->size--;
    // std::cout << "size after deletion: " << std::to_string(this->size) << std::endl;
    return gameStr;
}

void LLGames::printList(bool numbered) {
    // for(Node game : *this) {
    //     std::cout << game.name << std::endl;
    // }

    // pre-iterator implementation
    int tabLength = this->longestName + PRINT_TABLENGTH;
    Node* current = first;
    if(numbered) {
        for (size_t i = 0; i < this->size; i++) {
            std::cout << std::to_string(i+1) << getSpacing(PRINT_TABLENGTH) << current->getName() << getSpacing(tabLength-lengthOfName(current->getName())) << current->getPlatform() << getSpacing(longestPlatform+PRINT_TABLENGTH-current->getPlatform().length()) << current->dateToStr() << std::endl;
            current = current->getNext();
        }
    } else {
        for (size_t i = 0; i < this->size; i++) {
            std::cout << current->getName() << getSpacing(tabLength-lengthOfName(current->getName())) << current->getPlatform() << getSpacing(longestPlatform+PRINT_TABLENGTH-current->getPlatform().length()) << current->dateToStr() << std::endl;
            current = current->getNext();
        }
    }
}

bool LLGames::checkFilter(int filter, std::string currentPlatform) {
    if (filter == NO_FILTER)
    {
        return true;
    } else if (filter == BOOKS && currentPlatform == "BOOK")
    {
        return true;
    } else if (filter == GAMES && (currentPlatform == "PS4" || currentPlatform == "SWITCH"))
    {
        return true;
    } else if (filter == SWITCH && currentPlatform == "SWITCH")
    {
        return true;
    } else if (filter == PS4 && currentPlatform == "PS4")
    {
        return true;
    } else
    {
        return false;
    }    
}

void LLGames::printFiltered(int filter) {
    int tabLength = this->longestName + PRINT_TABLENGTH;
    Node* current = this->first;

    if(filter == NO_FILTER) {
        printList();
    } else {
        for (size_t i = 0; i < this->size; i++)
        {
            if (checkFilter(filter, current->getPlatform()))
            {
                std::cout << current->getName() << getSpacing(tabLength-lengthOfName(current->getName())) << current->getPlatform() << getSpacing(longestPlatform+PRINT_TABLENGTH-current->getPlatform().length()) << current->dateToStr() << std::endl;
            }
            current = current->getNext();
        }
        
    }
}

void LLGames::printToFile(std::string filename) {
    textFile_rw.open(filename, std::ios::out | std::ios::trunc);
    if(textFile_rw.is_open()) {
        int tabLength = this->longestName + PRINT_TABLENGTH;
        Node* current = first;
        for (int i = 0; i < this->size; i++) {
            textFile_rw << current->getName() << getSpacing(tabLength-lengthOfName(current->getName())) << current->getPlatform() << getSpacing(longestPlatform+PRINT_TABLENGTH-current->getPlatform().length()) << current->dateToStr() << std::endl;
            current = current->getNext();
        }
    } else {
        std::cout << "Failed to open for writing: " << TEXTFILE << std::endl;
    }
    textFile_rw.close();
}

bool LLGames::isPlatform(std::string input) {
    if(input == "PS4" || input == "SWITCH" || input == "PC" || input == "OTHER" || input == "BOOK")
        return true;
    else
        return false;
}

void LLGames::loadFromFile(std::string filename) {
    textFile_rw.open(filename, std::ios::in);
    if(textFile_rw.is_open()) {
        std::string tempInput;
        textFile_rw >> tempInput;
        while (textFile_rw.eof() == false) {
            std::string nameInput, platformInput, dateInput;
            // std::cout << "eof(): " << (textFile_rw.eof() ? "true" : "false") << std::endl;
            while(isPlatform(tempInput) == false) {
                nameInput.append(tempInput);
                nameInput += " ";
                textFile_rw >> tempInput;
            }
            nameInput.pop_back(); // remove last space character
            platformInput = tempInput;
            textFile_rw >> dateInput;
            insertGame(nameInput, platformInput, dateInput);
            textFile_rw >> tempInput;
            // std::cout << nameInput << std::endl << platformInput << std::endl;
            // std::cout << dateInput << std::endl << tempInput << std::endl;
        }
        textFile_rw.close();
    } else {
        std::cout << "Failed to open for reading: " << filename << std::endl;
    }
}

void LLGames::clearList() {
    int totalSize = this->size;
    for(int i = 0; i < totalSize; i++) {
        removeGame(1);
    }
}

int lengthOfName(std::string input) {
    int negCtr = 0;
    for(int i = 0; i < input.length(); i++) {
        negCtr += input[i] < 0;
    }

    negCtr /= 2;
    int realLength = input.length() - negCtr;
    return realLength;
}
