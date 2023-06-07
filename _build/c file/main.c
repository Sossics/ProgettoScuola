#include "raylib.h"
#include "raymath.h"
#include "../.h file/functions.h"
#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 8
#define SQUARE_SIZE 64
#define GIALLASTRO_MARRONCINO CLITERAL(Color){ 230, 180, 100, 255 }
#define GIALLO_MARRONCINO_CHIARO CLITERAL(Color){ 245, 220, 170, 255 }


FILE* fp;

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
        Texture2D white_pawn_TEXTURE;
    } white_pawn_;
    struct black_pawn_ {
        int mat[BOARD_SIZE][BOARD_SIZE];
        int x;
        int y;
        Texture2D black_pawn_TEXTURE;
    } black_pawn_;
    struct game{
        bool game_over;
        bool game_run;
        bool game_1v1;
        bool game_bot;
        bool Game_menu;
    };
    struct point {
        int numberwhite;
        int numberblack;
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

int xIndexResult_startPosition = -1, yIndexResult_startPosition = -1, xIndexResult_endPosition = -1, yIndexResult_endPosition = -1, giocatore=1, mossa_f=0;

// Funzione per disegnare un bottone
void DrawButton(Button button) {
    DrawRectangleRec(button.rect, button.color);
    DrawText(button.text, button.rect.x + button.fontSize / 2, button.rect.y + button.fontSize / 2, button.fontSize, button.textColor);
}

int GetIndexesFromPosition(Rectangle buttonRect, int squareSize, int *xIndex, int *yIndex) {
    *xIndex = ((int)(buttonRect.x / squareSize)) - 1;
    *yIndex = ((int)(buttonRect.y / squareSize)) - 1;
}

int checkMouseClick(MouseButton button, int lineOfCode) {
    fprintf(fp, "\n[LINE:%d] InFunction", lineOfCode);
    int isClicked = IsMouseButtonDown(button);
    if (isClicked) {
        fprintf(fp, "\n\tReceived a button click.");
       
       fprintf(fp, "\n\tWaiting for the button to be released. (status: %d)", IsMouseButtonDown(button));
        
        
        return 1;
    }
     fprintf(fp, "\n\tButton has been released(status: %d)", IsMouseButtonDown(button));
     return 0;
  
}

void checkIfAnyCellIsPressed(Vector2 mousePosition, int squareSize, int *flag, Rectangle *rectangleResult) {
    int row, col, x, y;
    for (row = 0; row < BOARD_SIZE; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            x = (col + 1) * squareSize;
            y = (row + 1) * squareSize;
            Rectangle buttonRect = { x, y, squareSize, squareSize };
            if (CheckCollisionPointRec(mousePosition, buttonRect)) {
                printf("\nYou pressed a cell...");
                *flag = 1;
                *rectangleResult =  buttonRect;
            }
        }
    }
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
    
}

void DrawWhitePawn(int x, int y, int size) {
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 2 - size / 10; 
    int innerRadius = radius - size / 20;

    DrawCircle(centerX, centerY, radius, WHITE);
    DrawCircle(centerX, centerY, innerRadius, LIGHTGRAY);
    DrawCircle(centerX, centerY, innerRadius / 2, WHITE);
}

void DrawBlackPawn(int x, int y, int size) {
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 2 - size / 10; 
    int innerRadius = radius - size / 20; 

    DrawCircle(centerX, centerY, radius, BLACK);
    DrawCircle(centerX, centerY, innerRadius, GRAY); 
    DrawCircle(centerX, centerY, innerRadius / 2, BLACK); 
}

void DrawWhiteSpecialPawn(int x, int y, int size) {
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 2 - size / 10; 
    int innerRadius = radius - size / 20; 

    DrawCircle(centerX, centerY, radius, WHITE); 
    DrawCircle(centerX, centerY, innerRadius, LIGHTGRAY); 
    DrawCircle(centerX, centerY, innerRadius / 2, WHITE);
    DrawCircle(centerX, centerY, innerRadius / 3, YELLOW);
}

