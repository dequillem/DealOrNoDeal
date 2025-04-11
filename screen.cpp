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
#include "greyout.h"
#include "const.h"

using namespace std;

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
        if (openedCases[i]) continue;

        int row = i / 6;
        int col = i % 6;
        int x = startX + col * (CASE_SIZE + CASE_MARGIN);
        int y = startY + row * (CASE_SIZE + CASE_MARGIN);

        SDL_Rect caseRect = {x, y, CASE_SIZE, CASE_SIZE};
        SDL_RenderCopy(renderer, caseTexture, nullptr, &caseRect);
        renderText(to_string(i + 1), x + CASE_SIZE / 4, y + CASE_SIZE / 4, {0, 0, 0, 200});
    }
}

void renderAmounts() {
    int startX = AMOUNT_MARGIN;
    int startY = (SCREEN_HEIGHT - (amounts.size() / 2 * 30)) / 2;

    for (int i = 0; i < (int)amounts.size(); ++i) {
        int x = startX;
        int y = startY + i * 30;
        if (i >= (int)amounts.size() / 2) {
            x = SCREEN_WIDTH - AMOUNT_MARGIN - 200;
            y = startY + (i - amounts.size() / 2) * 30;
        }

        renderText("$" + to_string(amounts[i]), x, y, { 255, 255, 255, 255 });
    }
}

void showGameOverScreen(int winningAmount, bool& returnToMainMenu) {
    SDL_Event e;
    bool done = false;

    while (!done) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                done = true;
                returnToMainMenu = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    done = true;
                    returnToMainMenu = true;
                } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    done = true;
                    returnToMainMenu = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_DestroyTexture(backgroundTexture);

        renderText("Game Over!", 300, (SCREEN_HEIGHT - 100) / 2, {255, 255, 255, 255});
        renderText("You won: $" + to_string(winningAmount), 300, (SCREEN_HEIGHT) / 2, {255, 255, 255, 255});
        renderText("Press SPACE to return to main menu", 300, (SCREEN_HEIGHT + 100) / 2, {255, 255, 255, 255});
        renderText("Press ESC to quit", 300, (SCREEN_HEIGHT + 200) / 2, {255, 255, 255, 255});

        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* argv[]) {
    initializeSDL();

    bool running = true;

    while (running) {
        bool inMainMenu = true;
        bool inGame = false;
        string playerName = "";

        renderMainMenu(renderer, font, "assets/background.png");

        // Main menu loop
        while (inMainMenu) {
            SDL_Event event;
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    inMainMenu = false;
                    running = false;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        inMainMenu = false;
                        inGame = true;
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        inMainMenu = false;
                        running = false;
                    }
                }
            }
        }

        if (inGame) {
            playerName = getname(renderer, font, "assets/background.png");
            cout << "Player name: " << playerName << endl;

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
            bool gameFinished = false;
            int round = 1;
            int elimcases = roundcases(round);
            int winningAmount = 0;
            bool returnToMainMenu = false;

            while (!gameFinished && running) {
                SDL_Event event;
                while (SDL_PollEvent(&event) != 0) {
                    if (event.type == SDL_QUIT) {
                        gameFinished = true;
                        running = false;
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

                                    if (elimcases == 0) {
                                        vector<int> remainingAmounts;
                                        for (int j = 0; j < 26; ++j) {
                                            if (!openedCases[j]) {
                                                remainingAmounts.push_back(caseAmounts[j]);
                                            }
                                        }

                                        bool dealAccepted = false;
                                        renderBankerScene(remainingAmounts, dealAccepted);

                                        if (dealAccepted) {
                                            renderCaseScene(playerCase, caseAmounts[playerCase]);
                                            winningAmount = calculateBankOffer(remainingAmounts);
                                            cout << "Deal accepted! You win: $" << winningAmount << endl;
                                            gameFinished = true;
                                        }
                                        else {
                                            cout << "No deal! Proceeding to the next round." << endl;
                                            round++;
                                            elimcases = roundcases(round);
                                        }
                                    }
                                }
                                if (round == 10) {
                                    int lastRemainingCase = -1;
                                    for (int j = 0; j < 26; ++j) {
                                        if (!openedCases[j] && j != playerCase) {
                                            lastRemainingCase = j;
                                            break;
                                        }
                                    }

                                    finalRound(playerCase, lastRemainingCase, caseAmounts, winningAmount, returnToMainMenu);
                                    cout << "You win: $" << winningAmount << endl;
                                    gameFinished = true;
                                }
                                break;
                            }
                        }
                    }
                }

                if (!gameFinished) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    SDL_Texture* backgroundTexture = loadTexture("assets/background.png");
                    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
                    SDL_DestroyTexture(backgroundTexture);

                    renderCases(caseTexture, caseAmounts, openedCases);
                    renderOpenedCases(caseAmounts, openedCases);

                    renderText("Player: " + playerName, 20, 20, { 255, 255, 255, 255 });
                    if (playerCase != -1) {
                        renderText("Your case: " + to_string(playerCase + 1), 20, 60, { 255, 255, 255, 255 });
                    }

                    vector<int> remainingAmounts;
                    for (int i = 0; i < 26; ++i) {
                        if (!openedCases[i]) {
                            remainingAmounts.push_back(caseAmounts[i]);
                        }
                    }
                    vector<int> openedAmounts;
                    for (int i = 0; i < 26; ++i) {
                        if (openedCases[i]) {
                            openedAmounts.push_back(caseAmounts[i]);
                        }
                    }
                    int playerCaseValue = (playerCase != -1) ? caseAmounts[playerCase] : -1;
                    renderGreyedAmounts(openedAmounts, playerCaseValue);

                    SDL_RenderPresent(renderer);
                }
            }

            SDL_DestroyTexture(caseTexture);

            if (!returnToMainMenu && gameFinished) {
                showGameOverScreen(winningAmount, returnToMainMenu);
            }
        }
    }

    closeSDL();
    return 0;
}

