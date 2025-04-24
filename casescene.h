#ifndef CASESCENE_H
#define CASESCENE_H
#include <string>
using namespace std;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int CASE_SIZE;
extern const int AMOUNT_MARGIN;

extern SDL_Texture* loadTexture(const string& filePath);
extern void renderText(const string& text, int x, int y, SDL_Color color);
void renderCaseScene(int chosenCase, int caseValue);
#endif // CASESCENE_H

