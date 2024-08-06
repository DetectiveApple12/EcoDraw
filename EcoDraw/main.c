#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <string.h>

#define MAX_STACK 500000

void RedrawTexture(RenderTexture2D* target);
void AddDrawnItem(Vector2 firstPoint, Vector2 secondPoint, int strokeSize, Color color);

const int screenWidth = 1200;
const int screenHeight = 800;


const Color green = { 89, 136, 92, 255 };
const Color brown = { 142, 111, 101, 255 };
const Color orangered = { 164, 61, 28, 255 };
const Color blue = { 101, 169, 223, 255 };
const Color white = { 255, 255, 254, 255 };
const Color black = { 0, 0, 0, 255 };
const Color backgroundColor = { 223, 217, 215, 255 };
const Color pink = { 173, 20, 87, 255 };

typedef struct Drawn {
    Vector2 firstPoint;
    Vector2 secondPoint;
    int strokeSize;
    Color color;
} Drawn;

Drawn stack[MAX_STACK] = { { 0 } };
int currentIndex = 0;

void clearStack() {
    for (int i = 0; i < MAX_STACK; i++) {
        stack[i].strokeSize = 0;
    }
}

void saveFile() {
    char path[1000] = { 0 };
    printf("Enter file path: ");
    fgets(path, 999, stdin);
    path[strcspn(path, "\n")] = 0;
    FILE* file = fopen(path, "w");
    if (file == NULL) return;
    for (int i = 0; i < MAX_STACK; i++) {
        if (stack[i].strokeSize != 0) {
            char firstLetters[6];
            char secondLetters[6];
            int firstPoint = (int)stack[i].firstPoint.x;
            firstLetters[0] = (firstPoint % 10) + 48;
            firstPoint /= 10;
            firstLetters[1] = (firstPoint % 10) + 48;
            firstPoint /= 10;
            firstLetters[2] = (firstPoint % 10) + 48;
            firstPoint = (int)stack[i].firstPoint.y;
            firstLetters[3] = (firstPoint % 10) + 48;
			firstPoint /= 10;
            firstLetters[4] = (firstPoint % 10) + 48;
			firstPoint /= 10;
            firstLetters[5] = (firstPoint % 10) + 48;
            int secondPoint = (int)stack[i].secondPoint.x;
            secondLetters[0] = (secondPoint % 10) + 48;
            secondPoint /= 10;
            secondLetters[1] = (secondPoint % 10) + 48;
            secondPoint /= 10;
            secondLetters[2] = (secondPoint % 10) + 48;
            secondPoint = (int)stack[i].secondPoint.y;
            secondLetters[3] = (secondPoint % 10) + 48;
			secondPoint /= 10;
            secondLetters[4] = (secondPoint % 10) + 48;
			secondPoint /= 10;
            secondLetters[5] = (secondPoint % 10) + 48;
            fprintf(file, "%c%c%c %c%c%c %c%c%c %c%c%c %c %c\n", firstLetters[2], firstLetters[1], firstLetters[0], firstLetters[5], firstLetters[4], firstLetters[3], secondLetters[2], secondLetters[1], secondLetters[0], secondLetters[5], secondLetters[4], secondLetters[3], stack[i].strokeSize / 5 + 48, stack[i].color.b % 10 + 48);
        }
    }
    fclose(file);
}

void openFile(RenderTexture2D* target) {
    clearStack();
    char path[1000] = { 0 };
    printf("Enter file path: ");
    fgets(path, 999, stdin);
    path[strcspn(path, "\n")] = 0;
    FILE* file = fopen(path, "r");
    if (file == NULL) return;
    while (!feof(file)) {
		char line[20] = { 0 };
        fgets(line, 20, file);
        if (line[0] == '\n') continue;
        Vector2 firstPoint = { 0, 0 };
        Vector2 secondPoint = { 0, 0 };
		int f = 0;
        f += line[0] - 48;
        f *= 10;
        f += line[1] - 48;
        f *= 10;
        f += line[2] - 48;
        firstPoint.x = f;
		f = 0;
        f += line[4] - 48;
        f *= 10;
        f += line[5] - 48;
        f *= 10;
        f += line[6] - 48;
        firstPoint.y = f;
		int s = 0;
        s += line[8] - 48;
        s *= 10;
        s += line[9] - 48;
        s *= 10;
        s += line[10] - 48;
        secondPoint.x = s;
		s = 0;
        s += line[12] - 48;
        s *= 10;
        s += line[13] - 48;
        s *= 10;
        s += line[14] - 48;
        secondPoint.y = s;
        int strokeSize = (line[16] - 48) * 5;
        int linearDigit = line[18] - 48;
        Color strokeColor;
        if (linearDigit == green.b % 10) strokeColor = green;
        else if (linearDigit == brown.b % 10) strokeColor = brown;
        else if (linearDigit == orangered.b % 10) strokeColor = orangered;
        else if (linearDigit == blue.b % 10) strokeColor = blue;
        else if (linearDigit == white.b % 10) strokeColor = white;
        else if (linearDigit == black.b % 10) strokeColor = black;
        else if (linearDigit == backgroundColor.b % 10) strokeColor = backgroundColor;
        else if (linearDigit == pink.b % 10) strokeColor = pink;
        else {
            strokeColor = black;
        }
        AddDrawnItem(firstPoint, secondPoint, strokeSize, strokeColor);
    }
    RedrawTexture(target);
    fclose(file);
}


