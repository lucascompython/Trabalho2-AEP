#ifdef _WIN32

#include <conio.h>
#include <stdio.h>
#include "menu.h"
#include <stdint.h>
#include "colors.h"
#include "term_size.h"
#include <string.h>
#include "json.h"

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ENTER 13
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define BACKSPACE 8
#define SPACE 32

extern TerminalSize term_size; // from src/main.c

int arrow_menu(char *strings[], int size)
{
    clear_menu();

    int selectedOption = 0;
    int totalOptions = size;
    while (1)
    {
        // Print menu options
        for (int i = 0; i < totalOptions; i++)
        {
            if (i == selectedOption)
            {
                printf("\033[7m"); // Invert text color (selected option)
                menu_centered_item(strings[i], "", "", i);
                printf("%s\n", RESET); // Reset text color
            }
            else
            {
                menu_centered_item(strings[i], "", "", i);
                puts("");
            }
        }

        // Handle arrow key input
        int key = 0;
        while (1)
        {
            key = _getch();
            if (key == ENTER) // Enter key
            {
                return selectedOption;
            }
            else if (key == ARROW_UP)
            {
                if (selectedOption > 0)
                    selectedOption--; // Up arrow key
                else
                    selectedOption = totalOptions - 1; // Voltar para o fim

                break;
            }
            else if (key == ARROW_DOWN)
            {
                if (selectedOption < totalOptions - 1)
                    selectedOption++; // Down arrow key
                else
                    selectedOption = 0; // Voltar para o inicio

                break;
            }
        }
    }
}

int32_t input_menu(Input inputItems[], int32_t inputItemsSize, int32_t isVenda)
{
    clear_menu();

    const unsigned int maxInputSize = 40;

    int selectedButton = 0;
    int selectedItem = 0;

    int converted = atoi(inputItems[3].input);
    int selectedCheckbox;
    if (converted >= 0 && converted <= 4 && strcmp(inputItems[3].input, "") != 0)
    {
        selectedCheckbox = converted;
    }
    else
    {
        selectedCheckbox = -1; // Nenhuma checkbox selecionada
    }
    int quantidadeMax = 0;
    if (isVenda)
    {
        quantidadeMax = atoi(inputItems[2].input);
    }

    int c;

    while (1)
    {

        if (isVenda)
        {
            char vendaString[40];
            sprintf_s(vendaString, 40, "Total: %.2f€", strtof(inputItems[1].input, NULL) * strtof(inputItems[2].input, NULL));
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

        c = _getch();
        if (c == ARROW_UP) // Up Arrow
        {
            if (selectedItem > 0)
                selectedItem--;
            else
                selectedItem = inputItemsSize + 5 - 1; // Voltar ao fim
        }
        else if (c == ARROW_DOWN) // Down Arrow
        {
            if (selectedItem < inputItemsSize + 5 - 1)
                selectedItem++;
            else
                selectedItem = 0; // Voltar ao inicio
        }
        else if (c == ARROW_LEFT) // Left Arrow
        {
            if (selectedButton == 1)
                selectedButton = 0;
            else
                selectedButton = 1;
        }
        else if (c == ARROW_RIGHT) // Right Arrow
        {
            if (selectedButton == 0)
                selectedButton = 1;
            else
                selectedButton = 0;
        }
        else if (c == ENTER)
        { // Enter Key
            return selectedButton;
        }
        else if (c == BACKSPACE)
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
        else if (c >= 32 && c <= 126)
        { // Printable characters
            if (isVenda && selectedItem < 2)
            { // Nao mudar preço nem nome
                continue;
            }
            if (strlen(inputItems[selectedItem].input) < maxInputSize - 1)
            {
                inputItems[selectedItem].input[strlen(inputItems[selectedItem].input)] = c;
            }
            if (isVenda && selectedItem == 2)
            {
                int quantidade = atoi(inputItems[2].input);
                if (quantidade > quantidadeMax)
                {
                    char quantidadeString[40];
                    sprintf_s(quantidadeString, 40, "%d", quantidadeMax);
                    copy_str(inputItems[2].input, quantidadeString, 40);
                }
            }
        }
        if (c == SPACE) // Barra de espaço
        {
            if (selectedItem > 3 && !isVenda) // Se tiver numa checkbox
            {
                selectedCheckbox = selectedItem - 4;
                sprintf_s(inputItems[3].input, sizeof(inputItems[3].input), "%d", selectedCheckbox);
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
            sprintf_s(filterString, 40, "(F)iltrar por: %s", filter);
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
        c = _getch();
        if (c == ARROW_UP)
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
        else if (c == ARROW_DOWN)
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

        else if (c == ENTER)
        {
            return selectedOption;
        }
        else if (c == BACKSPACE)
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

#endif