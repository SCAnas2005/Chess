
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>


#include "pieces.h"
#include "functions.h"

// PROTOTYPES ===================================================================================================
void askPieceName(char *choosenPiece);
void askPieceMoves(Coord *pos);

int isOut(int l, int c);
int caseIsEmpty(char area[8][8][4], int l, int c);
int isEnemy(char area[8][8][4], char *piece, int l, int c);

int isWhite(char *piece);
int isBlack(char *piece);
int isPion(char *piece);
int isKing(char *piece);
int isQueen(char *piece);
int isRock(char *piece);
int isKnight(char *piece);
int isBishop(char *piece);

int kingNearKing(char area[8][8][4] , Piece king, int L, int C);
void getKingPossibleMoves(char area[8][8][4], Piece pieces[32], int piece_nb,Piece *king);

int getPiecePositionFromName(Piece pieces[32], int piece_nb, char *name);
int getPiecePositionFromPos(Piece pieces[32], int piece_nb, Coord *pos);

int checkPosFromMoves(Piece *piece, Coord pos);
int addMove(Piece *piece, int l, int c);
int getPossibleMoves(char area[8][8][4], Piece *piece);
void getAllPiecesMoves(char area[8][8][4], Piece pieces[32], int piece_nb);
void clearOldMoves(Piece pieces[32], int piece_nb);
int checkMove(Piece *pieceActuel, Coord *pos, char *turn);
int move(char area[8][8][4], Piece pieces[32], int pieces_nb, Piece *piece, Coord *pos);

void changeTurnColor(char *turn);
void showTurnColor(char turn);

void getSurface(Piece *piece);
void initPiece(Piece *piece, char *name, Coord position);
void initTextures(Piece *piece, SDL_Renderer *render);
void initAllPiece(char area[8][8][4], Piece pieces[32], SDL_Renderer *render);

int isCaseClick(SDL_Rect rectArea[8][8], int x, int y, Coord *pos);
int isPieceChoosen(Piece pieces[32], int piece_nb, Coord *pos, int *pi);

void showPossibleMoves(Piece *piece);
void showAllPiecesName(Piece pieces[32], int piece_nb);

void drawPiece(SDL_Renderer *render, Piece *piece, SDL_Rect *rect);
void drawAllPiece(SDL_Renderer *render, Piece pieces[32], int piece_nb, SDL_Rect rectArea[8][8]);

int kingIsCheck(Piece pieces[32], int pieces_nb, char kingColor);
// ================================================================================================================




// FONCTIONS ===================================================================================================


void askPieceName(char *choosenPiece)
{
    printf("Piece : "); fgets(choosenPiece, 3+1, stdin); printf("\n");
    printf("[DEBUG] piece = %s\n", choosenPiece);
    printf("-------------------------------------\n");
}

void askPieceMoves(Coord *pos)
{
    printf("Ligne : "); scanf("%d", &pos->l); 
    printf("Colonne : "); scanf("%d", &pos->c);
    printf("[DEBUG] L, C = %d, %d\n", pos->l, pos->c);
    printf("-------------------------------------\n");
}

int isOut(int l, int c)
{
    return (l < 0 || l > 7) || (c < 0 || c > 7);
}

int caseIsEmpty(char area[8][8][4], int l, int c)
{
    return !isOut(l, c) && area[l][c][0] == 'v';
}

int isEnemy(char area[8][8][4], char *piece, int l, int c)
{
    if (caseIsEmpty(area, l, c)) { return 0; } // Si la case est vide, y a pas d'ennemie
    if (isWhite(piece)) // Si la piece est blanche,
    {
        return area[l][c][0] == 'n'; // Vrai sil y a un ennemie noir
    }
    return area[l][c][0] == 'b'; // Si l'ennemie est noir, retourne vrai sil y a un ennmie blanc
}


int isWhite(char *piece)
{
    return piece[0] == 'b';
}

int isBlack(char *piece)
{
    return piece[0] == 'n';
}

int isPion(char *piece)
{
    return piece[1] == 'p';
}

int isKing(char *piece)
{   
    return piece[1] == 'r';
}

int isQueen(char *piece)
{
    return piece[1] == 'q';
}

