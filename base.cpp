#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
using namespace std;

extern SDL_Renderer* renderer;
extern TTF_Font* font;
struct Mix_Chunk;
struct Mix_Music;
extern Mix_Chunk* soundEffect;
extern Mix_Music* backgroundMusic;

// Hàm tải texture lên
SDL_Texture* loadTexture(const string& filePath) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (surface == nullptr) {
        cerr << "Failed to load image: " << IMG_GetError() << endl;
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        cerr << "Failed to create texture: " << SDL_GetError() << endl;
    }

    return texture;
}
// Hàm in text
void renderText(const string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        cerr << "Failed to render text surface: " << TTF_GetError() << endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        cerr << "Failed to create texture from surface: " << SDL_GetError() << endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

bool initAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << endl;
        return false;
    }
    return true;
}

// Load sound effect
Mix_Chunk* loadSoundEffect(const string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (!sound) {
        cerr << "Failed to load sound effect! Error: " << Mix_GetError() << endl;
    }
    return sound;
}

// Load music track
Mix_Music* loadMusic(const string& filePath) {
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        cerr << "Failed to load music! Error: " << Mix_GetError() << endl;
    }
    return music;
}

// Play sound effect
void playSoundEffect(Mix_Chunk* sound, int loops = 0) {
    if (sound) {
        if (Mix_PlayChannel(-1, sound, loops) == -1) {
            cerr << "Failed to play sound effect: " << Mix_GetError() << endl;
        }
    }
}

// Play background music
void playMusic(Mix_Music* music, int loops = -1) {
    if (music) {
        if (Mix_PlayMusic(music, loops) == -1) {
            cerr << "Failed to play music: " << Mix_GetError() << endl;
        }
    }
}

void cleanupAudio() {
    if (soundEffect) Mix_FreeChunk(soundEffect);
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
}
