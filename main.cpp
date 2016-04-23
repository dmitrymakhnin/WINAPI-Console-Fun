#include <Windows.h>
#include <stdio.h>

inline static void setup();

/* centralize known information */
HANDLE hInput, hOutput, hError;
HWND hConsoleWindow;
WORD intense = 0;

int main(int argc, char **argv)
{
    setup();

    while (TRUE) {
        INPUT_RECORD irec;
        DWORD nbytes_read;

        ReadConsoleInput(hInput, &irec, 1, &nbytes_read);

        if (irec.EventType == KEY_EVENT && irec.Event.KeyEvent.bKeyDown == TRUE) {
            KEY_EVENT_RECORD *ker = &(irec.Event.KeyEvent);
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            WORD e = ker->wVirtualKeyCode;

            if (e == VK_ESCAPE) {
                exit(0);
            } 

            if (e == VK_SPACE) { putchar(' '); }
            
            if (e >= VK_LEFT && e <= VK_DOWN) {
                GetConsoleScreenBufferInfo(hOutput, &csbi);

                switch (e) {
                case VK_LEFT:
                    csbi.dwCursorPosition.X -= 1;
                    break;
                case VK_RIGHT:
                    csbi.dwCursorPosition.X += 1;
                    break;
                case VK_UP:
                    csbi.dwCursorPosition.Y -= 1;
                    break;
                case VK_DOWN:
                    csbi.dwCursorPosition.Y += 1;
                    break;
                };

                SetConsoleCursorPosition(hOutput, csbi.dwCursorPosition);      
            }

            /* anonymous scope */
            {
                char ch = ker->uChar.AsciiChar;

                if (ch >= 'a' && ch <= 'z') {
                    putchar(ch);
                }

                if (ch == '1') {
                    SetConsoleTextAttribute(hOutput, FOREGROUND_RED | intense);
                } else if (ch == '2') {
                    SetConsoleTextAttribute(hOutput, FOREGROUND_GREEN | intense);
                } else if (ch == '3') {
                    SetConsoleTextAttribute(hOutput, FOREGROUND_BLUE | intense);
                } else if (ch == '4') {
                    SetConsoleTextAttribute(hOutput, 0x00 | intense);
                } else if (ch == '5') {
                    if (intense) {
                        intense = 0;
                    } else {
                        intense = FOREGROUND_INTENSITY;
                    }
                }
            }   
        }
    }

    return 0;
}

/**
 * Responsible for:
 *    1. set up standard console handles.
 */
inline static void setup()
{
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    hError = GetStdHandle(STD_ERROR_HANDLE);
    hConsoleWindow = GetConsoleWindow();
}