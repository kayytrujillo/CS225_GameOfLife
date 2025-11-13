#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;


class Asset {
public:
    virtual void print(ostream& os) const = 0;
    virtual ~Asset() {
    }
};

                                                    // operator overload for Asset
inline ostream& operator<<(ostream& os, const Asset& a) {
    a.print(os);
    return os;
}


                                                    //inheritance from class
class Job : public Asset {
public:
    string title;
    int salary;
    int estimatedTaxDue;

    Job() : title("None"), salary(0), estimatedTaxDue(0) {} //default constructor, makes base character model, incase accidentally not fulfilled

    Job(string t, int s, int tax) {                         //constructor, and proof of exceptions
        if (s < 0) throw string("ERROR: Salary cannot be negative!");
        title = t;
        salary = s;
        estimatedTaxDue = tax;
    }

    void print(ostream& os) const override {
        os << title << " (Salary $" << salary << ", Est Tax $" << estimatedTaxDue << ")";
    }

    ~Job() override {}                      //destructor
};



class House : public Asset {
public:
    string name;
    int price;
    int monthlyCost;

    House() : name("None"), price(0), monthlyCost(0) {}     //default constructor

    House(string n, int p, int m) {             //throw exception
        if (p < 0) throw string("ERROR: House price cannot be negative!");
        name = n;
        price = p;
        monthlyCost = m;
    }

    void print(ostream& os) const override {            //operator override
        os << name << " ($" << price << ", Monthly $" << monthlyCost << ")";
    }

    ~House() override {}                                //destructor, doesnt do anything as there is no necessary dynamic memory to clean up
};                                                      //prevents memiry leaks incase it is later expanded



class Player {
public:
    string name;
    bool collegeFirst;
    bool collegeFinished;
    bool jobChosen;
    bool houseChosen;
    bool goodGrades;
    bool eventUsed[10];
    
    int cash;
    int debt;

    Job job;       
    House house;  

    vector<string> children;

    Player() : name(""), collegeFirst(false), collegeFinished(false),
               jobChosen(false), houseChosen(false), goodGrades(false),
               cash(0), debt(0) 
    { for (int i = 0; i < 10; i++){
        eventUsed[i] = false;
        }   
                   
    }

    ~Player() {}
};


//function declarations
void runGame();
void intro(Player& p);
int  readIntInRange(int lo, int hi, const string& prompt);

void doJobDraft(Player& p);
void rollForHouse(Player& p);
void payday(Player& p);
void runRandomEvent(Player& p);

vector<vector<Job>> getJobPacks();
vector<House> getHousePool();

#endif
