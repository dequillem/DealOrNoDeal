#ifndef MAINMENU_H
#define MAINMENU_H
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
using namespace std;
void renderMainMenu(SDL_Renderer* renderer, TTF_Font* font, const string& backgroundPath);
string getname(SDL_Renderer* renderer, TTF_Font* font, const string& backgroundPath);
void renderInstructions(SDL_Renderer* renderer, TTF_Font* font, bool& showInstructions);
void cleanupMainMenuMusic(Mix_Chunk* thememusic);
#endif // MAINMENU_H