void DrawBlackSpecialPawn(int x, int y, int size) {
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 2 - size / 10;
    int innerRadius = radius - size / 20;

    DrawCircle(centerX, centerY, radius, BLACK);
    DrawCircle(centerX, centerY, innerRadius, GRAY);
    DrawCircle(centerX, centerY, innerRadius / 2, BLACK); 
    DrawCircle(centerX, centerY, innerRadius / 3, YELLOW);
}
/*
void getSelectedPawn(MouseButton button, Rectangle _rectangle, int sq_size, int *xPos, int *yPos) {

    while(true){
        fprintf(fp,"\nWaiting a click..");
        if (IsMouseButtonPressed(button)) {
            GetIndexesFromPosition(_rectangle, sq_size, &xPos, &yPos);
            fprintf(fp,"\nGot Position: [%d; %d]", *xPos, *yPos);
            break;
        }
    }

}*/

void victoryScreen();

void pawnsCounter() {
    GAME.numberwhite = 0;
    GAME.numberblack = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (GAME.board.mat[i][j] == 2 || GAME.board.mat[i][j] == 5) {
                GAME.numberblack++;
            }
            if (GAME.board.mat[i][j] == 1 || GAME.board.mat[i][j] == 4) {
                GAME.numberwhite++;
            }
        }
    }
    char whiteCountStr[10];
    sprintf(whiteCountStr, "%d", GAME.numberwhite);
    DrawText(whiteCountStr, 1200, 100, 200, BLACK);
    char blackCountStr[10];
    sprintf(blackCountStr, "%d", GAME.numberblack);
    DrawText(blackCountStr, 1200, 600, 200, BLACK);
}

bool ifcolorcondition = false;
bool stallovero = false;

