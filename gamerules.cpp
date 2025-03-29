#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>
#include "gamerules.h"

using namespace std;
extern int round;

const vector<int> amounts = {
    0, 1, 5, 10, 25, 50, 75, 100, 200, 300, 400, 500, 750,
    1000, 5000, 10000, 25000, 50000, 75000, 100000, 200000, 300000,
    400000, 500000, 750000, 1000000
};

vector<int> shuffleAmounts() {
    vector<int> shuffledAmounts = amounts;
    random_device rd;
    mt19937 g(rd());
    shuffle(shuffledAmounts.begin(), shuffledAmounts.end(), g);
    return shuffledAmounts;
}

void displayCases(const vector<int>& remainingCases) {
    cout << "Remaining cases: ";
    for (int caseNum : remainingCases) {
        cout << caseNum << " ";
    }
    cout << endl;
}

int calculateBankOffer(const vector<int>& remainingAmounts) {
    double sum = 0;
    for (int amount : remainingAmounts) {
        sum += (double)sqrt(amount);
    }
    int average = (int)sum / remainingAmounts.size();
    return average * average;
}

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
