
#include <stdio.h>
#include <SDL.h>
#include "functions.h"

void set_err(char *err)
{
    printf("Err : %s\n", err);
    exit(1);
}

void destroy_all(SDL_Window *window, SDL_Renderer *render)
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}

void viderBuffer()
{
    int c = getchar();
    while (c != EOF && c != '\n')
    {
        c = getchar();
    }
}


void fps(int time)
{
    if (SDL_GetTicks64() - time <= 60)
    {
        SDL_Delay(SDL_GetTicks64() - time);
    }
}