void drawBoard() {
    int boardWidth = BOARD_SIZE * SQUARE_SIZE;
    int boardHeight = BOARD_SIZE * SQUARE_SIZE;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int squareSize = (int)(screenHeight / (BOARD_SIZE + 2));
    int flagValidMove = 1;
    int flagClick_left = 0;
    int flagClick_right = 0;
    int aquiredPositions = 0;
    int verify = 0;
    Color clikButton;

    if (ifcolorcondition == false) {
        clikButton = LIGHTGRAY;
    }
    else {
        clikButton = RED;
    }

    int x, y, flagRight=0, flagLeft=0, flagFunctionResult=0;
    Rectangle rectangleResult;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            x = (col + 1) * squareSize;
            y = (row + 1) * squareSize;
            if ((row + col) % 2 == 0) {
                DrawRectangle(x, y, squareSize, squareSize, GIALLO_MARRONCINO_CHIARO);
            }
            else {
                Rectangle buttonRect = { x, y, squareSize, squareSize };
                int buttonValue = GAME.board.mat[row][col];


                if (CheckCollisionPointRec(GetMousePosition(), buttonRect) )
                {
                    DrawRectangleRec(buttonRect, clikButton);
                    if (checkMouseClick(MOUSE_LEFT_BUTTON, __LINE__)) {
                        ifcolorcondition = true;
                        if (xIndexResult_startPosition == -1 && yIndexResult_startPosition == -1 && xIndexResult_endPosition == -1 && yIndexResult_endPosition == -1) {
                            GetIndexesFromPosition(buttonRect, squareSize, &xIndexResult_startPosition, &yIndexResult_startPosition);
                            printf("\n\tAquired start position[%d, %d]", yIndexResult_startPosition, xIndexResult_startPosition);
                        }
                        else if (xIndexResult_startPosition != -1 && yIndexResult_startPosition != -1 && xIndexResult_endPosition == -1 && yIndexResult_endPosition == -1 && xIndexResult_startPosition != xIndexResult_endPosition && yIndexResult_startPosition != yIndexResult_endPosition) {
                            GetIndexesFromPosition(buttonRect, squareSize, &xIndexResult_endPosition, &yIndexResult_endPosition);
                            printf("\n\tAquired end position[%d, %d]", yIndexResult_endPosition, xIndexResult_endPosition);
                        }
                    }
                }
                else {
                    DrawRectangleRec(buttonRect, DARKBROWN);
                }

                if (xIndexResult_startPosition != -1 && 
                    yIndexResult_startPosition != -1 && 
                    xIndexResult_endPosition != -1 && 
                    yIndexResult_endPosition != -1 && 
                    xIndexResult_startPosition == xIndexResult_endPosition && 
                    yIndexResult_startPosition == yIndexResult_endPosition) {
                    xIndexResult_endPosition = -1;
                    yIndexResult_endPosition = -1;
                }
                else if (xIndexResult_startPosition != -1 && 
                    yIndexResult_startPosition != -1 && 
                    xIndexResult_endPosition != -1 && 
                    yIndexResult_endPosition != -1 && giocatore ==1){
                    printf("\ngiocatore: %d", giocatore);
                    mossa_f = aggiorna_p(GAME.board.mat, yIndexResult_startPosition, xIndexResult_startPosition, yIndexResult_endPosition, xIndexResult_endPosition, 1);
                    if( mossa_f == 0){
                        xIndexResult_startPosition = -1;
                        yIndexResult_startPosition = -1;
                        xIndexResult_endPosition = -1;
                        yIndexResult_endPosition = -1;
                        giocatore = 0;
                    }
                    else {
                        xIndexResult_startPosition = -1;
                        yIndexResult_startPosition = -1;
                        xIndexResult_endPosition = -1;
                        yIndexResult_endPosition = -1;
                    }
                }
                else if (xIndexResult_startPosition != -1 &&
                    yIndexResult_startPosition != -1 &&
                    xIndexResult_endPosition != -1 &&
                    yIndexResult_endPosition != -1 && giocatore == 0 &&
                    GAME.game_1v1 == true
                    ) {
                    printf("\ngiocatore: %d", giocatore);
                    mossa_f = aggiorna_p(GAME.board.mat, yIndexResult_startPosition, xIndexResult_startPosition, yIndexResult_endPosition, xIndexResult_endPosition, 2);
                    if (mossa_f == 0) {
                        xIndexResult_startPosition = -1;
                        yIndexResult_startPosition = -1;
                        xIndexResult_endPosition = -1;
                        yIndexResult_endPosition = -1;
                        giocatore = 1;
                    }
                    else {
                        xIndexResult_startPosition = -1;
                        yIndexResult_startPosition = -1;
                        xIndexResult_endPosition = -1;
                        yIndexResult_endPosition = -1;
                    }
                }
                else if (giocatore==0 && GAME.game_bot == true) {
                    giocatore = 1;
                    verify = aggiorna_b(GAME.board.mat, 2);
                    if (verify != 0) {
                        victoryScreen();
                        stallovero = true;
                    }
                    xIndexResult_startPosition = -1;
                    yIndexResult_startPosition = -1;
                    xIndexResult_endPosition = -1;
                    yIndexResult_endPosition = -1;
                }

                Button button = CreateButton(x, y, squareSize, squareSize, "", 20, BLANK, WHITE);
                button.buttonValue = buttonValue;
                DrawButton(button);
            } 

            if (GAME.board.mat[row][col] == 1) {
                int pawnX = x + (int)squareSize/2 - SQUARE_SIZE + (int)SQUARE_SIZE/4;
                int pawnY = y + (int)squareSize/2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4;
                int pawnSize = squareSize;

                DrawWhitePawn(pawnX, pawnY, pawnSize);
            }
            else if (GAME.board.mat[row][col] == 2) {
                int pawnX = x + (int)squareSize/2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4; 
                int pawnY = y + (int)squareSize/2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4; 
                int pawnSize = (int)squareSize;

                DrawBlackPawn(pawnX, pawnY, pawnSize);
            }
            else if (GAME.board.mat[row][col] == 4) {
                int pawnX = x + (int)squareSize / 2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4; 
                int pawnY = y + (int)squareSize / 2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4; 
                int pawnSize = (int)squareSize; 
                DrawWhiteSpecialPawn(pawnX, pawnY, pawnSize);
            }
            else if (GAME.board.mat[row][col] == 5) {
                int pawnX = x + (int)squareSize / 2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4;
                int pawnY = y + (int)squareSize / 2 - SQUARE_SIZE + (int)SQUARE_SIZE / 4; 
                int pawnSize = (int)squareSize; 

                DrawBlackSpecialPawn(pawnX, pawnY, pawnSize);
            }
        }
    }
}

