#ifndef TERM_SIZE_H
#define TERM_SIZE_H

typedef struct
{
    int rows;
    int columns;
} TerminalSize;

int get_terminal_size(TerminalSize *size);

#endif // TERM_SIZE_H