#include "mainmenu.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int CASE_SIZE;
extern const int AMOUNT_MARGIN;

void renderMainMenu(SDL_Renderer* renderer, TTF_Font* font, const string& backgroundPath) {
    SDL_Surface* backgroundSurface = IMG_Load(backgroundPath.c_str());
    if (!backgroundSurface) {
        cerr << "Failed to load background image: " << IMG_GetError() << endl;
        return;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    string lines[4] = {"No mindless trivia.", "No tricky stunts.", "Just $1000000.", "There is only one question:"};

    for (int i = 0; i < 4; ++i) {
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* linesSurface = TTF_RenderText_Solid(font, lines[i].c_str(), textColor);
        SDL_Texture* linesTexture = SDL_CreateTextureFromSurface(renderer, linesSurface);
        SDL_Rect linesRect = { (SCREEN_WIDTH - linesSurface->w) / 2, (SCREEN_HEIGHT - linesSurface->h) / 2 + 50 * i, linesSurface->w, linesSurface->h };
        SDL_RenderCopy(renderer, linesTexture, nullptr, &linesRect);
        SDL_FreeSurface(linesSurface);
        SDL_DestroyTexture(linesTexture);

        SDL_RenderPresent(renderer);
        SDL_Delay(1500);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    string title = "Deal or No Deal";
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, title.c_str(), textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = { (SCREEN_WIDTH - titleSurface->w) / 2, (SCREEN_HEIGHT - titleSurface->h) / 2 - 50, titleSurface->w, titleSurface->h };
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    string prompt = "Press Space to Play";
    SDL_Surface* promptSurface = TTF_RenderText_Solid(font, prompt.c_str(), textColor);
    SDL_Texture* promptTexture = SDL_CreateTextureFromSurface(renderer, promptSurface);
    SDL_Rect promptRect = { (SCREEN_WIDTH - promptSurface->w) / 2, (SCREEN_HEIGHT - promptSurface->h) / 2 + 50, promptSurface->w, promptSurface->h };
    SDL_RenderCopy(renderer, promptTexture, nullptr, &promptRect);
    SDL_FreeSurface(promptSurface);
    SDL_DestroyTexture(promptTexture);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(backgroundTexture);
}

string getname(SDL_Renderer* renderer, TTF_Font* font, const string& backgroundPath) {
    string name = "";
    SDL_Event e;
    bool done = false;

    while (!done) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                done = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN && !name.empty()) {
                    done = true;
                } else if (e.key.keysym.sym == SDLK_BACKSPACE && !name.empty()) {
                    name.pop_back();
                } else if (e.key.keysym.sym == SDLK_SPACE) {
                    name += ' ';
                } else if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) {
                    name += (char)e.key.keysym.sym;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Surface* backgroundSurface = IMG_Load(backgroundPath.c_str());
        if (!backgroundSurface) {
            cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
            return "";
        }
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_DestroyTexture(backgroundTexture);

        // Render the prompt
        string prompt = "Enter your name: " + name;
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Surface* promptSurface = TTF_RenderText_Solid(font, prompt.c_str(), textColor);
        SDL_Texture* promptTexture = SDL_CreateTextureFromSurface(renderer, promptSurface);
        SDL_Rect promptRect = { (1280 - promptSurface->w) / 2, (720 - promptSurface->h) / 2, promptSurface->w, promptSurface->h };
        SDL_RenderCopy(renderer, promptTexture, nullptr, &promptRect);
        SDL_FreeSurface(promptSurface);
        SDL_DestroyTexture(promptTexture);

        SDL_RenderPresent(renderer);
    }

    return name;
}



















