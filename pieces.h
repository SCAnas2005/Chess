
#ifndef __PIECES__C__
    #define __PIECES__C__

    #include "functions.h"
    #include <SDL.h>

    typedef struct Piece
    {
        char name[4];
        Coord pos;
        Coord moves[28];
        int moves_nb;
        int isValid;

        SDL_Surface *surface;
        SDL_Texture *texture;
        SDL_Rect rect;
        
    } Piece;

    void askPieceName(char *choosenPiece);
    void askPieceMoves(Coord *pos);
    int caseIsEmpty(char area[8][8][4], int l, int c);
    void initAllPiece(char area[8][8][4], Piece pieces[32], SDL_Renderer *render);
    int move(char area[8][8][4], Piece pieces[32], int pieces_nb, Piece *piece, Coord *pos);
    void clearOldMoves(Piece pieces[32], int piece_nb);
    int getPossibleMoves(char area[8][8][4], Piece *piece);
    void getAllPiecesMoves(char area[8][8][4], Piece pieces[32], int piece_nb);
    void showPossibleMoves(Piece *piece);
    void showAllPiecesName(Piece pieces[32], int piece_nb);
    void showTurnColor(char turn);
    void changeTurnColor(char *turn);
    int getPiecePositionFromName(Piece pieces[32], int piece_nb, char *name);
    int checkMove(Piece *pieceActuel, Coord *pos, char *turn);
    int isKingCheck(char area[8][8][4], Piece *king, Piece pieces[32], int pieces_nb, int newL, int newC);
    void getAllSurfaces(Piece pieces[32], int pieces_nb);
    void drawAllPiece(SDL_Renderer *render, Piece pieces[32], int piece_nb, SDL_Rect rectArea[8][8]);
    int isCaseClick(SDL_Rect rectArea[8][8], int x, int y, Coord *pos);
    int isPieceChoosen(Piece pieces[32], int piece_nb, Coord *pos, int *pi);
    void drawPossibleMoves(SDL_Renderer *render, SDL_Rect rectArea[8][8], Piece *piece, int isPiece);
    int isClickOnPossibleMove(Piece *piece, Coord *pos);
    void freeAllPieceTexture(Piece pieces[32], int piece_nb);
    void killPiece(char area[8][8][4], Piece *piece, int l, int c);
    void showPiecesInfo(Piece pieces[32], int pieces_nb);
    int kingIsCheck(Piece pieces[32], int pieces_nb, char kingColor);
    int isCheckMate(char area[8][8][4], Piece pieces[32], int pieces_nb, char colorTurn);
    int checkPionElevation(char area[8][8][4], Piece pieces[32], int pieces_nb, SDL_Renderer *render);
    

#endif