int isRock(char *piece)
{
    return piece[1] == 't';
}

int isKnight(char *piece)
{
    return piece[1] == 'c';
}

int isBishop(char *piece)
{
    return piece[1] == 'f';
}

int kingNearKing(char area[8][8][4] , Piece king, int L, int C)
{   
    // fonctions pour savoir si un roi est proche d'un roi (interdis au echecs)
    int i = 1;
    int newL;
    int newC;
    int Lv = -1, Cv = -1;
    while (i <= 8)
    {
        newL = L+Lv;
        newC = C+Cv;
        if (isEnemy(area, king.name, newL, newC))
        {
            if (isKing(area[newL][newC])) 
            {
                return 1;
            }
        }

        if (Cv == 1)
        {
            Cv = -1; // Case au debut
            Lv += 1; // on augmente d'une case
        }
        else {Cv++;} // case suivante

        if (Lv == 0 && Cv == 0) { Cv++; } // Si Lv et Cv sont sur le roi, alors on skip la case
        i++; // On augmente de 1 le compteur (il y a 8 coups possible pour un roi)
    }
    return 0; // pas de roi au alentours
}

void getKingPossibleMoves(char area[8][8][4], Piece pieces[32], int piece_nb, Piece *king)
{
    int i = 1;
    int Lv, Cv;
    int newL, newC;


    Lv = -1; Cv = -1; // Vecteur de position (case en haut a gauche du roi au debut)
    while (i <= 8)
    {
        newL = king->pos.l+Lv;
        newC = king->pos.c+Cv;
        if ((caseIsEmpty(area, newL, newC) || isEnemy(area, king->name, newL, newC)) && !kingNearKing(area, *king, newL, newC))
        {
            addMove(king, newL, newC);
        }

        if (Cv == 1)
        {
            Cv = -1; // Case au debut
            Lv += 1; // on augmente d'une case
        }
        else {Cv++;} // case suivante

        if (Lv == 0 && Cv == 0) { Cv++; } // Si Lv et Cv sont sur le roi, alors on skip la case
        i++; // On augmente de 1 le compteur (il y a 8 coups possible pour un roi)
    }
}



int getPiecePositionFromName(Piece pieces[32], int piece_nb, char *name)
{
    for (int i = 0; i < piece_nb; i++)
    {
        if (strncmp(pieces[i].name, name, 3) == 0)
        {
            return i;
        }
    }
    printf("Erreur, piece non trouve !\n");
    return 0;
}

int getPiecePositionFromPos(Piece pieces[32], int piece_nb, Coord *pos)
{
    for (int p = 0; p < piece_nb; p++)
    {
        if (pieces[p].isValid == 1)
        {
            if (pieces[p].pos.l == pos->l && pieces[p].pos.c == pos->c) 
            {
                return p;
            }
        }
    }
    return -1;
}


int checkPosFromMoves(Piece *piece, Coord pos)
{
    for (int i = 0; i < piece->moves_nb; i++)
    {
        if (piece->moves[i].l == pos.l && piece->moves[i].c == pos.c)
        {
            printf("Position trouve !!\n");
            return 1;
        }
    }
    return 0;
}

int addMove(Piece *piece, int l, int c)
{
    piece->moves[piece->moves_nb].l = l;
    piece->moves[piece->moves_nb].c = c;
    piece->moves_nb++;
}

