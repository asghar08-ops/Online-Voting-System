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

public:
    VotingSystem() {
        loadCandidates();
        loadVoters();
    }

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

    // ---------- Core Functions ----------
    void addCandidate() {
        Candidate c;
        cout << "\nEnter Candidate ID: ";
        cin >> c.id;
        cout << "Enter Candidate Name: ";
        cin >> c.name;
        c.votes = 0;
        candidates.push_back(c);
        saveCandidates();
        cout << "Candidate added successfully!\n";
    }

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
        cout << "Voter registered successfully!\n";
    }

    void showCandidates() {
        cout << "\n--- Candidates List ---\n";
        for (auto &c : candidates) {
            cout << c.id << ". " << c.name << endl;
        }
    }

    void vote() {
        string cnic;
        cout << "\nEnter your CNIC/ID: ";
        cin >> cnic;

        for (auto &v : voters) {
            if (v.cnic == cnic) {
                if (v.hasVoted) {
                    cout << "You have already voted!\n";
                    return;
                }

                showCandidates();
                int choice;
                cout << "Enter Candidate ID to vote: ";
                cin >> choice;

                for (auto &c : candidates) {
                    if (c.id == choice) {
                        c.votes++;
                        v.hasVoted = true;
                        saveCandidates();
                        saveVoters();
                        cout << "Vote cast successfully!\n";
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
        cout << "\n--- Voting Results ---\n";
        for (auto &c : candidates) {
            cout << c.name << " : " << c.votes << " votes\n";
        }
    }

    // ---------- Menu ----------
    void menu() {
        int choice;
        do {
            cout << "\n====== Online Voting System ======\n";
            cout << "1. Add Candidate (Admin)\n";
            cout << "2. Register Voter\n";
            cout << "3. Vote\n";
            cout << "4. Show Results\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addCandidate(); break;
                case 2: registerVoter(); break;
                case 3: vote(); break;
                case 4: showResults(); break;
                case 5: cout << "System closed.\n"; break;
                default: cout << "Invalid choice!\n";
            }

        } while (choice != 5);
    }
};

int main() {
    VotingSystem vs;
    vs.menu();
    return 0;
}
