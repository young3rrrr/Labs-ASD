#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int SCREEN_WIDTH = 80;
int SCREEN_HEIGHT = 24;
int DELAY_MS = 50;

void gotoxy(int x, int y);

int main(int argc, char* argv[]) {
    if (argc < 2 || (argv[1] == "--terminal") != 0)
    {
        system("start main.exe --terminal");
        return 0;
    }
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int x_start = 1; x_start <= SCREEN_WIDTH; x_start +=2) {
        int x = x_start;
        int y = SCREEN_HEIGHT; 

        while (x >= 1 && y >= 1) {
            gotoxy(x, y);
            printf("*");
            
            if (DELAY_MS > 0) {
                fflush(stdout);
                Sleep(DELAY_MS);
            }

            x--; 
            y--;
        }
        x++;
        while (y <= SCREEN_HEIGHT)
        {
        gotoxy(x, y);
            printf("*");
            
            if (DELAY_MS > 0) {
                fflush(stdout);
                Sleep(DELAY_MS);
            }

            x++; 
            y++;
        }
        
    }
    for (int y_start = SCREEN_HEIGHT; y_start >= 1; y_start -=2) {
        int x = SCREEN_WIDTH;
        int y = y_start;
        while (y >= 1) {
            gotoxy(x, y);
            printf("*");
            
            if (DELAY_MS > 0) {
                fflush(stdout);
                Sleep(DELAY_MS);
            }

            x--; 
            y--;
        }
        x++;
        while (x <= SCREEN_WIDTH)
        {        gotoxy(x, y);
            printf("*");
            
            if (DELAY_MS > 0) {
                fflush(stdout);
                Sleep(DELAY_MS);
            }

            x++; 
            y++;
        }
    }
    gotoxy(1, SCREEN_HEIGHT + 1);
    getchar();
    return 0;
}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x - 1;
    coord.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}