int getPossibleMoves(char area[8][8][4], Piece *piece)
{
    if (piece->isValid == 0) { return 0; }

    int Lv = -1, Cv = 0;
    int newL, newC;
    if (isPion(piece->name))
    {
        if (isWhite(piece->name))
        {
            if (caseIsEmpty(area, piece->pos.l-1, piece->pos.c+0))
            {
                if (piece->pos.l == 6)
                {
                    if (caseIsEmpty(area, piece->pos.l-2, piece->pos.c+0)) // Si dans 2 cases c vide, on peux avancer
                        addMove(piece, piece->pos.l-2, piece->pos.c); // 2 case après le pion
                }
                addMove(piece, piece->pos.l-1, piece->pos.c); // 1 case après le pion
            }
            if (isEnemy(area, piece->name, piece->pos.l-1, piece->pos.c-1) ){
                // Si il y a un ennemie en haut a gauche, on peut le manger
                addMove(piece, piece->pos.l-1, piece->pos.c-1); // 1 case a droite 
            }
            if (isEnemy(area, piece->name, piece->pos.l-1, piece->pos.c+1))
            {
                // Sil y a un ennemie en haut a droite, on peut le manger
                addMove(piece, piece->pos.l-1, piece->pos.c+1);
            }
        }
        else{
            if (caseIsEmpty(area, piece->pos.l+1, piece->pos.c+0))
            {
                if (piece->pos.l == 1)
                {
                    if (caseIsEmpty(area, piece->pos.l+2, piece->pos.c+0))
                    {
                        // Si les deux cases devant le pion sont vides, on peut avancer
                        addMove(piece, piece->pos.l+2, piece->pos.c+0);
                    }
                }
                addMove(piece, piece->pos.l+1, piece->pos.c);
            }
            if (isEnemy(area, piece->name, piece->pos.l+1, piece->pos.c+1) ){
                // Si il y a un ennemie en haut a gauche, on peut le manger
                addMove(piece, piece->pos.l+1, piece->pos.c+1); // 1 case a droite 
            }
            if (isEnemy(area, piece->name, piece->pos.l+1, piece->pos.c-1))
            {
                // Sil y a un ennemie en haut a droite, on peut le manger
                addMove(piece, piece->pos.l+1, piece->pos.c-1);
            }
        }
    }
    else if (isQueen(piece->name))
    {
        Lv = -1; Cv = -1; // debut en haut a gauche de la piece
        int i = 1; 
        while (i <= 8) // 8 chemins
        {
            newL = piece->pos.l + Lv;
            newC = piece->pos.c + Cv;

            while (caseIsEmpty(area, newL, newC))
            {
                addMove(piece, newL, newC);
                newL += Lv;
                newC += Cv;
            }
            if (isEnemy(area, piece->name, newL, newC)){ // si c un ennemie
                addMove(piece, newL, newC); // on ajoute
            }
            if (Cv == 1)
            {
                Cv = -1; // retourne a gauche
                Lv++; // ligne suivante
            }
            else{ Cv++; } // sinon case suivante
                
            if (Cv == 0 && Lv == 0) { Cv++; } // on skippe si on est sur le roi
            i++; // chemin suivant
        }
    }
    else if (isRock(piece->name))
    {
        int i = 1;
        Lv = -1; Cv = 0; // en haut
        while (i <= 4)
        {
            newL = piece->pos.l + Lv;
            newC = piece->pos.c + Cv;

            while (caseIsEmpty(area, newL, newC))
            {
                addMove(piece, newL, newC);
                newL += Lv;
                newC += Cv;
            }
            if (isEnemy(area, piece->name, newL, newC)){ // si c un ennemie
                addMove(piece, newL, newC); // on ajoute
            }

            Lv = -Lv;
            Cv = -Cv;

            i++;
            if (i == 3)
            {
                Lv = 0;
                Cv = -1;
            }
        }
    }

    else if (isKnight(piece->name))
    {
        Lv = -2; Cv = -1;
        int i = 1;
        while ( i <= 8 ) // 8 coups possibles
        {
            newL = piece->pos.l + Lv;
            newC = piece->pos.c + Cv;

            if ((caseIsEmpty(area, newL, newC) || isEnemy(area, piece->name, newL, newC)))
            {
                addMove(piece, newL, newC);
            }

            if (i % 2 == 0) // on change de case 2 a 2
            {
                Lv = -Lv;
            }
            else { Cv = -Cv; }

            i++;
            if (i == 5) // on fait les autres case a l'horizontal
            {
                Cv = -2;
                Lv = -1;
            }
        }
    }

    else if (isBishop(piece->name))
    {
        Lv = -1; Cv = -1; // en haut a gauche
        int i = 1;
        
        while (i <= 4) // 4 chemins possibles pour le fou
        {   
            newL = piece->pos.l + Lv;
            newC = piece->pos.c + Cv;

            while (caseIsEmpty(area, newL, newC))
            {
                addMove(piece, newL, newC);
                newL += Lv;
                newC += Cv;
            }
            if (isEnemy(area, piece->name, newL, newC)){ // si c un ennemie
                addMove(piece, newL, newC); // on ajoute
            }

            Lv = -Lv; // on change de diagonale (bas ou haut)
            i++;
            if (i == 3) { Cv = -Cv; } // on change de diagonale (droite ou gauche)
        }
    }
    return 1;

}

