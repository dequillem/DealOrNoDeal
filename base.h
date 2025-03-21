#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;
extern SDL_Renderer* renderer;
extern TTF_Font* font;

SDL_Texture* loadTexture(const string& filePath);
void renderText(const string& text, int x, int y, SDL_Color color);

#endif // UTILS_H
