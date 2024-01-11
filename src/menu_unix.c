#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#include "menu.h"
#include <stdint.h>
#include <stdio.h>
#include "colors.h"
#include "main.h"
#include "term_size.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "json.h"

#define ESC 27

extern TerminalSize term_size; // from src/main.c

// Function to enable raw mode for terminal input
void enableRawMode(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

// Function to disable raw mode for terminal input
void disableRawMode(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

int32_t arrow_menu(char *strings[], int32_t size)
{
    clear_menu();
    enableRawMode();

    int32_t selectedOption = 0;
    int32_t totalOptions = size;

    while (1)
    {
        // Print menu options
        for (int32_t i = 0; i < totalOptions; i++)
        {
            if (i == selectedOption)
            {
                printf("\033[7m"); // Invert text color (selected option)
                menu_centered_item(strings[i], "", "", i - (totalOptions / 2) + 1);
                printf("%s\n", RESET); // Reset text color
            }
            else
            {

                menu_centered_item(strings[i], "", "", i - (totalOptions / 2) + 1);
                puts("");
            }
        }

        // Handle arrow key input
        char c = 0;
        while (1)
        {
            c = getchar();
            if (c == ESC)
            {
                c = getchar(); // Read the [
                c = getchar(); // Read the A, B, C, or D
                if (c == 'A')
                {
                    if (selectedOption > 0)
                        selectedOption--; // Up arrow key
                    else
                        selectedOption = totalOptions - 1; // Voltar ao fim
                    break;
                }
                else if (c == 'B')
                {
                    if (selectedOption < totalOptions - 1)
                        selectedOption++; // Down arrow key
                    else
                        selectedOption = 0; // Voltar ao inicio
                    break;
                }
            }
            else if (c == '\n')
            {
                disableRawMode();
                return selectedOption;
            }
        }
        clear_menu();
        cursor_upLeft();
    }
}

int32_t input_menu(Input inputItems[], int32_t inputItemsSize, int32_t isVenda)
{
    clear_menu();
    enableRawMode();

    const unsigned int maxInputSize = 40;

    int selectedButton = 0;
    int selectedItem = 0;

    int converted = atoi(inputItems[3].input); // Converter o input para int | Input de checkbox indica a sua posição
    int selectedCheckbox;
    if (converted >= 0 && converted <= 4 && strcmp(inputItems[3].input, "") != 0) // Se o input for um numero entre 0 e 4
        selectedCheckbox = converted;                                             // selectedCheckbox = input
    else
        selectedCheckbox = -1; // -1 = Nenhum selecionado

    int quantidadeMax = 0;
    if (isVenda)
    {
        quantidadeMax = atoi(inputItems[2].input);
    }

    char c;

    while (1)
    {
        if (isVenda) // Mostrar o preco total
        {
            char vendaString[40];
            sprintf(vendaString, "Total: %.2f€", atof(inputItems[1].input) * atof(inputItems[2].input));
            menu_centered_item(vendaString, UNDERLINE, "", -5);
        }

        for (int i = 0; i < inputItemsSize; i++)
        {

            if (inputItems[i].isCheckbox)
            {
                if (i == selectedItem)
                {
                    printMenuItem(inputItems[i], 1, (i + 1) - ((inputItemsSize + 5) / 2));
                }
                else
                {

                    printMenuItem(inputItems[i], 0, (i + 1) - ((inputItemsSize + 5) / 2));
                }

                for (int j = 0; j < 5; j++)
                {

                    if (j == selectedCheckbox)
                    {
                        if (selectedItem > 3 && selectedItem == j + 4)
                        {
                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 1, 1, i - (inputItemsSize / 2) + j);
                        }
                        else
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 0, 1, i - (inputItemsSize / 2) + j);
                        }
                    }
                    else
                    {
                        if (selectedItem > 3 && selectedItem == j + 4)
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 1, 0, i - (inputItemsSize / 2) + j);
                        }
                        else
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 0, 0, i - (inputItemsSize / 2) + j);
                        }
                    }
                }
                continue;
            }

            if (i == selectedItem)
            {
                printMenuItem(inputItems[i], 1, i - ((inputItemsSize + 5) / 2) + 1);
            }
            else
            {
                printMenuItem(inputItems[i], 0, i - ((inputItemsSize + 5) / 2) + 1);
            }
        }

        if (selectedButton == 0)
        {
            printf("\033[%d;%dH\033[7m[OK]\033[0m   [Cancelar]\n", (term_size.rows / 2) + inputItemsSize + 2,
                   (term_size.columns - 17) / 2); // 17 é o tamanho da string "[OK]   [Cancel]"
        }
        else
        {
            printf("\033[%d;%dH[OK]   \033[7m[Cancelar]\033[0m\n", (term_size.rows / 2) + inputItemsSize + 2,
                   (term_size.columns - 17) / 2); // 17 é o tamanho da string "[OK]   [Cancel]"
        }

        c = getchar();
        if (c == ESC)
        {
            c = getchar(); // Read the [
            c = getchar(); // Read the A, B, C, or D
            if (c == 'A')  // Up Arrow
            {
                if (selectedItem > 0)
                    selectedItem--;
                else
                    selectedItem = inputItemsSize + 5 - 1; // Voltar ao fim
            }
            else if (c == 'B') // Down Arrow
            {
                if (selectedItem < inputItemsSize + 5 - 1)
                    selectedItem++;
                else
                    selectedItem = 0; // Voltar ao inicio
            }
            else if (c == 'D') // Left Arrow
            {
                if (selectedButton == 1)
                    selectedButton = 0;
                else
                    selectedButton = 1;
            }
            else if (c == 'C') // Right Arrow
            {
                if (selectedButton == 0)
                    selectedButton = 1;
                else
                    selectedButton = 0;
            }
        }
        else if (c == '\n')
        { // Enter Key
            disableRawMode();
            return selectedButton; // 0 = OK, 1 = Cancelar
        }
        else if (c == 127)
        { // Backspace

            if (selectedItem >= 0 && selectedItem <= inputItemsSize - 1)
            {
                // Remove the last character from the input
                size_t len = strlen(inputItems[selectedItem].input);
                if (len > 0)
                {
                    inputItems[selectedItem].input[len - 1] = '\0';
                }
            }
        }
        else
        { // Handle input characters
            if (selectedItem >= 0 && selectedItem <= inputItemsSize - 1 && !inputItems[selectedItem].isCheckbox)
            {
                if (isVenda && selectedItem < 2) // Nao mudar preço nem nome
                {
                    continue;
                }

                if (strlen(inputItems[selectedItem].input) < maxInputSize - 1)
                {
                    inputItems[selectedItem].input[strlen(inputItems[selectedItem].input)] = c;
                    // inputItems[selectedItem].input[strlen(inputItems[selectedItem].input)] = '*';
                    // strncat(inputItems[selectedItem].input, &c, 1);
                }
                if (isVenda && selectedItem == 2)
                {
                    int userInput = atoi(inputItems[2].input);
                    if (userInput > quantidadeMax)
                    {
                        char quantidadeMaxString[40];
                        sprintf(quantidadeMaxString, "%d", quantidadeMax);
                        copy_str(inputItems[2].input, quantidadeMaxString, 40);
                    }
                }
            }
        }
        if (c == 32) // Barra de espaço
        {
            if (selectedItem > 3 && !isVenda)
            {
                selectedCheckbox = selectedItem - 4;
                sprintf(inputItems[3].input, "%d", selectedCheckbox);
            }
        }

        cursor_upLeft();
        clear_menu();
    }

    return 0;
}