void getAllPiecesMoves(char area[8][8][4], Piece pieces[32], int piece_nb)
{
    for (int i = 0; i < piece_nb; i++)
    {
        if (isKing(pieces[i].name))
        {
            getKingPossibleMoves(area, pieces, piece_nb, &pieces[i]);
        }
        else{
            getPossibleMoves(area, &pieces[i]);
        }
    }  
}

void clearOldMoves(Piece pieces[32], int piece_nb)
{
    for (int i = 0; i < piece_nb; i++)
    {
        pieces[i].moves_nb = 0;
    }
}

int checkMove(Piece *pieceActuel, Coord *pos, char *turn)
{
    if (*turn == 'b') // si le tour est blanc et qu'on choisis un piece noir, on renvoie faux
    {
        if (isBlack(pieceActuel->name)) 
            return 0; 
    }
    else if (*turn == 'n') { // contraire
        if (isWhite(pieceActuel->name)) 
            return 0; 
    }

    if (checkPosFromMoves(pieceActuel, *pos) == 1)
    {
        return 1;
    }

    return 0;
}



int move(char area[8][8][4], Piece pieces[32], int pieces_nb, Piece *piece, Coord *pos)
{
    int newLig = pos->l;
    int newCol = pos->c;
    int old_l = piece->pos.l;
    int old_c = piece->pos.c;
    int isPieceKilled = 0;

    char old_piece_name[4]; strcpy(old_piece_name, area[newLig][newCol]);

    Coord posPieceToKill = {newLig, newCol};
    int p = getPiecePositionFromPos(pieces, pieces_nb, &posPieceToKill);

    if (!caseIsEmpty(area, newLig, newCol))
    {   
        isPieceKilled = 1;
        pieces[p].isValid = 0;
        printf("%s isValid = %d\n", pieces[p].name, pieces[p].isValid);
    }

    strcpy(area[newLig][newCol], piece->name);
    strcpy(area[old_l][old_c], "v");

    piece->pos.l = newLig; 
    piece->pos.c = newCol;

    clearOldMoves(pieces, pieces_nb);
    getAllPiecesMoves(area, pieces, pieces_nb);

    if (kingIsCheck(pieces, pieces_nb, piece->name[0]))
    {
        printf("Cant move, king is check !\n");

        strcpy(area[newLig][newCol], old_piece_name);
        strcpy(area[old_l][old_c], piece->name);

        if (isPieceKilled == 1)
        {
            pieces[p].isValid = 1;
        }

        piece->pos.l = old_l; 
        piece->pos.c = old_c;

        return 0;
    }


    return 1;
}

void changeTurnColor(char *turn)
{
    if (*turn == 'b') // si le tour était celui des blancs, on passe au noir
    {
        *turn = 'n';
    }
    else{
        *turn = 'b'; // sinon au blanc
    }
}

void showTurnColor(char turn)
{
    if (turn == 'b')
    {
        printf("[Au tour des BLANCS]\n");
    }
    else {
        printf("[Au tour des NOIRS]\n");
    }
}


void getSurface(Piece *piece)
{
    SDL_Surface *image = NULL;
    char filename[30] = "../pieces/"; // nombre de caractères pour un nom de fichier 20
    strcat(filename, piece->name);
    filename[strlen(filename)-1] = 0; // Supprime le numero de la pièce.
    strcat(filename, ".png");
    printf("Filename : %s\n", filename);
    image = IMG_Load(filename);
    if (image == NULL) { set_err("err creation surface getSurface"); }
    piece->surface = image;
}

void initPiece(Piece *piece, char *name, Coord position)
{
    strncpy(piece->name, name, 4); // création du nom
    piece->pos.l = position.l; // initialisation  
    piece->pos.c = position.c; // des coordonnées
    piece->moves_nb = 0; // aucun mouvement au debut
    piece->isValid = 1; // valide au debut
}

