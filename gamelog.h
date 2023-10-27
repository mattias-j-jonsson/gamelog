#pragma once

#include <fstream>

struct Date {
    unsigned int year;
    unsigned int month;
    unsigned int day;
};
typedef struct Date date_t;

struct Game {
    std::string name;
    std::string platform;
    date_t finishDate;
};
typedef struct Game game_t;


class LLGames {
private:

    class Node {
    private:
        game_t data;
        Node* next;
        // unsigned int dateValue;

        std::string checkPlatform(std::string platform);
        unsigned int getRankNr();

    public:
        Node (std::string name, std::string platform, int date[], Node* next);
        std::string getName() const;
        std::string getPlatform() const;
        date_t getFinishDate() const;
        Node* getNext() const;
        std::string dateToStr();
        // int* dateToInt();

        void setNext(Node* newNext);

        int nodeCompare(Node* other);

        virtual ~Node () {next = nullptr;};
    };

    Node* first;
    int size;
    // For formatting
    unsigned int longestName;
    static const unsigned int longestPlatform = 6;
    static const std::string TEXTFILE;
    std::fstream textFile_rw;

    bool parseDate(std::string date, int numberDate[]);
    bool isPlatform(std::string input);
    std::string gameAsStr(Node* game);

public:
    class GLIterator {
    private:
        Node* currentPos;
    public:
        GLIterator (Node* firstNode) : currentPos(firstNode) {};
        virtual ~GLIterator ();
    };

    LLGames() : first(nullptr), size(0), longestName(0) {};
    void insertGame(std::string name, std::string platform, std::string date);
    std::string removeGame(int index);
    int getSize() {return size;};
    void printList(bool numbered = false);
    void printToFile(std::string filename = TEXTFILE);
    void loadFromFile(std::string filename = TEXTFILE);
    void clearList();

    // Iteratorfunktioner
    GLIterator* begin() const {
        return new GLIterator(first);
    }
    virtual ~LLGames () {first = nullptr;};

};

int lengthOfName(std::string s);
