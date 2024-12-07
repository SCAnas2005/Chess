
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <SDL.h>
#include "functions.h"
#include "background.h"

void createRectArea(char area[8][8][4], SDL_Rect rectArea[8][8], int padx, int pady)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            rectArea[i][j].x = j*100 + padx;
            rectArea[i][j].y = i*100 + pady;
            rectArea[i][j].w = 100;
            rectArea[i][j].h = 100;
        }
    }
}

void showBackgroundColor(SDL_Renderer *render, Color *color)
{
    SDL_SetRenderDrawColor(render, color->r, color->g, color->b, color->a);
    SDL_RenderClear(render);
}


void drawArea(SDL_Renderer *render, SDL_Rect rectArea[8][8], Color colors[])
{
    int n = 0;
    Color currentColor;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            currentColor = colors[n % 2];
            SDL_SetRenderDrawColor(render, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
            SDL_RenderFillRect(render, &rectArea[i][j]);
            n++;
        }
        n++;
    }
}


void writeArea(char area[8][8][4])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (!strcmp(area[i][j], "v"))
            {
                printf("___ ");
            }
            else 
                printf("%s ", area[i][j]);
        }
        printf("\n");
    }
}