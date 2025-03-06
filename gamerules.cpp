#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>

using namespace std;

const vector<double> amounts = {
    0.01, 1, 5, 10, 25, 50, 75, 100, 200, 300, 400, 500, 750,
    1000, 5000, 10000, 25000, 50000, 75000, 100000, 200000, 300000,
    400000, 500000, 750000, 1000000
};

// Function to shuffle the amounts in cases
vector<double> shuffleAmounts() {
    vector<double> shuffledAmounts = amounts;
    random_device rd;
    mt19937 g(rd());
    shuffle(shuffledAmounts.begin(), shuffledAmounts.end(), g);
    return shuffledAmounts;
}

// Function to display the remaining cases
void displayCases(const vector<int>& remainingCases) {
    cout << "Remaining cases: ";
    for (int caseNum : remainingCases) {
        cout << caseNum << " ";
    }
    cout << endl;
}

// Function to calculate the bank offer
double calculateBankOffer(const vector<double>& remainingAmounts) {
    double sum = 0;
    for (double amount : remainingAmounts) {
        sum += sqrt(amount);
    }
    int average = (int)sum / remainingAmounts.size();
    return average * average;
}

// Function to determine the number of cases to eliminate based on the round
int roundcases(int round) {
    int elim = 0;
    switch (round) {
        case 1:
            elim = 6;
            break;
        case 2:
            elim = 5;
            break;
        case 3:
            elim = 4;
            break;
        case 4:
            elim = 3;
            break;
        case 5:
            elim = 2;
            break;
        default:
            elim = 1;
    }
    return elim;
}

// Function to get the player's guess (not used in this version of the game)
int getPlayerGuess() {
    int number;
    cout << "Choose a number from 1-10: ";
    cin >> number;
    while (number < 1 || number > 10) {
        cout << "Invalid number, Try again: ";
        cin >> number;
    }
    return number;
}

int main() {
    cout << "Welcome to Deal or No Deal!" << endl;
    cout << "There are 26 cases, each containing a unique amount of money." << endl;
    cout << "Your goal is to eliminate cases and negotiate with the bank to win the highest amount possible." << endl;

    vector<double> caseAmounts = shuffleAmounts();
    vector<int> remainingCases;
    for (int i = 1; i <= 26; i++) {
        remainingCases.push_back(i);
    }

    // Player chooses a case
    int playerCase;
    cout << "Choose your case (1-26): ";
    cin >> playerCase;
    while (playerCase < 1 || playerCase > 26) {
        cout << "Invalid case number. Choose a case between 1 and 26: ";
        cin >> playerCase;
    }

    // Remove the player's case from the remaining cases
    auto it = find(remainingCases.begin(), remainingCases.end(), playerCase);
    remainingCases.erase(it);

    // Game loop
    int round = 1;
    while (round <= 9) {
        int elimcases = roundcases(round); // Get the number of cases to eliminate for this round
        cout << "It's round " << round << ", you have to eliminate " << elimcases << " cases." << endl;

        int haselim = 0;
        while (haselim < elimcases) {
            cout << "\n----------------------------------------" << endl;
            displayCases(remainingCases);

            // Player chooses a case to eliminate
            int caseToEliminate;
            cout << "Choose a case to eliminate: ";
            cin >> caseToEliminate;
            while (find(remainingCases.begin(), remainingCases.end(), caseToEliminate) == remainingCases.end()) {
                cout << "Invalid case number. Choose a remaining case: ";
                cin >> caseToEliminate;
            }

            // Remove the eliminated case
            it = find(remainingCases.begin(), remainingCases.end(), caseToEliminate);
            remainingCases.erase(it);

            // Display the amount in the eliminated case
            double eliminatedAmount = caseAmounts[caseToEliminate - 1];
            cout << "Case " << caseToEliminate << " contained: $" << fixed << setprecision(0) << eliminatedAmount << endl;

            haselim++;
        }

        // Calculate and display the bank offer
        vector<double> remainingAmounts;
        for (int caseNum : remainingCases) {
            remainingAmounts.push_back(caseAmounts[caseNum - 1]);
        }
        double bankOffer = calculateBankOffer(remainingAmounts);
        cout << "The bank offers: $" << fixed << setprecision(0) << bankOffer << endl;

        // Ask the player if they want to deal or no deal
        string choice;
        cout << "Deal or No Deal? (Enter 'yes' or 'no'): ";
        cin >> choice;
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

        if (choice == "yes") {
            cout << "Congratulations! You win $" << fixed << setprecision(0) << bankOffer << "!" << endl;
            return 0;
        }
        else if (choice == "no") {
            cout << "Let's go to the next round!" << endl;
            round++;
        }
        else {
            cout << "Quick! Make a choice!" << endl;
        }
    }

    // Final case
    cout << "\n----------------------------------------" << endl;
    cout << "Only your case and one other case remain." << endl;
    string swapChoice;
    cout << "Do you want to swap your case? (yes/no): ";
    cin >> swapChoice;

    if (swapChoice == "yes") {
        int finalCase = remainingCases[0];
        cout << "You swapped your case for case " << finalCase << "." << endl;
        cout << "Your case contained: $" << fixed << setprecision(0) << caseAmounts[finalCase - 1] << endl;
    }
    else {
        cout << "You kept your case." << endl;
        cout << "Your case contained: $" << fixed << setprecision(0) << caseAmounts[playerCase - 1] << endl;
    }

    return 0;
}
