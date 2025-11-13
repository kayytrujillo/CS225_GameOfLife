#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Game.h"
using namespace std;

// random number setup
int randint(int lo, int hi) {
	return lo + rand() % (hi - lo + 1);
}

int readIntInRange(int lo, int hi, const string& prompt) {      //reduces need for constant while loops for each condition from the rand events
	int value;
	bool loopValue = true;
	do {
		cout << prompt;
		cin >> value;
		if (!cin.fail() && value >= lo && value <= hi) {
			cin.ignore(1000, '\n');
			return value;
		}
		cout << "Please enter a number between " << lo << " and " << hi << "." << endl;
		cin.clear();
		cin.ignore(1000, '\n');
	} while (loopValue == true);
	return lo;
}


void earn(Player& p, int amt, const string& why) {              //outputs to console the current cash amount of the user while updating from the amnt variable input
	p.cash += amt;
	
	cout << "\033[32m" <<why << " (+$" << amt << ")  Cash: $" << p.cash << "\033[0m" << endl;
}

void spend(Player& p, int amt, const string& why) {
	p.cash -= amt;
	cout <<"\033[31m" << why << " (-$" << amt << ")  Cash: $" << p.cash << "\033[0m" << endl;
}

// for formatting text output game log
void saveGameLog(Player& p) {
	string filename = "game_log_" + p.name + ".txt";
	for (size_t i = 0; i < filename.size(); ++i) {
		if (filename[i] == ' ') filename[i] = '_';
	}

	ofstream file(filename.c_str());
	if (!file) {
		cout << "ERROR! Could not save log file." << endl;
		return;
	}

	file << " _______ _                 _            __                   _             _             " << endl;
	file << "|__   __| |               | |          / _|                 | |           (_)            " << endl;
	file << "   | |  | |__   __ _ _ __ | | _____   | |_ ___  _ __   _ __ | | __ _ _   _ _ _ __   __ _ " << endl;
	file << "   | |  | '_ \\ / _` | '_ \\| |/ / __|  |  _/ _ \\| '__| | '_ \\| |/ _` | | | | | '_ \\ / _` |" << endl;
	file << "   | |  | | | | (_| | | | |   <\\__ \\  | || (_) | |    | |_) | | (_| | |_| | | | | | (_| |" << endl;
	file << "   |_|  |_| |_|\\__,_|_| |_|_|\\_\\___/  |_| \\___/|_|    | .__/|_|\\__,_|\\__, |_|_| |_|\\__, |" << endl;
	file << "                                                      | |             __/ |         __/ |" << endl;
	file << "                                                      |_|            |___/         |___/ " << endl;
	file << endl;





	file << "===== GAME SUMMARY =====" << endl;
	file << "Name: " << p.name << endl;
	if (p.jobChosen)
		file << "Job: " << p.job.title << ", Salary: $" << p.job.salary
		     << ", Est. Tax: $" << p.job.estimatedTaxDue << endl;
	if (p.houseChosen)
		file << "House: " << p.house.name << ", Price: $" << p.house.price
		     << ", Monthly: $" << p.house.monthlyCost << endl;
	file << "Good Grades Bonus Applied: " << (p.goodGrades ? "Yes (+$5000 to salary)" : "No") << endl;
	file << "Cash: $" << p.cash << endl;
	file << "Debt: $" << p.debt << endl;
	int assets = p.cash + (p.houseChosen ? p.house.price : 0);
	int netWorth = assets - p.debt;
	file << "Assets (cash + house if any): $" << assets << endl;
	file << "Final Net Worth: $" << netWorth << endl;

	file.close();
	cout << "Game log saved as '" << filename << "'" << endl;
}

                                                                    //data information setup from gamelog
vector<vector<Job>> getJobPacks() {
	vector<vector<Job>> packs;
	vector<Job> pack1, pack2, pack3;

	pack1.push_back({"Software Engineer", 95000, 19000});           // first three chosen
	pack1.push_back({"Nurse", 78000, 14000});
	pack1.push_back({"Graphic Designer", 62000, 9000});

	pack2.push_back({"Data Analyst", 80000, 15000});                //second three to choose from
	pack2.push_back({"High School Teacher", 60000, 8000});
	pack2.push_back({"Electrician", 68000, 10000});

	pack3.push_back({"Civil Engineer", 90000, 18000});              //last three pack to choose from
	pack3.push_back({"Marketing Manager", 85000, 17000});
	pack3.push_back({"Dental Hygienist", 76000, 13000});

	packs.push_back(pack1);
	packs.push_back(pack2);
	packs.push_back(pack3);
	return packs;
}

