#include "term_size.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // Desativar coisas desnecessárias do windows.h
#include <windows.h>

int get_terminal_size(TerminalSize *size)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {

        size->columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size->rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return 0; // Sucesso
    }
    else
    {
        return -1; // Erro
    }
}

#elif __unix__

#include <sys/ioctl.h>
#include <unistd.h>

int get_terminal_size(TerminalSize *size)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
    {
        size->rows = ws.ws_row;
        size->columns = ws.ws_col;
        return 0; // Sucesso
    }
    else
    {
        return -1; // Erro
    }
}

#endif