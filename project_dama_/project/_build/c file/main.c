#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define BOARD_SIZE 8
#define SQUARE_SIZE 64

typedef struct {
    struct board {
        int mat[BOARD_SIZE][BOARD_SIZE];
        int x;
        int y;
        Texture2D white_place;
        Texture2D black_place;
    } board;
    struct white_pawn_ {
        int mat[BOARD_SIZE][BOARD_SIZE];
        int x;
        int y;
        Texture2D white_pawn;
    } white_pawn_;
    struct black_pawn_ {
        int mat[BOARD_SIZE][BOARD_SIZE];
        int x;
        int y;
        Texture2D black_pawn;
    } black_pawn_;
    struct game{
        bool game_over;
        bool game_run;
        bool game_1v1;
        bool game_bot;
        bool Game_menu;
    };
} game;

typedef struct {
    Rectangle rect;
    const char* text;
    int fontSize;
    Color color;
    Color textColor;
    bool clicked;
} Button;

// Funzione per disegnare un bottone
void DrawButton(Button button) {
    DrawRectangleRec(button.rect, button.color);
    DrawText(button.text, button.rect.x + button.fontSize / 2, button.rect.y + button.fontSize / 2, button.fontSize, button.textColor);
}

// Funzione per creare un bottone
Button CreateButton(float x, float y, float width, float height, const char* text, int fontSize, Color color, Color textColor) {
    Button button;
    button.rect.x = x;
    button.rect.y = y;
    button.rect.width = width;
    button.rect.height = height;
    button.text = text;
    button.fontSize = fontSize;
    button.color = color;
    button.textColor = textColor;
    button.clicked = false;
    return button;
}

game GAME;

void loadTexture() {
    GAME.black_pawn_.black_pawn = LoadTexture("img/pedinanera-removebg-preview.png");
    GAME.white_pawn_.white_pawn = LoadTexture("img/pedinabianca-removebg-preview.png");
    GAME.board.black_place = LoadTexture("img/scacchiera_black.png");
    GAME.board.white_place = LoadTexture("img/scacchiera_white.png");
}

void drawBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            Vector2 pos = { col * SQUARE_SIZE, row * SQUARE_SIZE };

            if ((row + col) % 2 == 0) {
                DrawTextureEx(GAME.board.white_place, pos, 0.0f, 1.0f, WHITE);
            }
            else {
                DrawTextureEx(GAME.board.black_place, pos, 0.0f, 1.0f, WHITE);
            }

            if (GAME.white_pawn_.mat[row][col]) {
                DrawTextureEx(GAME.white_pawn_.white_pawn, pos, 0.0f, 1.0f, WHITE);
            }

            if (GAME.black_pawn_.mat[row][col]) {
                DrawTextureEx(GAME.black_pawn_.black_pawn, pos, 0.0f, 1.0f, WHITE);
            }
        }
    }
}

int main() {
    const int screenWidth = BOARD_SIZE * SQUARE_SIZE;
    const int screenHeight = BOARD_SIZE * SQUARE_SIZE;

    InitWindow(screenWidth, screenHeight, "Chess");

    SetTargetFPS(60);

    loadTexture();

    Button vsBotButton = CreateButton(100,
        100,
        200, 
        50, 
        "vs Bot", 
        20, 
        GRAY, 
        BLACK
    );
    Button oneVsOneButton = CreateButton(100, 
        200, 
        200, 
        50, 
        "1v1", 
        20, 
        GRAY, 
        BLACK
    );
    GAME.Game_menu = true;
    GAME.game_1v1 = false;
    GAME.game_bot = false;
    GAME.game_run = true;
    GAME.game_over = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (GAME.Game_menu == true) {
            DrawButton(vsBotButton);
            DrawButton(oneVsOneButton);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, vsBotButton.rect)) {
                    drawBoard();
                    GAME.game_bot = true;
                    GAME.Game_menu = false;
                }
                if (CheckCollisionPointRec(mousePos, oneVsOneButton.rect)) {
                    drawBoard();
                    GAME.game_bot = true;
                    GAME.Game_menu = false;
                }
            }
        }
        if (GAME.game_1v1 == true) {
            drawBoard();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
