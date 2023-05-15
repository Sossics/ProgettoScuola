#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define BOARD_SIZE 8
#define SQUARE_SIZE 64
#define GIALLASTRO_MARRONCINO CLITERAL(Color){ 230, 180, 100, 255 }
#define GIALLO_MARRONCINO_CHIARO CLITERAL(Color){ 245, 220, 170, 255 }   // Giallo Marroncino Chiaro

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
    int buttonValue;
    int mat[8][8];
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

void DrawWhitePawn(int x, int y, int size) {
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 2 - size / 10; // Dimensione del cerchio esterno
    int innerRadius = radius - size / 20; // Dimensione del cerchio interno

    DrawCircle(centerX, centerY, radius, WHITE); // Cerchio esterno bianco
    DrawCircle(centerX, centerY, innerRadius, LIGHTGRAY); // Cerchio interno grigio chiaro
    DrawCircle(centerX, centerY, innerRadius / 2, WHITE); // Cerchio centrale bianco
}

void DrawBlackPawn(int x, int y, int size) {
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 2 - size / 10; // Dimensione del cerchio esterno
    int innerRadius = radius - size / 20; // Dimensione del cerchio interno

    DrawCircle(centerX, centerY, radius, BLACK); // Cerchio esterno nero
    DrawCircle(centerX, centerY, innerRadius, GRAY); // Cerchio interno grigio
    DrawCircle(centerX, centerY, innerRadius / 2, BLACK); // Cerchio centrale nero
}

void drawBoard() {
    int boardWidth = BOARD_SIZE * SQUARE_SIZE;
    int boardHeight = BOARD_SIZE * SQUARE_SIZE;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int squareSize = screenHeight / (BOARD_SIZE + 2);

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int x = (col + 1) * squareSize;
            int y = (row + 1) * squareSize;
            if ((row + col) % 2 == 0) {
                DrawRectangle(x, y, squareSize, squareSize, GIALLO_MARRONCINO_CHIARO);
            }
            else {
                Rectangle buttonRect = { x, y, squareSize, squareSize };
                int buttonValue = GAME.board.mat[row][col];  // Ottieni il valore corrispondente nella matrice

                if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
                {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        GAME.board.mat[row][col] = buttonValue;
                        
                    }
                    DrawRectangleRec(buttonRect, LIGHTGRAY);
                }
                else {
                    DrawRectangleRec(buttonRect, DARKBROWN);
                }

                Button button = CreateButton(x, y, squareSize, squareSize, "", 20, BLANK, WHITE);
                button.buttonValue = buttonValue;
                DrawButton(button);
            }

            if (GAME.board.mat[row][col] == 1) {
                int pawnX = x + squareSize / 2 - SQUARE_SIZE + SQUARE_SIZE/4;
                int pawnY = y + squareSize / 2 - SQUARE_SIZE + SQUARE_SIZE /4;
                int pawnSize = squareSize;

                DrawWhitePawn(pawnX, pawnY, pawnSize);
            }
            else if (GAME.board.mat[row][col] == 2) {
                int pawnX = x + squareSize / 2 - SQUARE_SIZE + SQUARE_SIZE / 4; // Coordinata x per il centro del rettangolo
                int pawnY = y + squareSize / 2 - SQUARE_SIZE + SQUARE_SIZE / 4; // Coordinata y per il centro del rettangolo
                int pawnSize = squareSize; // Dimensione della pedina (dimensione del rettangolo)

                DrawBlackPawn(pawnX, pawnY, pawnSize);
            }
        }
    }
}


int main() {
    const int screenWidth = BOARD_SIZE * SQUARE_SIZE;
    const int screenHeight = BOARD_SIZE * SQUARE_SIZE;

    InitWindow(screenWidth, screenHeight, "dama");
    
    int monitor = GetCurrentMonitor(),
        monitorWidth = GetMonitorWidth(monitor),
        monitorHeight = GetMonitorHeight(monitor);

    SetWindowSize(monitorWidth, monitorHeight);
    ToggleFullscreen();

    SetTargetFPS(60);

    loadTexture();

    Button vsBotButton = CreateButton(100,100,200, 50, "vs Bot", 20, GRAY, BLACK);
    Button oneVsOneButton = CreateButton(100, 200, 200, 50, "1v1", 20, GRAY, BLACK);


    Image background = LoadImage("img/background.png");
    Texture2D backgroundTexture = LoadTextureFromImage(background);

    GAME.Game_menu = true;
    GAME.game_1v1 = false;
    GAME.game_bot = false;
    GAME.game_run = true;
    GAME.game_over = false;

    for (int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++){
            GAME.board.mat[i][j] = 0;
        }
    }

    GAME.board.mat[0][1] = 1;
    GAME.board.mat[0][3] = 1;
    GAME.board.mat[0][5] = 1;
    GAME.board.mat[0][7] = 1;
    GAME.board.mat[1][0] = 1;
    GAME.board.mat[1][2] = 1;
    GAME.board.mat[1][4] = 1;
    GAME.board.mat[1][6] = 1;
    GAME.board.mat[2][1] = 1;
    GAME.board.mat[2][3] = 1;
    GAME.board.mat[2][5] = 1;
    GAME.board.mat[2][7] = 1;

    GAME.board.mat[5][0] = 2;
    GAME.board.mat[5][2] = 2;
    GAME.board.mat[5][4] = 2;
    GAME.board.mat[5][6] = 2;
    GAME.board.mat[6][1] = 2;
    GAME.board.mat[6][3] = 2;
    GAME.board.mat[6][5] = 2;
    GAME.board.mat[6][7] = 2;
    GAME.board.mat[7][0] = 2;
    GAME.board.mat[7][2] = 2;
    GAME.board.mat[7][4] = 2;
    GAME.board.mat[7][6] = 2;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        if (GAME.Game_menu == true) {
            ClearBackground(RAYWHITE);
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
                    GAME.game_1v1 = true;
                    GAME.Game_menu = false;
                }
            }
        }
        if (GAME.game_bot == true) {
            drawBoard();

        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
