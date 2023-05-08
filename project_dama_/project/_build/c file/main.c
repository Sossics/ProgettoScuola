#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define SQUARE_SIZE 80
#define BOARD_SIZE 8

typedef int BOARD;
typedef Texture2D OBJ_DAMA;
typedef int POSITION_X;
typedef int POSITION_Y;

typedef struct {
    POSITION_X x;
    POSITION_Y y;
    OBJ_DAMA texture;
}white;
white* PAWN_WHITE[12];

typedef struct {
    POSITION_X x;
    POSITION_Y y;
    OBJ_DAMA texture;
}nigga;
nigga* PAWN_BLACK[12];

typedef struct {
    BOARD mat[BOARD_SIZE][BOARD_SIZE];
    OBJ_DAMA Black_space;
    OBJ_DAMA White_space;
}BOARD_P;

BOARD_P BOARDP;

void drawBoard() {
    BOARDP.Black_space = 
        LoadTexture("img/scacchiera_black.png");
    BOARDP.White_space = 
        LoadTexture("img/scacchiera_white.png");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 0) {
                DrawTexture(BOARDP.White_space,
                    j * SQUARE_SIZE, i * SQUARE_SIZE
                    , WHITE
                );
            }
            else {
                DrawTexture(BOARDP.Black_space,
                    j * SQUARE_SIZE,
                    i * SQUARE_SIZE,
                    WHITE
                );
            }
        }
    }
}

void drawPawn() {

}

int main() {
    //iniziallizazione di tutte le varibili
    const int screenWidth = BOARD_SIZE * SQUARE_SIZE;
    const int screenHeight = BOARD_SIZE * SQUARE_SIZE;
    Color buttonColor = BLUE;
    Color textColor = WHITE;
    Rectangle buttonRec1 = { screenWidth / 2 - 75, screenHeight / 2 - 25, 150, 50 };
    Rectangle buttonRec2 = { screenWidth / 2 - 75, screenHeight / 2 + 50, 150, 50 };
    const char* buttonText1 = "BOT";
    const char* buttonText2 = "1 vs 1";
    const char* MSG_WORK = "TRUE";
    bool menu = true,
         bot = false,
         ovo = false;
    InitWindow(screenWidth, screenHeight, "Dama");
    //ToggleFullscreen();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (menu == true) {
            if (CheckCollisionPointRec(GetMousePosition(),
                buttonRec1) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
                ) {
                printf("%s\n", MSG_WORK);
                menu = false;
                bot = true;
            }

            if (CheckCollisionPointRec(GetMousePosition(),
                buttonRec2) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
                ) {
                printf("%s\n", MSG_WORK);
                menu = false;
                ovo = true;
            }
            DrawRectangleRec(buttonRec1, buttonColor);
            DrawRectangleRec(buttonRec2, buttonColor);
            DrawText(buttonText1, buttonRec1.x + buttonRec1.width / 2 - MeasureText(buttonText1, 20) / 2,
                buttonRec1.y + buttonRec1.height / 2 - 10, 20, textColor);
            DrawText(buttonText2, buttonRec2.x + buttonRec2.width / 2 - MeasureText(buttonText2, 20) / 2,
                buttonRec2.y + buttonRec2.height / 2 - 10, 20, textColor);
        }
        if (bot) {
            drawBoard();
        }
        if (ovo) {
            drawBoard();
        }

        EndDrawing();
        UnloadTexture(BOARDP.Black_space);
        UnloadTexture(BOARDP.White_space);

    }
    CloseWindow();

    return 0;
}