void AddDrawnItem(Vector2 firstPoint, Vector2 secondPoint, int strokeSize, Color color) {
    stack[currentIndex].firstPoint = firstPoint;
    stack[currentIndex].secondPoint = secondPoint;
    stack[currentIndex].strokeSize = strokeSize;
    stack[currentIndex].color = color;
    currentIndex++;
    currentIndex %= MAX_STACK;
}

void DeleteDrawnItem(RenderTexture2D* target) {
    if (currentIndex > 0)
        currentIndex--;
    else
        currentIndex = MAX_STACK - 1;
    stack[currentIndex].strokeSize = 0;
    RedrawTexture(target);
}

void RedrawTexture(RenderTexture2D* target) {
    BeginTextureMode(*target);
    ClearBackground(backgroundColor);
    
    for (int i = 0; i < MAX_STACK; i++) {
        if (stack[i].strokeSize != 0) {
			DrawLineEx(stack[i].firstPoint, stack[i].secondPoint, stack[i].strokeSize, stack[i].color);
        }
    }

    EndTextureMode();
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    Vector2 lastPosition = { -1, -1 };
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    int strokeSize = 10;
    Color strokeColor = green;

    Rectangle greenRect = { screenWidth - 180, 80, 70, 70 };
    Rectangle brownRect = { screenWidth - 90, 80, 70, 70 };
    Rectangle orangeredRect = { screenWidth - 180, 170, 70, 70 };
    Rectangle blueRect = { screenWidth - 90, 170, 70, 70 };
    Rectangle whiteRect = { screenWidth - 180, 260, 70, 70 };
    Rectangle blackRect = { screenWidth - 90, 260, 70, 70 };
    Rectangle eraserRect = { screenWidth - 180, 350, 70, 70 };
    Rectangle pinkRect = { screenWidth - 90, 350, 70, 70 };

	BeginTextureMode(target);
	ClearBackground(backgroundColor);
	EndTextureMode();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(0)) {
            if (CheckCollisionPointRec(GetMousePosition(), greenRect)) strokeColor = green;
            if (CheckCollisionPointRec(GetMousePosition(), brownRect)) strokeColor = brown;
            if (CheckCollisionPointRec(GetMousePosition(), orangeredRect)) strokeColor = orangered;
            if (CheckCollisionPointRec(GetMousePosition(), blueRect)) strokeColor = blue;
            if (CheckCollisionPointRec(GetMousePosition(), whiteRect)) strokeColor = white;
            if (CheckCollisionPointRec(GetMousePosition(), blackRect)) strokeColor = black;
            if (CheckCollisionPointRec(GetMousePosition(), eraserRect)) strokeColor = backgroundColor;
            if (CheckCollisionPointRec(GetMousePosition(), pinkRect)) strokeColor = pink;
            
        }
        if (IsKeyDown(KEY_Z)) {
            DeleteDrawnItem(&target);
        }
        if (IsKeyPressed(KEY_S)) {
            saveFile();
        }
        if (IsKeyPressed(KEY_O)) {
            openFile(&target);
        }
        if (IsKeyPressed(KEY_ONE)) {
            strokeSize = 5;
        }
        else if (IsKeyPressed(KEY_TWO)) {
            strokeSize = 10;
        }
        else if (IsKeyPressed(KEY_THREE)) {
            strokeSize = 15;
        }
        else if (IsKeyPressed(KEY_FOUR)) {
            strokeSize = 20;
        }
        if (IsMouseButtonDown(0) && GetMousePosition().x < 1000) {
            Vector2 mousePos = GetMousePosition();
            BeginTextureMode(target);
            if (lastPosition.x != -1 && lastPosition.y != -1) {

                Vector2 firstPoint = { (int)mousePos.x - (int)mousePos.x % 5, screenHeight - (int)mousePos.y - (int)mousePos.y % 5 };
                Vector2 secondPoint = { (int)lastPosition.x - (int)lastPosition.x % 5, screenHeight - (int)lastPosition.y - (int)lastPosition.y % 5 };

				DrawLineEx(firstPoint, secondPoint, strokeSize, strokeColor);
                AddDrawnItem(firstPoint, secondPoint, strokeSize, strokeColor);
            }
            lastPosition = mousePos;
            EndTextureMode();
        }
        else {
            lastPosition.x = -1;
            lastPosition.y = -1;
        }

        BeginDrawing();

		DrawTexture(target.texture, 0, 0, backgroundColor);

		DrawRectangle(screenWidth - 200, 0, 200, screenHeight, DARKBROWN);
        DrawText("Color Picker:", screenWidth - 180, 50, 25, LIGHTGRAY);

        DrawRectangleRec(greenRect, green);
        DrawRectangleRec(brownRect, brown);
        DrawRectangleRec(orangeredRect, orangered);
        DrawRectangleRec(blueRect, blue);
        DrawRectangleRec(whiteRect, white);
        DrawRectangleRec(blackRect, black);
        DrawRectangleRec(eraserRect, backgroundColor);
        DrawRectangleRec(pinkRect, pink);
    
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