// Function that is a arrow menu but when you press 'f' the user can start typing and the menu will filter the options by Artigo.categoria
int32_t arrow_menu_filter(Artigo *artigos, int32_t size)
{
    // TODO: Resolver bug de ter que clicar varias vezes se items filtrados tiverem separados
    clear_menu();
    enableRawMode();

    int32_t selectedOption = 0;
    int32_t totalOptions = size;
    int32_t isFiltering = 0;
    char filter[20];
    char c;
    int changed = 0;

    while (1)
    {
        // Print menu options
        if (isFiltering)
        {
            char filterString[40];
            sprintf(filterString, "(F)iltrar por: %s", filter);
            menu_centered_item(filterString, UNDERLINE, "", -2);
        }
        else
        {

            menu_centered_item("(F)iltrar", UNDERLINE, "", -2);
        }

        for (int32_t i = 0; i < totalOptions; i++)
        {
            if (i == selectedOption)
            {
                if (!isFiltering || strstr(categoria_to_str(artigos[i].categoria), filter) != NULL)
                {
                    printf("\033[7m"); // Invert text color (selected option)
                    menu_centered_item(artigos[i].nome, "", "", i - (totalOptions / 2) + 1);
                    printf("%s\n", RESET); // Reset text color
                }
            }
            else
            {
                if (!isFiltering || strstr(categoria_to_str(artigos[i].categoria), filter) != NULL)
                {
                    menu_centered_item(artigos[i].nome, "", "", i - (totalOptions / 2) + 1);
                    puts("");
                }
            }
        }

        if (isFiltering && !changed)
        {
            int firstMatch = -1;
            for (int i = 0; i < totalOptions; i++)
            {
                if (strstr(categoria_to_str(artigos[i].categoria), filter) != NULL)
                {
                    firstMatch = i;
                    break;
                }
            }

            if (firstMatch != -1)
            {
                selectedOption = firstMatch;
            }
        }

        // Handle arrow key input
        c = getchar();
        if (c == ESC)
        {
            c = getchar(); // Read the [
            c = getchar(); // Read the A, B, C, or D
            if (c == 'A')
            {
                if (selectedOption > 0)
                {
                    selectedOption--; // Up arrow key
                    changed = 1;
                }
                else
                {

                    changed = 1;
                    selectedOption = totalOptions - 1; // Voltar ao fim
                }
            }
            else if (c == 'B')
            {
                if (selectedOption < totalOptions - 1)
                {

                    changed = 1;
                    selectedOption++; // Down arrow key
                }
                else
                {

                    changed = 1;
                    selectedOption = 0; // Voltar ao inicio
                }
            }
        }
        else if (c == '\n')
        {
            disableRawMode();
            return selectedOption;
        }
        else if (c == 127)
        { // Backspace
            if (isFiltering)
            {
                if (strlen(filter) > 0)
                {
                    filter[strlen(filter) - 1] = '\0';
                }
            }
        }
        else if (c == '\t') // Sair do modo de filtrar quando clicar no Tab
        {
            isFiltering = 0;
            changed = 0;
        }
        else
        { // Handle input characters
            if (isFiltering)
            {
                if (strlen(filter) < 20 - 1)
                {
                    filter[strlen(filter)] = c;
                    filter[strlen(filter) + 1] = '\0';
                }
            }
        }

        if (c == 'f' || c == 'F')
        {
            isFiltering = 1;
            copy_str(filter, "\0", 1);
        }

        clear_menu();
        cursor_upLeft();
    }
    return 0;
}

#elif _WIN32

#pragma warning(disable : 4206) // Tirar aviso de "nonstandard extension used: translation unit is empty" porque este ficheiro só é usado no linux

#endif