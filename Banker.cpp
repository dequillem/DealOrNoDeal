#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "banker.h"
#include "base.h"
#include "gamerules.h"
#include "const.h"
using namespace std;

extern SDL_Renderer* renderer;
extern TTF_Font* font;

void renderBankerScene(const vector<int>& remainingAmounts, bool& dealAccepted) {
    bool quit = false;
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();
    bool offerRevealed = false;
    int bankOffer = 0;

    TTF_SetFontSize(font, 35);

    //fade effect
    Uint32 fadeStartTime = SDL_GetTicks();
    bool fadingOut = true;
    Uint8 alpha = 255;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_d) { //Deal
                    dealAccepted = true;
                    quit = true;
                } else if (e.key.keysym.sym == SDLK_n) { //No Deal
                    dealAccepted = false;
                    quit = true;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_DestroyTexture(backgroundTexture);

        Uint32 currentTime = SDL_GetTicks();
        if (fadingOut) {
            alpha = static_cast<Uint8>(255 - (currentTime - fadeStartTime) / 8);
            if (alpha <= 50) {
                alpha = 0;
                fadingOut = false;
            }
        }

        //red fading overlay with the current alpha value
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, alpha);
        SDL_Rect Overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderFillRect(renderer, &Overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        if (!fadingOut) {
            renderText("The Banker is calling...", (SCREEN_WIDTH - 600) / 2, (SCREEN_HEIGHT - 50) / 2 - 50, { 255, 255, 255, 255 });
        }

        if (!offerRevealed && SDL_GetTicks() - startTime > 4000) {
            offerRevealed = true;
            bankOffer = calculateBankOffer(remainingAmounts);
        }

        if (offerRevealed) {
            string offerText = "Bank Offer: $" + to_string(bankOffer);
            renderText(offerText, (SCREEN_WIDTH - 500) / 2, (SCREEN_HEIGHT + 150) / 2 - 50, { 255, 255, 255, 255 });

            string promptText = "Press D to Deal, N to No Deal";
            renderText(promptText, (SCREEN_WIDTH - 700) / 2, (SCREEN_HEIGHT + 150) / 2 + 50, { 255, 255, 255, 255 });
        }
        SDL_RenderPresent(renderer);
    }

    if (dealAccepted) {
        bool winScreenQuit = false;
        while (!winScreenQuit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
                    winScreenQuit = true;
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            SDL_DestroyTexture(backgroundTexture);

            renderText("You win: " + to_string(bankOffer) + "$", (SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 50) / 2 - 50, { 255, 255, 255, 255 });
            renderText ("What about the case you have chosen in the beginning?", 50, (SCREEN_HEIGHT - 50) / 2 + 50, { 255, 255, 255, 255 });
            renderText ("Did you make the right choice?", 50, (SCREEN_HEIGHT - 50) / 2 + 100, { 255, 255, 255, 255 });
            renderText("Press any key to continue", (SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 50) / 2 + 150, { 255, 255, 255, 255 });
            SDL_RenderPresent(renderer);
        }
    }

    TTF_SetFontSize(font, 40);
}
