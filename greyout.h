#ifndef GREYOUT_H
#define GREYOUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <algorithm>
using namespace std;

extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern const vector<int> amounts;

void renderGreyedAmounts(const vector<int>& openedAmounts, int playerCaseValue);
void renderOpenedCases(const vector<int>& caseAmounts, const vector<bool>& openedCases);

#endif
