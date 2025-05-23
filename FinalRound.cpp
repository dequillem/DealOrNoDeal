#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include "finalround.h"
#include "base.h"
#include "const.h"
#include "casescene.h"
using namespace std;
// Hàm chuyển cảnh khi đến vòng cuối của chương trình
void finalRound(int playerCase, int lastRemainingCase, const vector<int>& caseAmounts, int& winningAmount, bool& returnToMainMenu) {
    bool decisionMade = false;
    bool keepCase = false;
    SDL_Event e;
    Mix_Music* thinkmusic = loadMusic("assets/ThinkAfterBankOffer.mp3");
    if (!thinkmusic) {
        cerr << "Failed to load case open sound effect!" << endl;
    }
    bool playingmusic = false;
    Uint32 voicelinesTimer = SDL_GetTicks();
    while (!decisionMade) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                decisionMade = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_k) { // keep the case
                    keepCase = true;
                    decisionMade = true;
                } else if (e.key.keysym.sym == SDLK_s) { //switch cases
                    keepCase = false;
                    decisionMade = true;
                }
            }
        }
        if (!playingmusic) {
            playMusic(thinkmusic);
            playingmusic = true;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_DestroyTexture(backgroundTexture);

        renderText("So we are at the final round, there are 2 cases left", 100, (SCREEN_HEIGHT - 50) / 2 - 50, { 255, 255, 255, 255 });
        if(SDL_GetTicks() - voicelinesTimer > 3000 ) {
            renderText("You have 2 options:", 100, (SCREEN_HEIGHT - 50) / 2, { 255, 255, 255, 255 });
        }
        if(SDL_GetTicks() - voicelinesTimer > 4000 ) {
            renderText("To keep your case, or to switch to the other one", 100, (SCREEN_HEIGHT - 50) / 2 + 50, { 255, 255, 255, 255 });
        }
        if(SDL_GetTicks() - voicelinesTimer > 5000 ) {
            renderText("Press K to keep, Press S to switch", 400, (SCREEN_HEIGHT - 50) / 2 + 100, { 255, 255, 255, 255 });
        }
        SDL_RenderPresent(renderer);
    }

    if (keepCase) {
        renderCaseScene(playerCase, caseAmounts[playerCase]);
        winningAmount = caseAmounts[playerCase];
    } else {
        renderCaseScene(lastRemainingCase, caseAmounts[lastRemainingCase]);
        winningAmount = caseAmounts[lastRemainingCase];
    }

    bool winScreenQuit = false;
    while (!winScreenQuit && !returnToMainMenu) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                winScreenQuit = true;
                returnToMainMenu = true;
            } else if (e.type == SDL_KEYDOWN) {
                winScreenQuit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_DestroyTexture(backgroundTexture);

        string winText = "You win: $" + to_string(winningAmount);
        renderText(winText, (SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 50) / 2 - 50, { 255, 255, 255, 255 });

        string promptText = "Press any key to exit";
        renderText(promptText, (SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 50) / 2 + 50, { 255, 255, 255, 255 });
        SDL_RenderPresent(renderer);
    }
    if (thinkmusic) Mix_FreeMusic(thinkmusic);
}