vector<House> getHousePool() {
	vector<House> homes;
	homes.push_back({"Cozy Condo",       24000, 600});
	homes.push_back({"Suburban Starter", 33000, 700});
	homes.push_back({"Downtown Loft",    42000, 800});
	homes.push_back({"Tiny Home",        18000,  500});
	homes.push_back({"Lakeview Cottage", 38000, 675});
	return homes;
}

//systems for assigning jobs
void doJobDraft(Player& p) {
	cout << endl << "=== JOB DRAFT ===" << endl;
	vector<vector<Job>> packs = getJobPacks();

	for (size_t round = 0; round < packs.size(); ++round) {
		cout << endl << "Round " << (round + 1) << ": choose 1, 2, or 3" << endl;
		for (size_t i = 0; i < packs[round].size(); ++i) {
			Job j = packs[round][i];
			cout << "  (" << (i + 1) << ") " << j.title
			     << ", Salary $" << j.salary
			     << ", Est. Tax $" << j.estimatedTaxDue << endl;
		}
		int pick = readIntInRange(1, 3, "Your choice: ");
		p.job = packs[round][pick - 1];
		cout << "You selected: " << p.job.title << endl;
	}

	if (p.goodGrades) {
		cout << endl << "Your good grades in college pay off!" << endl;
		p.job.salary += 5000;
		cout << "Career salary bonus +$5000. New salary: $" << p.job.salary << endl;
	}

	p.jobChosen = true;

	int signingBonus = randint(1000, 3000);
	p.cash += signingBonus;
	cout << "Signing bonus +$" << signingBonus << ". Cash: $" << p.cash << endl;

	cout << "Your final assigned job is: " << p.job.title
	     << " with a salary of $" << p.job.salary << "." << endl;
}
//system for assigning a house
void rollForHouse(Player& p) {
	cout << endl << endl << "=== HOUSE ROLL ===" << endl;
	vector<House> pool = getHousePool();
	int r = randint(1, (int)pool.size());
	House h = pool[r - 1];
	cout << "You were assigned: " << h.name << " ($" << h.price << ", monthly $" << h.monthlyCost << ")" << endl;

	int down = (int)(h.price * 0.10);                           // sddkmlkvm
	if (p.cash >= down) {
		p.cash -= down;
		p.debt += (h.price - down);
		p.house = h;
		p.houseChosen = true;
		cout << "Bought it! Down payment $" << down << ". Mortgage $" << (h.price - down)
		     << ". Cash \033[32m$" << p.cash << "\033[0m" << ", Debt $" << p.debt << endl;
	} else {
		cout << "Not enough for down payment, you rent instead (-$500)." << endl;
		p.cash -= 500;
	}
}

void payday(Player& p) {
	if (p.jobChosen) {
		int monthly = (int)((p.job.salary - p.job.estimatedTaxDue) / 12.0);  //
		p.cash += monthly;
		cout << "Payday! +$" << monthly << ". Cash \033[32m$" << p.cash << "\033[0m" << endl;
		if (p.houseChosen) {                                                    //if loop situations need to be reviewed!!
			p.cash -= p.house.monthlyCost;
			cout << "Housing upkeep -$" << p.house.monthlyCost << ". Cash \033[32m$" << p.cash << "\033[0m" << endl;
		}
	}
}

int getUniqueEvent(Player& p){
    int n;
    bool foundUnused = false;
    bool allUsed = true;
    for (int i = 1; i <= 9; i++){
        if (!p.eventUsed[i]){
            allUsed = false;
            break;
        }
    }
    if (allUsed){            //if this case is true
        for (int i = 1; i <= 9; i++){
            p.eventUsed[i] = false;
        }
    }
    do {
        n = randint(1,9);
        if (!p.eventUsed[n]){
            foundUnused = true;
        }
    } while (!foundUnused);
    
    p.eventUsed[n] = true;
    return n;
}


