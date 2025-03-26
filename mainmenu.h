#ifndef MAINMENU_H
#define MAINMENU_H
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string>
using namespace std;
void renderMainMenu(SDL_Renderer* renderer, TTF_Font* font, const string& backgroundPath);
string getname(SDL_Renderer* renderer, TTF_Font* font, const string& backgroundPath);

#endif // MAINMENU_H
