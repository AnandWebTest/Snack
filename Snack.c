
//To make the snake game area larger to match the size of the Windows console window, you'll need to adjust the dimensions of the game board and the position of the snake and the fruit accordingly. Here's the modified code:

//c
//Copy code
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 80
#define HEIGHT 25

int kbhit() {
    return _kbhit();
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    srand(time(NULL));

    int gameover = 0;
    int score = 0;
    int x = WIDTH / 2, y = HEIGHT / 2;
    int fruitX = rand() % WIDTH;
    int fruitY = rand() % HEIGHT;
    int nTail = 0;
    int tailX[WIDTH * HEIGHT];
    int tailY[WIDTH * HEIGHT];
    enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
    enum eDirecton dir = STOP;

    while (!gameover) {
        Sleep(100); // Adjust the speed of the game

        if (kbhit()) {
            switch (_getch()) {
                case 'a':
                case 75: // left arrow key
                    dir = LEFT;
                    break;
                case 'd':
                case 77: // right arrow key
                    dir = RIGHT;
                    break;
                case 'w':
                case 72: // up arrow key
                    dir = UP;
                    break;
                case 's':
                case 80: // down arrow key
                    dir = DOWN;
                    break;
                case 'x':
                    gameover = 1;
                    break;
            }
        }

        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        switch (dir) {
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            default:
                break;
        }

        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
            gameover = 1;
        }

        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameover = 1;
            }
        }

        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % WIDTH;
            fruitY = rand() % HEIGHT;
            nTail++;
        }

        system("cls");

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                    printf("#");
                } else if (i == fruitY && j == fruitX) {
                    printf("F");
                } else if (i == y && j == x) {
                    printf("O");
                } else {
                    int isPrinted = 0;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            printf("o");
                            isPrinted = 1;
                        }
                    }
                    if (!isPrinted) {
                        printf(" ");
                    }
                }
            }
            printf("\n");
        }

        printf("Score: %d\n", score);
    }

    return 0;
}