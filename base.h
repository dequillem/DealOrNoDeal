#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
struct Mix_Chunk;
struct Mix_Music;
extern Mix_Chunk* soundEffect;
extern Mix_Music* backgroundMusic;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int CASE_SIZE;
extern const int CASE_MARGIN;
extern const int AMOUNT_MARGIN;

SDL_Texture* loadTexture(const string& filePath);
void renderText(const string& text, int x, int y, SDL_Color color);
bool initAudio();
Mix_Chunk* loadSoundEffect(const string& filePath);
Mix_Music* loadMusic(const string& filePath);
void playSoundEffect(Mix_Chunk* sound, int loops = 0);
void playMusic(Mix_Music* music, int loops = -1);
void cleanupAudio();

#endif // UTILS_H