void victoryScreen() {
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    int selButtonsWidth = 500,
        selButtonsHeight = 90;
    Button next = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 600, selButtonsWidth, selButtonsHeight, "MENU", 50, GRAY, BLACK);
    DrawText("VICTORY", (int)(screenWidth - selButtonsWidth) / 3, 200, 200, WHITE);
    DrawButton(next);
    if (CheckCollisionPointRec(GetMousePosition(), next.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        GAME.game_bot = false;
        GAME.Game_menu = true;
        stallovero = false;
    }
}

void loseScreen() {
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    int selButtonsWidth = 500,
        selButtonsHeight = 90;
    Button next = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 600, selButtonsWidth, selButtonsHeight, "MENU", 50, GRAY, BLACK);
    DrawText("LOSER", (int)(screenWidth - selButtonsWidth) / 3, 200, 200, WHITE);
    DrawButton(next);
    if (CheckCollisionPointRec(GetMousePosition(), next.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        GAME.game_bot = false;
        GAME.Game_menu = true;
    }
}

void blackVictoryScreen() {
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    int selButtonsWidth = 500,
        selButtonsHeight = 90;
    Button next = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 600, selButtonsWidth, selButtonsHeight, "MENU", 50, GRAY, BLACK);
    DrawText("BLACK WIN", (int)(screenWidth - selButtonsWidth) / 3, 200, 200, WHITE);
    DrawButton(next);
    if (CheckCollisionPointRec(GetMousePosition(), next.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        GAME.game_1v1 = false;
        GAME.Game_menu = true;
    }
}

void whiteVictoryScreen() {
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    int selButtonsWidth = 500,
        selButtonsHeight = 90;
    Button next = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 600, selButtonsWidth, selButtonsHeight, "MENU", 50, GRAY, BLACK);
    DrawText("WHITE WIN", (int)(screenWidth - selButtonsWidth) / 3, 200, 200, WHITE);
    DrawButton(next);
    if (CheckCollisionPointRec(GetMousePosition(), next.rect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        GAME.game_1v1 = false;
        GAME.Game_menu = true;
    }
}