void initTextures(Piece *piece, SDL_Renderer *render)
{
    getSurface(piece);
    piece->texture = SDL_CreateTextureFromSurface(render, piece->surface);
    SDL_FreeSurface(piece->surface);
    if (piece->texture == NULL) { set_err("Err creation texture");}
}

void initAllPiece(char area[8][8][4], Piece pieces[32], SDL_Renderer *render)
{
    int nb_piece = 0;
    Coord pos;
    for (int l = 0; l < 8; l++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (area[l][c][0] != 'v') // si case pas vide
            {
                pos.l = l;
                pos.c = c;
                initPiece(&pieces[nb_piece], area[l][c], pos); // on initialiser les pieces
                initTextures(&pieces[nb_piece], render);
                nb_piece++;
            }
        }
    }
}

int isCaseClick(SDL_Rect rectArea[8][8], int x, int y, Coord *pos)
{
    for (int l = 0; l < 8; l++)
    {
        for (int c = 0; c < 8; c++)
        {
            if ((x >=  rectArea[l][c].x && x <= rectArea[l][c].x+rectArea[l][c].w)
            && (y >=  rectArea[l][c].y && y <= rectArea[l][c].y+rectArea[l][c].h))
            {
                pos->l = l;
                pos->c = c;
                return 1;
            }
        }
    }
    return 0;
}

int Intersect(Coord *pos, Piece *piece)
{
    if (pos->l == piece->pos.l && pos->c == piece->pos.c)
        return 1;
    return 0;
}

int isPieceChoosen(Piece pieces[32], int piece_nb, Coord *pos, int *pi)
{
    for (int p = 0; p < piece_nb; p++)
    {
        if (pieces[p].isValid == 1)
        {
            if (Intersect(pos, &pieces[p]))
            {
                *pi = p;
                return 1;
            }
        }
    }
    return 0;
}



void showPossibleMoves(Piece *piece)
{   
    printf("==== %s (%d coups possibles )====\n\n", piece->name, piece->moves_nb);
    for (int i = 0; i < piece->moves_nb; i++)
    {
        printf("%d : (%d, %d)\n", i+1, piece->moves[i].l, piece->moves[i].c);
    }
}

void showAllPiecesName(Piece pieces[32], int piece_nb)
{
    for (int i = 0; i < piece_nb; i++)
    {
        printf("%s : %d\n", pieces[i].name, pieces[i].moves_nb);
    }
}

void showPiecesInfo(Piece pieces[32], int pieces_nb)
{
    for (int p = 0; p < pieces_nb; p++)
    {
        printf("%s (%d) : (%d, %d) | %d mouvements possibles\n", pieces[p].name, pieces[p].isValid, 
        pieces[p].pos.l, pieces[p].pos.c, pieces[p].moves_nb);
    }
}

void drawPiece(SDL_Renderer *render, Piece *piece, SDL_Rect *rect)
{
    SDL_RenderCopy(render, piece->texture, NULL, rect); // affichage de la piece
}

void drawAllPiece(SDL_Renderer *render, Piece pieces[32], int piece_nb, SDL_Rect rectArea[8][8])
{
    SDL_Rect rect;
    for (int p = 0; p < piece_nb; p++)
    {
        if (pieces[p].isValid == 1)
            drawPiece(render, &pieces[p], &rectArea[pieces[p].pos.l][pieces[p].pos.c]);
    }
}



int isClickOnPossibleMove(Piece *piece, Coord *pos)
{
    if (checkPosFromMoves(piece, *pos))
    {
        return 1;
    }
    return 0;
}

void freeAllPieceTexture(Piece pieces[32], int piece_nb)
{
    for (int i = 0; i < piece_nb; i++)
    {
        SDL_DestroyTexture(pieces[i].texture);
    }
}


int isNewMoveCheck(Piece pieces[32], int pieces_nb, Piece *king)
{
    for (int i = 0; i < pieces_nb; i++)
    {
        if (pieces[i].name[0] != king->name[0])
        {
            if (checkPosFromMoves(&pieces[i], king->pos))
            {
                return 1;
            }
        }
    }
    return 0;
}

