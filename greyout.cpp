#include "greyout.h"
#include "base.h"
#include "const.h"
using namespace std;

void renderGreyedAmounts(const vector<int>& openedAmounts, int playerCaseValue) {
    int startX = AMOUNT_MARGIN;
    int startY = (SCREEN_HEIGHT - (amounts.size() / 2 * 30)) / 2;

    for (int i = 0; i < (int)amounts.size(); ++i) {
        bool isOpened = find(openedAmounts.begin(), openedAmounts.end(), amounts[i]) != openedAmounts.end();
        bool isPlayerCase = (amounts[i] == playerCaseValue);

        int x = startX;
        int y = startY + i * 30;
        if (i >= (int)amounts.size() / 2) {
            x = SCREEN_WIDTH - AMOUNT_MARGIN - 200;
            y = startY + (i - amounts.size() / 2) * 30;
        }

        SDL_Color color;
        if (isPlayerCase) {
            color = {255, 255, 255, 255};
        } else {
            color = isOpened ? SDL_Color{100, 100, 100, 255} : SDL_Color{255, 255, 255, 255};
        }

        renderText("$" + to_string(amounts[i]), x, y, color);
    }
}

void renderOpenedCases(const vector<int>& caseAmounts, const vector<bool>& openedCases) {
    int startX = (SCREEN_WIDTH - (6 * (CASE_SIZE + CASE_MARGIN))) / 2;
    int startY = (SCREEN_HEIGHT - (4 * (CASE_SIZE + CASE_MARGIN))) / 2;

    for (int i = 0; i < 26; ++i) {
        if (!openedCases[i]) continue;

        int row = i / 6;
        int col = i % 6;
        int x = startX + col * (CASE_SIZE + CASE_MARGIN);
        int y = startY + row * (CASE_SIZE + CASE_MARGIN);

        SDL_Rect caseRect = {x, y, CASE_SIZE, CASE_SIZE};
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &caseRect);

        renderText(to_string(i + 1), x + CASE_SIZE / 4, y + CASE_SIZE / 4, {200, 200, 200, 255});
        TTF_SetFontSize(font, 40);
    }
}
