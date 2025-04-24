#ifndef GAMERULES_H
#define GAMERULES_H

#include <vector>

using namespace std;

//// Game amounts
////extern const vector<double> amounts;
//
// Function declarations
vector<int> shuffleAmounts();
int calculateBankOffer(const vector<int>& remainingAmounts, const int playercasevalue);
int roundcases(int round);

#endif // GAMERULES_H
