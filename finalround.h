#ifndef FINALROUND_H
#define FINALROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

extern SDL_Renderer* renderer;
extern TTF_Font* font;

void finalRound(int playerCase, int lastRemainingCase, const std::vector<int>& caseAmounts, int& winningAmount, bool& returnToMainMenu);

#endif // FINALROUND_H
