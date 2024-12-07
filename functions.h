
#ifndef __FUNCTIONS__C__
#define __FUNCTIONS__C__

    typedef struct Color
    {
        int r;
        int g;
        int b;
        int a;
    } Color;

    typedef struct Coord
    {
        int l;
        int c;
    } Coord;


    void set_err(char *err);
    void destroy_all(SDL_Window *window, SDL_Renderer *render);
    void viderBuffer();
    void fps(int time);

#endif