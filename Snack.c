#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    srand(time(NULL));

    int gameover = 0;
    int score = 0;
    int x = 10, y = 10;
    int fruitX = rand() % 20;
    int fruitY = rand() % 20;
    int nTail = 0;
    int tailX[100], tailY[100];
    enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
    enum eDirecton dir = STOP;

    while (!gameover) {
        usleep(100000); // Adjust the speed of the game

        if (kbhit()) {
            switch (getchar()) {
                case 'a':
                    dir = LEFT;
                    break;
                case 'd':
                    dir = RIGHT;
                    break;
                case 'w':
                    dir = UP;
                    break;
                case 's':
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

        if (x < 0 || x >= 20 || y < 0 || y >= 20) {
            gameover = 1;
        }

        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameover = 1;
            }
        }

        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % 20;
            fruitY = rand() % 20;
            nTail++;
        }

        system("clear");

        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (i == 0 || i == 19 || j == 0 || j == 19) {
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
