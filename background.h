#ifndef __BACKGROUND__C__
#define __BACKGROUND__C__

    #include <SDL.h>
    #include "functions.h"

    void showBackgroundColor(SDL_Renderer *render, Color *color);
    void createRectArea(char area[8][8][4], SDL_Rect rectArea[8][8], int padx, int pady);
    void drawArea(SDL_Renderer *render, SDL_Rect rectArea[8][8], Color colors[2]);
    void writeArea(char area[8][8][4]);

#endif