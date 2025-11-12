#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
using namespace std;

struct Job {
    string title;
    int salary;
    int estimatedTaxDue;
};

struct House {
    string name;
    int price;
    int monthlyCost;
};

struct Player {
    string name;
    bool collegeFirst;
    bool collegeFinished;
    bool jobChosen;
    bool houseChosen;
    bool goodGrades;

    int cash;
    int debt;

    Job job;
    House house;
    vector<string> children;
};

void runGame();
void intro(Player& p);
int  readIntInRange(int lo, int hi, const string& prompt);
void pressEnter(const string& prompt);

void doJobDraft(Player& p);
void rollForHouse(Player& p);
void payday(Player& p);
void runRandomEvent(Player& p);

vector<vector<Job>> getJobPacks();
vector<House> getHousePool();

#endif // GAME_H