//random live events that user can possibly be simulated to experience
void runRandomEvent(Player& p) {
	int choice = getUniqueEvent(p);
	switch (choice) {
	case 1: {                                                           // Karaoke competition: pick difficulty
		cout << "Karaoke Night! Pick your song difficulty:" << endl;
		cout << "  1) Easy crowd-pleaser (low risk, low reward)" << endl;
		cout << "  2) Power Riff (higher risk/reward)" << endl;
		int songDiff = readIntInRange(1, 2, "Your choice: ");
		int roll = randint(1, 10);
		if (songDiff == 1) {
			if (roll <= 8) earn(p, 300, "Solid performance...tip jar");
			else spend(p, 100, "Mic feedback...you buy the next round");
		} else {
			if (roll <= 5) earn(p, 900, "Standing ovation...contest prize");
			else spend(p, 250, "Voice crack...tea & throat lozenges");
		}
		break;
	}
	case 2: {                                                           // Take a course for fun: pick free app vs paid class
		cout << "Learn something new!" << endl;
		cout << "  1) Free app (time cost, no money)" << endl;
		cout << "  2) Local class ($300, better networking)" << endl;
		int option = readIntInRange(1, 2, "Your choice: ");
		if (option == 1) {
			cout << "You practiced daily. Confidence up! (no cash change)" << endl;
		} else {
			spend(p, 300, "Enrolled in a local class");
			if (randint(1, 10) <= 6) earn(p, 500, "Classmate referral...small freelance gig");
		}
		break;
	}
	case 3: {                                                           // New language path: pick one of three languages, and some have higher incentives
		cout << "Pick a language to focus on:" << endl;
		cout << "  1) Spanish" << endl;
		cout << "  2) Japanese" << endl;
		cout << "  3) German" << endl;
		int lang = readIntInRange(1, 3, "Your choice: ");
		int roll = randint(1, 10);
		if (lang == 1) {                                                // Spanish common, moderate chance
			if (roll <= 7) {
				earn(p, 400, "Volunteer interpreter stipend");
			}
			else {
				spend(p, 150, "Books & flashcards");
			}
		}
		else if (lang == 2) {                                         // japanese harder, higher variance
			if (roll <= 4) {
				earn(p, 900, "Wow! You learned so much you earned a tour guide side gig");
			}
			else {
				spend(p, 250, "Required payment for  course materials");
			}
		} else {                                                        // german tech meetup to attend
			if (roll <= 6) {
				earn(p, 600, "Offered to attend a tech meetup translation honorarium");
			}
			else {
				spend(p, 180, "Purchased grammar workbook & app sub");
			}
		}
		break;
	}
	case 4: {                                                           // banana split mishap: choose to fix pants or
		cout << "You slipped on a banana split, and ... RIPPED YOUR PANTS??!" << endl;
		cout << "  1) Get the jeans repaired ($40)" << endl;
		cout << "  2) Ignore it (no cost, minor embarrassment)" << endl;
		int jeanPayment = readIntInRange(1, 2, "Your choice: ");
		if (jeanPayment == 1) {
			spend(p, 40, "Tailor fixes your jeans");
			cout << "They look brand new!" << endl;
		} else {
			cout << "You rock the ripped look, fashion statement ." << endl;
		}
		break;
	}
	case 5: {                                                           // scare actor audition
		cout << "At Universal Studios, you get offered a tiny scare acting audition. Would you like to audition?" << endl;
		cout << "  1) Yes" << endl;
		cout << "  2) No" << endl;
		int audition = readIntInRange(1, 2, "Your choice: ");
		if (audition == 1) {
			int roll = randint(1, 10);
			if (roll <= 5) {
				earn(p, 1000, "You landed a small role of a Chupacabra and received a stipend!"); //received the role
			}
		    else {
			    spend(p, 120, "Sorry, you didn't get the part. Still had to pay for parking + Uber to audition");
			}
		}
	    else {
		    cout << "You chose to pass. After all, its not everyone's calling." << endl;  //passed on the audition
		}
	    break;
	}
		case 6: {                                                           // internet famous
			cout << "You went viral! How do you respond?" << endl;
			cout << "  1) Brand collab (quick cash, risk backlash)" << endl;
			cout << "  2) Pass on it (no risk, no reward)" << endl;
			int viral = readIntInRange(1, 2, "Your choice: ");
			if (viral == 1) {
				int roll = randint(1, 10);
				if (roll <= 6) {
					earn(p, 1500, "Sponsored outfit post");
				}
				else {
					spend(p, 300, "EEK! Doesn't seem like it was received so well...need to pay publicist to clean up all the hate comments.");
				}
			} else {
				cout << "You keep your privacy and stay on the DL, no financial gain or loss." << endl;
			}
			break;
		}
		case 7: {                                                                                                   // trip to tokyo
			cout << "Spontaneous trip to Tokyo, Japan? 1=Yes 2=No" << endl;
			cout << "1) Yes" << endl;
			cout << "2) No"  << endl;
			int vacation = readIntInRange(1, 2, "Your choice: ");
			if (vacation == 1) {
				int roll = randint(1, 10);
				if (roll <= 3) {
					spend(p, 800, "Flat tire on the way to the airport and missed your flight, with no refund");  //charged for tire and missed flight
				}
				else if (roll <= 8) {
					spend(p, 1200, "You had an amazing trip! However, memories aren't free...");           // able to still go, but obv still charged for the trip
				}
				else {
					earn(p, 400, "Lucky you! You had extra miles and points from your credit card. Not a dime needed to be paid");     //chose not to go
				}
			} else {
				cout << "You stayed at home, where it's comfy and cheap. No effect." << endl;
			}
			break;
		}
		case 8: { // Everest training
			cout << "Mount Everest Summit Training Day!" << endl;
			cout << "  1) Push for a PR (high risk / high reward)" << endl;
			cout << "  2) Train safely (low risk / low reward)" << endl;
			int trainStyle = readIntInRange(1, 2, "Your choice: ");
			int roll = randint(1, 10);
			if (trainStyle == 1) {
				if (roll <= 3) {
					spend(p, 1500, "Oh no! You pulled a muscle! -Charged for medical bills");
				}
				else {
					earn(p, 2500, "A sponsor spotted your record breaking time and offered a stipend to climb for their team!");
				}
			}
			else {
				if (roll <= 8) {
					earn(p, 300, "Steady progress, coach rewards with a stipend");
				}
				else {
					spend(p, 150, "Blisters are the worst! Need to buy medical supplies and new equipment for periodical tuneup.");
				}
			}
			break;
		}
		case 9: {                                                                                   // slowpitch softball tournament
			cout << "Slowpitch Softball tournament in your area...pick your role:" << endl;
			cout << "  1) Backup Outfielder (safer)" << endl;
			cout << "  2) Confident Homerun Slugger (riskier)" << endl;
			int role = readIntInRange(1, 2, "Your choice: ");
			int roll = randint(1, 10);
			if (role == 1) {
				if (roll <= 8) {
					earn(p, 400, "Congrats, your team won! Team wins and splits the prize pool");
				}
				else {
					spend(p, 200, "YEOWCH! You pulled your muscle, emergency clinic visit needed..");
				}
			} else {
				if (roll <= 5) {
					earn(p, 700, "Hooray! You walked it off and won the MVP Prize!");
				}
				else {
					spend(p, 500, "Sliding into home plate with a close play ending in a collision. Hosipital sends you the bill.");
				}
			}
			break;
		}
	}
	}

	//game intro
	void intro(Player& p) {

		cout << "\033[93m";                         //determines hex code of what the header will be
		cout << "__        __   _                             _           _   _              ____                                __    _     _  __      " << endl;
		cout << "\\ \\      / /__| | ___ ___  _ __ ___   ___   | |_ ___    | |_| |__   ___    / ___| __ _ _ __ ___   ___     ___  / _|  | |   (_)/ _| ___ " << endl;
		cout << " \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\  | __/ _ \\   | __| '_ \\ / _ \\  | |  _ / _` | '_ ` _ \\ / _ \\   / _ \\| |_   | |   | | |_ / _ \\" << endl;
		cout << "  \\ V  V /  __/ | (_| (_) | | | | | |  __/  | || (_) |  | |_| | | |  __/  | |_| | (_| | | | | | |  __/  | (_) |  _|  | |___| |  _|  __/" << endl;
		cout << "   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|   \\__\\___/    \\__|_| |_|\\___|   \\____|\\__,_|_| |_| |_|\\___|   \\___/|_|    |_____|_|_|  \\___|" << endl;

		cout << "\033[0m" << endl << endl;          //resets color backk to normal for rest of game


		cout << "Enter your name: ";
		getline(cin, p.name);

		cout << "Do you want to start with College or Career first?" << endl;
		cout << "  1 = College first" << endl;
		cout << "  2 = Career first" << endl;
		int path = readIntInRange(1, 2, "Your choice: ");
		p.collegeFirst = (path == 1);

		p.collegeFinished = false;
		p.jobChosen = false;
		p.houseChosen = false;
		p.goodGrades = false;
		p.cash = 8000;
		p.debt = 0;

		if (!p.collegeFirst) {
			cout << endl << "Starting Cash: \033[32m$" << p.cash << "\033[0m" << endl;
			doJobDraft(p);
			rollForHouse(p);
		} else {
			cout << "College-first path: you will finish college before job selection." << endl;
			p.debt += 8000;
			cout << "Initial tuition debt +$8000. Debt $" << p.debt << endl;
			cout <<"Starting Cash: \033[32m$" << p.cash << "\033[0m" << endl;
		}
	}

	void runGame() {
		srand(time(0)); // seed random numbers
		Player p;
		intro(p);
		int gradeChoice = 0;
		int eventNum = 0;

		if (p.collegeFirst) {
			cout << endl << "You have now finished college!" << endl;
			cout << endl;
			p.collegeFinished = true;
			p.cash -= 5000;
			cout << "Tuition payment -$5000. Cash \033[32m$" << p.cash << "\033[0m" <<endl;

			while (gradeChoice != 1 && gradeChoice != 2) {
				cout << "Did you graduate with good grades? 1=Yes 2=No" << endl;
				cout << "Your choice: ";
				cin >> gradeChoice;

				if (gradeChoice != 1 && gradeChoice != 2) {
					cout << "Please enter 1 or 2." << endl;
				}
			}

			p.goodGrades = (gradeChoice == 1);
			doJobDraft(p);
			rollForHouse(p);
		}

		for (int paydayNum = 1; paydayNum <= 4; ++paydayNum) {
			cout << endl << "===== Events before Payday #" << paydayNum << " =====" << endl;
			
			cout << "Event #" << ++eventNum << ": ";
			runRandomEvent(p);
			cout << endl;
			
			cout << "Event #" << ++eventNum << ": ";
			runRandomEvent(p);
			cout << endl;
			cout << endl;
			
			cout << "-- Payday #" << paydayNum << " --" << endl;
			payday(p);
		}

		cout << endl << "===== RETIREMENT =====" << endl;
		cout << "You wrap up your career and reflect on your journey." << endl;

		int assets = p.cash + (p.houseChosen ? p.house.price : 0);
		int netWorth = assets - p.debt;

		cout << endl << "===== GAME OVER =====" << endl;
		cout << "Player: " << p.name << endl;
		if (p.jobChosen)
			cout << "Job: " << p.job.title << "  Salary:$" << p.job.salary << "  TaxDue:$" << p.job.estimatedTaxDue << endl;
		if (p.houseChosen)
			cout << "House: " << p.house.name << "  Price:$" << p.house.price << "  Monthly:$" << p.house.monthlyCost << endl;
		cout << "Good Grades Bonus Applied: " << (p.goodGrades ? "Yes (+$5000 to salary)" : "No") << endl;
		cout << "Cash: $" << p.cash << endl;
		cout << "Debt: $" << p.debt << endl;
		cout << "Assets (cash + house if any): $" << assets << endl;
		cout << "Final Net Worth: $" << netWorth << endl;
		cout << "Thanks for playing! Enjoy retirement." << endl;

		saveGameLog(p);                         // writes game_log_<your_name>.txt
	}
