#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include "main.h"
#include <stdlib.h>

typedef struct
{
    char label[40];
    char input[40];
    int isCheckbox;
    char checkBoxOptions[6][40];
    // Checkbox checkbox;

} Input;

void menu_item(char *string, char *color1, char *color2);
void menu_centered_item(char *string, char *color1, char *color2,
                        int32_t row_offset);
void clear_menu(void);
int arrow_menu(char *strings[], size_t size, int is_welcome_screen);
void cursor_upLeft(void);
void hide_cursor(void);
void show_cursor(void);
#ifdef __unix__
void enableRawMode(void);
void disableRawMode(void);
#endif

void menu_principal(void);
int input_menu(Input inputItems[], int inputItemsSize, int isVenda);
void printMenuItem(Input item, int32_t isSelected, int32_t offset);
void printMenuCheckbox(char *string, int32_t isSelected, int32_t isCheckboxSelected, int32_t offset);
int arrow_menu_filter(Livro *livros, int size);

void pressione_qualquer_tecla(int row_offset);

#endif // MENU_H