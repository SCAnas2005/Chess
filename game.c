
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <string.h>
#include <direct.h>

#include "functions.h"
#include "background.h"
#include "pieces.h"

int main(int argv, char **argc)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    // VARIABLES
    char *title = "Chess";
    int window_dimension[2] = {1080, 800}; 
    Color bg_color = {32, 33, 36, 0};
    Color white_color = {255, 255, 255, 0};
    Color blue_case = {75, 115, 153, 0};
    Color case_colors[2] = { white_color, blue_case };
    Coord rescueMoves[20];
    int rescueMovesNumber = 0;
    char turn = 'b';
    int checkMate = 0;
    int isPiece = 0;
    int time = 0;

    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    printf("Le répertoire de travail courant est : %s\n", cwd);

    char area[8][8][4] = {
                            {"nt1", "nc1", "nf1", "nq2", "nr1","nf2", "nc2", "nt2"},
                            {"np1", "np2", "np3", "np4", "np5", "np6", "np7", "np8"},
                            {"v", "v", "v", "v", "v", "v", "v", "v"},
                            {"v", "v", "v", "v", "v", "v", "v", "v"},
                            {"v", "v", "v", "v", "v", "v", "v", "v"},
                            {"v", "v", "v", "v", "v", "v", "v", "v"},
                            {"bp1", "bp2", "bp3", "bp4", "bp5", "bp6", "bp7", "bp8"},
                            {"bt1", "bc1", "bf1", "bq2", "br1","bf2", "bc2", "bt2"}
                        };

    Piece pieces[32]; // liste de 32 pieces au départ
    int piece_nb = 32; // nombre de pieces dans l'echequier

    char choosenPieceStr[4] = "aaa"; // pour console
    int p = 0; // piece choisis
    Coord pos = {0,0}; // coordonnée du click

    
    SDL_Rect rectArea[8][8];  createRectArea(area, rectArea, (window_dimension[0]-800)/2, 0);
    if (rectArea == NULL) { set_err("rect area null"); }

    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    window_dimension[0], window_dimension[1], 0);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (render == NULL || window == NULL) 
    { destroy_all(window, render); set_err("render or window creation"); }

    
    // GAME
    initAllPiece(area, pieces, render); // initialise toute les pieces dans pieces[32] (pos, nom...)
    getAllPiecesMoves(area, pieces, piece_nb); // calcule tout les mouvements possibles pour chaque piece

    showTurnColor(turn); 
    writeArea(area);

    getAllPiecesMoves(area, pieces, piece_nb);

    showPiecesInfo(pieces, piece_nb);

    // MAIN LOOP
    int started = 1;
    while (started)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            time = SDL_GetTicks();
            switch (event.type)
            {
                case SDL_QUIT:
                    started = 0;
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    // choix du mouvement après le choxi de la case
                    if (event.button.button == SDL_BUTTON_LEFT && isPiece == 1)
                    {
                        showPossibleMoves(&pieces[p]);
                        if (isCaseClick(rectArea, event.button.x, event.button.y, &pos))
                        {
                            if (isPiece == 0) {printf("piece deja choisis\n");}
                            if (checkMove(&pieces[p], &pos, &turn))
                            {
                                if (move(area, pieces, piece_nb, &pieces[p], &pos)) // on bouge la piece
                                {
                                    checkPionElevation(area, pieces, piece_nb, render);

                                    changeTurnColor(&turn); // on change de couleur

                                    clearOldMoves(pieces, piece_nb); // ON effaces les anciens mouvmeents
                                    getAllPiecesMoves(area, pieces, piece_nb); // on recalcule les coups

                                    //showPiecesInfo(pieces, piece_nb);
                                    writeArea(area);
                                    isPiece = 0;
                                    
                                    if (isCheckMate(area, pieces, piece_nb, turn))
                                    {
                                        printf("echec et maths !\n");
                                        started = 0;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    // Choix de la case
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (isCaseClick(rectArea, event.button.x, event.button.y, &pos) == 1)
                        {
                            isPiece = 0;
                            if (isPieceChoosen(pieces, piece_nb, &pos, &p) == 1)
                            {
                                //printf("Piece choisis : %s\n", pieces[p].name);
                                isPiece = 1;
                                if (pieces[p].name[1] == 'r')
                                {
                                    showPossibleMoves(&pieces[p]);
                                }
                            }
                        }
                    }
                    
                    break;

                default:
                    break;
            }
        }
        
        

        showBackgroundColor(render, &bg_color); // Affiche la couleur du background
        drawArea(render, rectArea, case_colors); // Affiche l'echequier
        drawAllPiece(render, pieces, piece_nb, rectArea); // affiche toute les pieces



        SDL_RenderPresent(render); // rafraichissement
        fps(time); // 60 frame par secondes
    }

    printf("Ends....\n");
    freeAllPieceTexture(pieces, piece_nb);
    destroy_all(window, render);
    SDL_Quit();

    return EXIT_SUCCESS;
}