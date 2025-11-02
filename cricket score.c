#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Player {
    string name;
    int runs = 0;
    int balls = 0;
    bool out = false;
};

struct Team {
    string name;
    vector<Player> players;
    int totalRuns = 0;
    int wickets = 0;
    int overs = 0;
    int ballsInOver = 0;
    int extras = 0;

    void addPlayer(const string& pname) {
        Player p; p.name = pname;
        players.push_back(p);
    }
};

void printLine() {
    cout << "-----------------------------------------------\n";
}

void displayScore(const Team &t) {
    cout << "\nScoreboard: " << t.name << "\n";
    printLine();
    cout << left << setw(20) << "Player" << setw(8) << "R" << setw(8) << "B" << setw(6) << "Out" << "\n";
    printLine();
    for (size_t i=0;i<t.players.size();++i) {
        const Player &p = t.players[i];
        cout << left << setw(20) << p.name
             << setw(8) << p.runs
             << setw(8) << p.balls
             << setw(6) << (p.out ? "Yes" : "No") << "\n";
    }
    printLine();
    cout << "Total: " << t.totalRuns << "/" << t.wickets
         << "  Overs: " << t.overs << "." << t.ballsInOver
         << "  Extras: " << t.extras << "\n";
    printLine();
}

int nextBatsmanIndex(const Team &t) {
    for (size_t i=0;i<t.players.size();++i) {
        if (!t.players[i].out && t.players[i].balls==0 && t.players[i].runs==0)
            return (int)i;
    }
    for (size_t i=0;i<t.players.size();++i)
        if (!t.players[i].out) return (int)i;
    return -1;
}

bool isAllOut(const Team &t) {
    return t.wickets >= (int)t.players.size() - 1;
}

void ballPlayed(Team &t, int strikerIndex, int runs) {
    t.totalRuns += runs;
    t.players[strikerIndex].runs += runs;
    t.players[strikerIndex].balls += 1;
    t.ballsInOver += 1;
    if (t.ballsInOver == 6) {
        t.overs += 1;
        t.ballsInOver = 0;
    }
}

int main() {
    cout << "=== Cricket Score Management System ===\n\n";

    Team team1, team2;
    cout << "Enter Team 1 name: ";
    getline(cin, team1.name);
    cout << "Enter Team 2 name: ";
    getline(cin, team2.name);

    cout << "\nEnter 11 players for " << team1.name << ":\n";
    for (int i=0;i<11;++i) {
        cout << "Player " << i+1 << " name: ";
        string pname; getline(cin, pname);
        if (pname.empty()) { --i; continue; }
        team1.addPlayer(pname);
    }

    cout << "\nEnter 11 players for " << team2.name << ":\n";
    for (int i=0;i<11;++i) {
        cout << "Player " << i+1 << " name: ";
        string pname; getline(cin, pname);
        if (pname.empty()) { --i; continue; }
        team2.addPlayer(pname);
    }

    Team *batting = &team1;
    Team *bowling = &team2;
    cout << "\n" << batting->name << " will bat first.\n";

    int striker = 0, nonStriker = 1;
    cout << "\nOpening batsmen: " << batting->players[striker].name << " (striker), "
         << batting->players[nonStriker].name << " (non-striker)\n";

    int choice, maxOvers = 2;
    bool inningsOver = false;
    cout << "\nEnter number of overs for the match (e.g., 2): ";
    string tmp; getline(cin, tmp);
    if (!tmp.empty()) maxOvers = stoi(tmp);

    while (!inningsOver) {
        cout << "\nMenu:\n";
        cout << "1. Add runs\n";
        cout << "2. Add wicket\n";
        cout << "3. Display scoreboard\n";
        cout << "4. End innings\n";
        cout << "Choose option: ";
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000,'\n'); continue; }

        if (choice == 1) {
            int runs; cout << "Enter runs (0/1/2/3/4/6): "; cin >> runs;
            ballPlayed(*batting, striker, runs);
            if (runs % 2 == 1) swap(striker, nonStriker);
            if (batting->ballsInOver == 0) swap(striker, nonStriker);
        }
        else if (choice == 2) {
            batting->wickets++;
            batting->players[striker].out = true;
            batting->players[striker].balls++;
            cout << batting->players[striker].name << " is OUT!\n";
            int idx = nextBatsmanIndex(*batting);
            if (idx == -1 || isAllOut(*batting)) {
                cout << "All out! Innings ended.\n";
                inningsOver = true;
            } else striker = idx;
        }
        else if (choice == 3) {
            displayScore(*batting);
        }
        else if (choice == 4) {
            inningsOver = true;
        }

        if (batting->overs >= maxOvers) {
            cout << "Overs completed. Innings ended.\n";
            inningsOver = true;
        }
    }

    cout << "\nFirst Innings Complete. Total: " << batting->totalRuns << "/" << batting->wickets
         << " in " << batting->overs << "." << batting->ballsInOver << "\n";

    cout << "\nThank you for using the Cricket Score Management System.\n";
    return 0;
}
