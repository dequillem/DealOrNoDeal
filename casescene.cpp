#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "casescene.h"
#include "base.h"
#include "const.h"
#include <SDL2/SDL_mixer.h>
using namespace std;

Mix_Chunk* soundEffect = nullptr;
Mix_Music* backgroundMusic = nullptr;
// Hàm chuyển cảnh mở vali trong trường hợp người chơi loại bỏ vali nào đó
void renderCaseScene(int chosenCase, int caseValue) {
    SDL_Texture* caseTexture = loadTexture("assets/case.png");
    if (caseTexture == nullptr) {
        cerr << "Failed to load case texture!" << endl;
        return;
    }
    Mix_Chunk* buildup = loadSoundEffect("assets/CaseReveal.mp3");
    if (!buildup) {
        cerr << "Failed to load case open sound effect!" << endl;
    }
    Mix_Chunk* bigcase = loadSoundEffect("assets/BigCase.mp3");
    if (!bigcase) {
        cerr << "Failed to load case open sound effect!" << endl;
    }
    Mix_Chunk* smallcase = loadSoundEffect("assets/SmallCase.mp3");
    if (!smallcase) {
        cerr << "Failed to load case open sound effect!" << endl;
    }
    if (caseTexture == nullptr) {
        cerr << "Failed to load case texture!" << endl;
        if (bigcase) Mix_FreeChunk(bigcase);
        if (smallcase) Mix_FreeChunk(smallcase);
        return;
    }
    bool quit = false;
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();
    bool valueRevealed = false;
    bool buildupPlayed = false;
    bool soundPlayed = false;
    TTF_SetFontSize(font, 100);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE)
                    quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        // Render the chosen case
        int x = (SCREEN_WIDTH - CASE_SIZE * 5) / 2;
        int y = (SCREEN_HEIGHT - CASE_SIZE * 5) / 2;
        SDL_Rect caseRect = { x, y, CASE_SIZE * 5, CASE_SIZE * 5 };
        SDL_RenderCopy(renderer, caseTexture, nullptr, &caseRect);

        renderText(to_string(chosenCase + 1), (SCREEN_WIDTH - CASE_SIZE) / 2, (SCREEN_HEIGHT - CASE_SIZE) / 2, { 0, 150, 0, 255 });
        if (!buildupPlayed) {
            playSoundEffect(buildup);
            buildupPlayed = true;
        }

        if (!valueRevealed && SDL_GetTicks() - startTime > 2000) {
            if (caseValue < 25000 && smallcase && !soundPlayed) {
                playSoundEffect(smallcase);
                soundPlayed = true;
            }
            if (caseValue >= 25000 && bigcase && !soundPlayed) {
                playSoundEffect(bigcase);
                soundPlayed = true;
            }
            valueRevealed = true;
        }

        if (valueRevealed) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, ("$" + to_string(caseValue)).c_str(), { 255, 255, 255, 255 });
            if (textSurface == nullptr) {
                cerr << "Failed to render text surface: " << TTF_GetError() << endl;
            } else {
                int textWidth = textSurface->w;
                int textHeight = textSurface->h;
                int textX = (SCREEN_WIDTH - textWidth) / 2;
                int textY = ((SCREEN_HEIGHT - textHeight) / 2) + 200;

                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = { textX, textY, textWidth, textHeight };
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
            renderText("Press Space to Return", 100, 600, {255, 255, 255, 255});
        }
        SDL_DestroyTexture(backgroundTexture);
        SDL_RenderPresent(renderer);
    }

    TTF_SetFontSize(font, 40);
    SDL_DestroyTexture(caseTexture);
    if (buildup) Mix_FreeChunk(buildup);
    if (bigcase) Mix_FreeChunk(bigcase);
    if (smallcase) Mix_FreeChunk(smallcase);
}