int kingIsCheck(Piece pieces[32], int pieces_nb, char kingColor)
{
    int posKing;
    char kingName[4];

    if (kingColor == 'b')
    {
        strcpy(kingName, "br1");
    }
    else { strcpy(kingName, "nr1"); }

    posKing = getPiecePositionFromName(pieces, pieces_nb, kingName);

    for (int p = 0; p < pieces_nb; p++)
    {
        if (kingColor != pieces[p].name[0])
        {
            if (checkPosFromMoves(&pieces[p], pieces[posKing].pos) == 1){
                return 1;
            }
        }
    }
    return 0;
}

int isCheckMate(char area[8][8][4], Piece pieces[32], int pieces_nb, char colorTurn)
{
    int newLig;
    int newCol;
    int old_l;
    int old_c;
    int isPieceKilled = 0;
    int checkMate = 1;
    Coord posPieceToKill;
    char old_piece_name[4];
    int posKill;
    
    char kingName[4];
    if (colorTurn == 'b')
    {
        strcpy(kingName, "br1");
    } 
    else if (colorTurn == 'n')
    {
        strcpy(kingName, "nr1");
    }

    int kingPos = getPiecePositionFromName(pieces, pieces_nb, kingName);

    for (int p = 0; p < pieces_nb; p++)
    {
        if (colorTurn == pieces[p].name[0])
        {
            for (int m = 0; m < pieces[p].moves_nb; m++)
            {
                printf("%s : %d, %d\n", pieces[p].name, pieces[p].moves[m].l, pieces[p].moves[m].c);
                newLig = pieces[p].moves[m].l;
                newCol = pieces[p].moves[m].c;

                old_l = pieces[p].pos.l;
                old_c = pieces[p].pos.c;

                isPieceKilled = 0;
                posPieceToKill.l = newLig; posPieceToKill.c = newCol;
                old_piece_name[4]; strcpy(old_piece_name, area[newLig][newCol]);

                posKill = getPiecePositionFromPos(pieces, pieces_nb, &posPieceToKill);

                if (!caseIsEmpty(area, newLig, newCol))
                {   
                    isPieceKilled = 1;
                    pieces[posKill].isValid = 0;
                }

                strcpy(area[newLig][newCol], pieces[p].name);
                strcpy(area[old_l][old_c], "v");

                pieces[p].pos.l = newLig; 
                pieces[p].pos.c = newCol;

                clearOldMoves(pieces, pieces_nb);
                getAllPiecesMoves(area, pieces, pieces_nb);

                if (kingIsCheck(pieces, pieces_nb, colorTurn) == 0)
                {
                    checkMate = 0;
                }

                strcpy(area[newLig][newCol], old_piece_name);
                strcpy(area[old_l][old_c], pieces[p].name);

                if (isPieceKilled == 1)
                {
                    pieces[posKill].isValid = 1;
                }

                pieces[p].pos.l = old_l; 
                pieces[p].pos.c = old_c;

                clearOldMoves(pieces, pieces_nb);
                getAllPiecesMoves(area, pieces, pieces_nb);

                if (checkMate == 0)
                {
                    printf("not check mate\n");
                    return 0;
                }
            }
        }
    }
    return 1;
}

void changeTexture(Piece *piece, SDL_Renderer *render)
{
    getSurface(piece);
    initTextures(piece, render);
}

int checkPionElevation(char area[8][8][4], Piece pieces[32], int pieces_nb, SDL_Renderer *render)
{
    for (int p = 0; p < pieces_nb; p++)
    {
        if (isPion(pieces[p].name))
        {
            if (isWhite(pieces[p].name))
            {
                if (pieces[p].pos.l == 0)
                {
                    strcpy(pieces[p].name, "bq3");
                    strcpy(area[pieces[p].pos.l][pieces[p].pos.c], "bq3");
                    changeTexture(&pieces[p], render);
                    return 1;
                }
            }
            else
            {
                if (pieces[p].pos.l == 7)
                {
                    strcpy(pieces[p].name, "nq3");
                    strcpy(area[pieces[p].pos.l][pieces[p].pos.c], "nq3");
                    changeTexture(&pieces[p], render);
                    return 1;
                }
            }
        }
    }
    return 0;
}