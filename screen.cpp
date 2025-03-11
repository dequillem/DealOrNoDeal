#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include "gamerules.h"

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
        exit(1);
    }

    if (TTF_Init() == -1) {
        cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << endl;
        exit(1);
    }

    window = SDL_CreateWindow("Deal or No Deal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(1);
    }

    font = TTF_OpenFont("assets/Montserrat-Bold.ttf", 30);
    if (font == nullptr) {
        cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << endl;
        exit(1);
    }
}

void closeSDL() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void renderText(const string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

SDL_Texture* loadTexture(const string& filePath) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (surface == nullptr) {
        cerr << "Failed to load image: " << SDL_GetError() << endl;
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        cerr << "Failed to create texture: " << SDL_GetError() << endl;
    }

    return texture;
}

void renderCases(SDL_Texture* caseTexture) {
    int startX = (SCREEN_WIDTH - (6 * (CASE_SIZE + CASE_MARGIN))) / 2;
    int startY = (SCREEN_HEIGHT - (4 * (CASE_SIZE + CASE_MARGIN))) / 2;

    for (int i = 0; i < 26; ++i) {
        int row = i / 6;
        int col = i % 6;
        int x = startX + col * (CASE_SIZE + CASE_MARGIN);
        int y = startY + row * (CASE_SIZE + CASE_MARGIN);

        SDL_Rect caseRect = { x, y, CASE_SIZE, CASE_SIZE };

        //case texture
        SDL_RenderCopy(renderer, caseTexture, nullptr, &caseRect);

        //case number
        renderText(to_string(i + 1), x + CASE_SIZE/4, y + CASE_SIZE/4, { 0, 0, 0, 200 });
    }
}
SDL_Texture* loadTexture(const string &file, SDL_Renderer *renderer) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = IMG_Load(file.c_str());
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return texture;
}
// case value on both sides
void renderAmounts() {
    int startX = AMOUNT_MARGIN;
    int startY = (SCREEN_HEIGHT - (amounts.size() / 2 * 30)) / 2;

    for (size_t i = 0; i < amounts.size(); ++i) {
        int x = startX;
        int y = startY + i * 30;
        if (i >= amounts.size() / 2) {
            x = SCREEN_WIDTH - AMOUNT_MARGIN - 150;
            y = startY + (i - amounts.size() / 2) * 30;
        }

        renderText("$" + to_string(amounts[i]), x, y, { 255, 255, 255, 255 });
    }
}

int main(int argc, char* argv[]) {
    initializeSDL();
    vector<int> caseAmounts = shuffleAmounts();
    for (int i=0; i<26; i++) {
        cout << caseAmounts[i] << " ";
    }
    SDL_Texture* imageTexture = loadTexture("assets/background.png", renderer);

    SDL_Texture* caseTexture = loadTexture("assets/case.png");
    if (caseTexture == nullptr) {
        cerr << "Failed to load case texture!" << endl;
        closeSDL();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, imageTexture, nullptr, nullptr);

        renderCases(caseTexture);
        renderAmounts();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(caseTexture);
    closeSDL();
    return 0;
}
