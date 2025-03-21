#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include "gamerules.h"
#include "casescene.h"
#include "mainmenu.h"
#include "banker.h"
#include "finalround.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CASE_SIZE = 80;
const int CASE_MARGIN = 20;
const int AMOUNT_MARGIN = 20;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

const vector<int> amounts = {
    0, 1, 5, 10, 25, 50, 75, 100, 200, 300, 400, 500, 750,
    1000, 5000, 10000, 25000, 50000, 75000, 100000, 200000, 300000,
    400000, 500000, 750000, 1000000
};

void initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    }

    if (TTF_Init() == -1) {
        cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << endl;
    }

    window = SDL_CreateWindow("Deal or No Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
    }

    font = TTF_OpenFont("assets/Montserrat-Bold.ttf", 40);
    if (font == nullptr) {
        cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << endl;
    }
}

void closeSDL() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void renderCases(SDL_Texture* caseTexture, const vector<int>& caseAmounts, const vector<bool>& openedCases) {
    int startX = (SCREEN_WIDTH - (6 * (CASE_SIZE + CASE_MARGIN))) / 2;
    int startY = (SCREEN_HEIGHT - (4 * (CASE_SIZE + CASE_MARGIN))) / 2;

    for (int i = 0; i < 26; ++i) {
        int row = i / 6;
        int col = i % 6;
        int x = startX + col * (CASE_SIZE + CASE_MARGIN);
        int y = startY + row * (CASE_SIZE + CASE_MARGIN);

        SDL_Rect caseRect = { x, y, CASE_SIZE, CASE_SIZE };

        // Render the case texture only if it's not opened
        if (!openedCases[i]) {
            SDL_RenderCopy(renderer, caseTexture, nullptr, &caseRect);
            // Render the case number
            renderText(to_string(i + 1), x + CASE_SIZE / 4, y + CASE_SIZE / 4, { 0, 0, 0, 200 });
        }
    }
}

void renderAmounts() {
    int startX = AMOUNT_MARGIN;
    int startY = (SCREEN_HEIGHT - (amounts.size() / 2 * 30)) / 2;

    for (int i = 0; i < (int)amounts.size(); ++i) {
        int x = startX;
        int y = startY + i * 30;
        if (i >= (int)amounts.size() / 2) {
            x = SCREEN_WIDTH - AMOUNT_MARGIN - 150;
            y = startY + (i - amounts.size() / 2) * 30;
        }

        renderText("$" + to_string(amounts[i]), x, y, { 255, 255, 255, 255 });
    }
}

int main(int argc, char* argv[]) {
    initializeSDL();

    vector<int> caseAmounts = shuffleAmounts();
    vector<bool> openedCases(26, false);
    int playerCase = -1;

    SDL_Texture* caseTexture = loadTexture("assets/case.png");
    if (caseTexture == nullptr) {
        cerr << "Failed to load case texture!" << endl;
        closeSDL();
        return 1;
    }

    // Game loop
    bool quit = false;
    int round = 1;
    int elimcases = roundcases(round);

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                int startX = (SCREEN_WIDTH - (6 * (CASE_SIZE + CASE_MARGIN))) / 2;
                int startY = (SCREEN_HEIGHT - (4 * (CASE_SIZE + CASE_MARGIN))) / 2;

                for (int i = 0; i < 26; ++i) {
                    int row = i / 6;
                    int col = i % 6;
                    int x = startX + col * (CASE_SIZE + CASE_MARGIN);
                    int y = startY + row * (CASE_SIZE + CASE_MARGIN);

                    if (mouseX >= x && mouseX <= x + CASE_SIZE && mouseY >= y && mouseY <= y + CASE_SIZE) {
                        if (playerCase == -1) {
                            playerCase = i;
                            openedCases[i] = true;
                            cout << "Your case is: " << (i + 1) << endl;
                        }
                        else if (!openedCases[i] && elimcases > 0) {
                            cout << "Round " << round << "! Remove " << elimcases << " cases." << endl;
                            renderCaseScene(i, caseAmounts[i]);
                            openedCases[i] = true;
                            elimcases--;
                            cout << "Case " << (i + 1) << " removed. Value: $" << caseAmounts[i] << endl;
                            break;
                        }
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            SDL_DestroyTexture(backgroundTexture);

            renderCases(caseTexture, caseAmounts, openedCases);

            if (playerCase != -1) {
                renderText("Your case: " + to_string(playerCase + 1), 20, 60, { 255, 255, 255, 255 });
            }

            vector<int> remainingAmounts;
            for (int i = 0; i < 26; ++i) {
                if (!openedCases[i]) {
                    remainingAmounts.push_back(caseAmounts[i]);
                }
            }
            renderAmounts();

            SDL_RenderPresent(renderer);
        }
    SDL_DestroyTexture(caseTexture);
    closeSDL();
    return 0;
}

