#include "mainmenu.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "const.h"
using namespace std;
// Hàm in intro đầu game
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
        SDL_Rect linesRect = { (SCREEN_WIDTH - linesSurface->w) / 2, (SCREEN_HEIGHT - linesSurface->h) / 2 - 50 + 50 * i, linesSurface->w, linesSurface->h };
        SDL_RenderCopy(renderer, linesTexture, nullptr, &linesRect);
        SDL_FreeSurface(linesSurface);
        SDL_DestroyTexture(linesTexture);

        SDL_RenderPresent(renderer);
        SDL_Delay(1500);
    }
    SDL_DestroyTexture(backgroundTexture);

    SDL_Surface* menuBackgroundSurface = IMG_Load("assets/mainmenu.png");
    SDL_Texture* menuBackgroundTexture = SDL_CreateTextureFromSurface(renderer, menuBackgroundSurface);
    SDL_FreeSurface(menuBackgroundSurface);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, menuBackgroundTexture, nullptr, nullptr);
    string prompt = "Press Space to Play";
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* promptSurface = TTF_RenderText_Solid(font, prompt.c_str(), textColor);
    SDL_Texture* promptTexture = SDL_CreateTextureFromSurface(renderer, promptSurface);
    SDL_Rect promptRect = { (SCREEN_WIDTH - promptSurface->w) / 2, (SCREEN_HEIGHT - promptSurface->h) / 2 + 200, promptSurface->w, promptSurface->h };
    SDL_RenderCopy(renderer, promptTexture, nullptr, &promptRect);
    SDL_FreeSurface(promptSurface);
    SDL_DestroyTexture(promptTexture);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(backgroundTexture);
}
// Hàm lấy tên người chơi
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



