int main() {
    
    

    fp = fopen("log.txt", "w+");
    if (fp == NULL)
    {
        printf("Error opening the file %s", "log.txt");
        return -1;
    }

    InitWindow(0, 0, "dama");
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    int selButtonsWidth = 500,
        selButtonsHeight = 90;

    printf("\n*****WIDTH: %d, HEIGHT %d\n", screenWidth, screenHeight);
  
    //ToggleFullscreen();
    GAME.numberblack = 0;
    GAME.numberwhite = 0;

    SetTargetFPS(60);

    loadTexture();

    Button vsBotButton = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 400, selButtonsWidth, selButtonsHeight, "Player vs Bot", 50, GRAY, BLACK);
    Button oneVsOneButton = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 600, selButtonsWidth, selButtonsHeight, "Player vs Player", 50, GRAY, BLACK);
    Button quit = CreateButton((int)(screenWidth - selButtonsWidth) / 2, 800, selButtonsWidth, selButtonsHeight, "QUIT", 50, GRAY, BLACK);
    Button reset = CreateButton(1500, 1000, 100, 50, "EXIT", 20, GRAY, BLACK);

    Image background = LoadImage("img/men.png");
    Texture2D backgroundTexture = LoadTextureFromImage(background);
    Image background2 = LoadImage("img/gioco.png");
    Texture2D backgroundTexture2 = LoadTextureFromImage(background2);

    GAME.Game_menu = true;
    GAME.game_1v1 = false;
    GAME.game_bot = false;
    GAME.game_run = true;
    GAME.game_over = false;
    int defaultPawn = 0;
    InitAudioDevice();
    int lose1 = 1;
    int lose2 = 1;
    Music ambientSound = LoadMusicStream("sound/ambient.mp3");
    Sound eatSound = LoadSound("sound/eatsound.mp3");
    PlayMusicStream(ambientSound);
    printf("\nBOARD DIM: %dx%d", BOARD_SIZE * SQUARE_SIZE, BOARD_SIZE * SQUARE_SIZE);
    int temp = 0;
    int temp2 = 0;
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        UpdateMusicStream(ambientSound);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        //DrawLineEx((Vector2) { screenWidth / 2, 0 }, (Vector2) { screenWidth / 2,screenHeight }, 3.0, BLUE);
        //DrawLineEx((Vector2) { 0, screenHeight / 2 }, (Vector2) { screenWidth, screenHeight / 2 }, 3.0, BLUE);
            if (GAME.Game_menu == true) {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        defaultPawn = 0;
                        if ((i + j) % 2 != 0) {
                            if (i >= 0 && i <= 2) {
                                defaultPawn = 1;
                            }
                            else if (i >= 5 && i <= 7) {
                                defaultPawn = 2;
                            }
                        }
                        GAME.board.mat[i][j] = defaultPawn;
                    }
                }
                ClearBackground(RAYWHITE);
                DrawTexture(backgroundTexture, 0, 0, WHITE);
                DrawButton(vsBotButton);
                DrawButton(oneVsOneButton);
                DrawButton(quit);
                DrawText("DAMA", (int)(screenWidth - selButtonsWidth) / 2, 150, 180, GRAY);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    fprintf(fp, "\n[LINE: %d] ENTER FLAGS: pressed -> %d released -> %d", __LINE__, IsMouseButtonDown(MOUSE_LEFT_BUTTON), IsMouseButtonUp(MOUSE_LEFT_BUTTON));
                    //checkMouseClick(MOUSE_LEFT_BUTTON, __LINE__);
                    printf("\nU pressed left button");
                    //printf("\nx:%.2f y:%.2f | w:%.2f h:%.2f", vsBotButton.rect.x, vsBotButton.rect.y, vsBotButton.rect.width, vsBotButton.rect.height);
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
                    if (CheckCollisionPointRec(mousePos, quit.rect)) {
                        ClearBackground(RAYWHITE);
                        DrawText("ARRIVEDERCI", 100, 100, NULL, BLACK);
                        break;
                    }
                }
            }
        if (GAME.game_bot == true) {
            ClearBackground(RAYWHITE);
            if (stallovero == true) {
                victoryScreen();
            }
            else {
                DrawTexture(backgroundTexture2, 0, 0, WHITE);
                lose1 = count_pieces(1, GAME.board.mat);
                lose2 = count_pieces(2, GAME.board.mat);
                if (lose1 != 0 && lose2 != 0) {
                    drawBoard();
                    DrawButton(reset);
                    pawnsCounter();
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mousePos, reset.rect)) {
                            GAME.game_bot = false;
                            GAME.Game_menu = true;
                        }
                    }
                }
                else {
                    if (lose1 == 0) {
                        loseScreen();
                    }
                    else {
                        victoryScreen();
                    }

                }
            }
        }
        if (GAME.game_1v1 == true) {
            ClearBackground(RAYWHITE);
            DrawTexture(backgroundTexture2, 0, 0, WHITE);
            lose1 = count_pieces(1, GAME.board.mat);
            lose2 = count_pieces(2, GAME.board.mat);
            if (lose1 != 0 && lose2 != 0) {
                drawBoard();
                DrawButton(reset);
                pawnsCounter();
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (CheckCollisionPointRec(mousePos, reset.rect)) {
                        GAME.game_1v1 = false;
                        GAME.Game_menu = true;
                    }
                }
            }
            else {
                if (lose1 == 0) {
                    blackVictoryScreen();
                }
                else {
                    whiteVictoryScreen();
                }
            }
        }
        EndDrawing();
        if (temp > lose1 || temp2 > lose2)
        {
            PlaySound(eatSound); 
        }

        temp = lose1;
        temp2 = lose2;
    }
    UnloadSound(eatSound);
    CloseWindow();

    return 0;
}
