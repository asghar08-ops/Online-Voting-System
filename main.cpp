#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Candidate {
    int id;
    string name;
    int votes;
};

struct Voter {
    string cnic;
    bool hasVoted;
};

class VotingSystem {
private:
    vector<Candidate> candidates;
    vector<Voter> voters;

    // ---------- File Handling ----------
    void loadCandidates() {
        ifstream file("candidates.txt");
        candidates.clear();
        int id, votes;
        string name;
        while (file >> id >> name >> votes) {
            candidates.push_back({id, name, votes});
        }
        file.close();
    }

    void saveCandidates() {
        ofstream file("candidates.txt");
        for (auto &c : candidates) {
            file << c.id << " " << c.name << " " << c.votes << endl;
        }
        file.close();
    }

    void loadVoters() {
        ifstream file("voters.txt");
        voters.clear();
        string cnic;
        bool voted;
        while (file >> cnic >> voted) {
            voters.push_back({cnic, voted});
        }
        file.close();
    }

    void saveVoters() {
        ofstream file("voters.txt");
        for (auto &v : voters) {
            file << v.cnic << " " << v.hasVoted << endl;
        }
        file.close();
    }

    void logEvent(string msg) {
        ofstream file("logs.txt", ios::app);
        file << msg << endl;
        file.close();
    }

public:
    VotingSystem() {
        loadCandidates();
        loadVoters();
    }

    // ---------- Admin ----------
    void addCandidate() {
        Candidate c;
        cout << "\nCandidate ID: ";
        cin >> c.id;
        cout << "Candidate Name: ";
        cin >> c.name;
        c.votes = 0;
        candidates.push_back(c);
        saveCandidates();
        logEvent("Candidate added: " + c.name);
        cout << "Candidate added successfully!\n";
    }

    void removeCandidate() {
        int id;
        cout << "\nEnter Candidate ID to remove: ";
        cin >> id;
        for (auto it = candidates.begin(); it != candidates.end(); ++it) {
            if (it->id == id) {
                logEvent("Candidate removed: " + it->name);
                candidates.erase(it);
                saveCandidates();
                cout << "Candidate removed.\n";
                return;
            }
        }
        cout << "Candidate not found!\n";
    }

    void viewVoters() {
        cout << "\n--- Registered Voters ---\n";
        for (auto &v : voters) {
            cout << v.cnic << " | Voted: " << (v.hasVoted ? "Yes" : "No") << endl;
        }
    }

    void resetElection() {
        ofstream c("candidates.txt");
        ofstream v("voters.txt");
        ofstream l("logs.txt", ios::app);
        l << "Election reset\n";
        c.close(); v.close(); l.close();
        candidates.clear();
        voters.clear();
        cout << "Election data reset successfully.\n";
    }

    // ---------- Voter ----------
    void registerVoter() {
        Voter v;
        cout << "\nEnter CNIC/ID: ";
        cin >> v.cnic;
        v.hasVoted = false;

        for (auto &x : voters) {
            if (x.cnic == v.cnic) {
                cout << "Voter already registered!\n";
                return;
            }
        }

        voters.push_back(v);
        saveVoters();
        logEvent("Voter registered: " + v.cnic);
        cout << "Voter registered successfully!\n";
    }

    void showCandidates() {
        cout << "\n--- Candidates ---\n";
        for (auto &c : candidates) {
            cout << c.id << ". " << c.name << endl;
        }
    }

    void vote() {
        string cnic;
        cout << "\nEnter CNIC/ID: ";
        cin >> cnic;

        for (auto &v : voters) {
            if (v.cnic == cnic) {
                if (v.hasVoted) {
                    cout << "You have already voted!\n";
                    return;
                }

                showCandidates();
                int choice;
                cout << "Enter Candidate ID: ";
                cin >> choice;

                for (auto &c : candidates) {
                    if (c.id == choice) {
                        c.votes++;
                        v.hasVoted = true;
                        saveCandidates();
                        saveVoters();
                        logEvent("Vote cast by: " + cnic);
                        cout << "Vote successfully submitted!\n";
                        return;
                    }
                }
                cout << "Invalid candidate!\n";
                return;
            }
        }
        cout << "Voter not registered!\n";
    }

    void showResults() {
        cout << "\n--- Election Results ---\n";
        for (auto &c : candidates) {
            cout << c.name << " : " << c.votes << " votes\n";
        }
    }

    // ---------- Menus ----------
    void adminMenu() {
        int ch;
        do {
            cout << "\n=== ADMIN PANEL ===\n";
            cout << "1. Add Candidate\n";
            cout << "2. Remove Candidate\n";
            cout << "3. View Voters\n";
            cout << "4. View Results\n";
            cout << "5. Reset Election\n";
            cout << "6. Back\n";
            cout << "Choice: ";
            cin >> ch;

            switch (ch) {
                case 1: addCandidate(); break;
                case 2: removeCandidate(); break;
                case 3: viewVoters(); break;
                case 4: showResults(); break;
                case 5: resetElection(); break;
                case 6: break;
                default: cout << "Invalid choice!\n";
            }
        } while (ch != 6);
    }

    void voterMenu() {
        int ch;
        do {
            cout << "\n=== VOTER PANEL ===\n";
            cout << "1. Register Voter\n";
            cout << "2. Vote\n";
            cout << "3. View Results\n";
            cout << "4. Back\n";
            cout << "Choice: ";
            cin >> ch;

            switch (ch) {
                case 1: registerVoter(); break;
                case 2: vote(); break;
                case 3: showResults(); break;
                case 4: break;
                default: cout << "Invalid choice!\n";
            }
        } while (ch != 4);
    }

    void mainMenu() {
        int ch;
        do {
            cout << "\n====== ONLINE VOTING SYSTEM ======\n";
            cout << "1. Admin Panel\n";
            cout << "2. Voter Panel\n";
            cout << "3. Exit\n";
            cout << "Choice: ";
            cin >> ch;

            switch (ch) {
                case 1: adminMenu(); break;
                case 2: voterMenu(); break;
                case 3: cout << "System shutdown.\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (ch != 3);
    }
};

int main() {
    VotingSystem system;
    system.mainMenu();
    return 0;
}
