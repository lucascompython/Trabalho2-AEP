#include "menu.h"
#include "colors.h"
#include "main.h"
#include "term_size.h"
#include "json.h"
#include "uuid.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
#include <conio.h> // Para a welcome screen _getch() ler todas as teclas
#endif

#ifdef __unix__
// sprintf_s() não existe no linux
#define sprintf_s(buffer, size, format, ...) sprintf(buffer, format, __VA_ARGS__)
#endif

extern TerminalSize term_size;                 // from src/main.c
extern Livro *livros;                          // from src/main.c
extern size_t size_livros;                     // from src/main.c
extern Emprestimo *emprestimos;                // from src/main.c
extern size_t size_emprestimos;                // from src/main.c
extern struct tm current_datetime;             // from src/main.c
extern PessoaRequisicoes *pessoas_requisicoes; // from src/main.c

void pressione_qualquer_tecla(int row_offset)
{
    menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", row_offset);
#ifdef __unix__ // ler "qualquer" teclas no linux

    enableRawMode();
    getchar();
    disableRawMode();
#elif _WIN32
    _getch(); // ler qualquer tecla no windows
#endif
}

void printMenuItem(Input item, int32_t isSelected, int32_t offset)
{
    int labelLength = strlen(item.label);
    int inputLength = strlen(item.input);
    if (isSelected)
    {
        if (item.isCheckbox)
        {
            printf("\033[%d;%dH> %s%s%s <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - labelLength) / 2) - 2, UNDERLINE, item.label, RESET);
            return;
        }
        if (inputLength)
        {

            printf("\033[%d;%dH> %s: %s <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - labelLength) / 2) - 2, item.label, item.input);
        }
        else
        {

            printf("\033[%d;%dH> %s: %s<\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - labelLength) / 2) - 2, item.label, item.input);
        }
    }
    else
    {
        if (item.isCheckbox)
        {
            printf("\033[%d;%dH%s%s%s\n", (term_size.rows / 2) + offset,
                   (term_size.columns - labelLength) / 2, UNDERLINE, item.label, RESET);
            return;
        }

        printf("\033[%d;%dH%s: %s\n", (term_size.rows / 2) + offset,
               (term_size.columns - labelLength) / 2, item.label, item.input);
    }
}

void printMenuCheckbox(char *string, int32_t isSelected, int32_t isCheckboxSelected, int32_t offset)
{
    int stringLength = strlen(string);
    if (isSelected)
    {
        if (isCheckboxSelected)
        {
            printf("\033[%d;%dH> %s: [X] <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - stringLength) / 2) - 2, string);
        }
        else
        {
            printf("\033[%d;%dH> %s: [ ] <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - stringLength) / 2) - 2, string);
        }
    }
    else
    {
        if (isCheckboxSelected)
        {
            printf("\033[%d;%dH%s: [X]\n", (term_size.rows / 2) + offset,
                   (term_size.columns - stringLength) / 2, string);
        }
        else
        {
            printf("\033[%d;%dH%s: [ ]\n", (term_size.rows / 2) + offset,
                   (term_size.columns - stringLength) / 2, string);
        }
    }

    // int stringLength = strlen(string);
    // if (isSelected)
    // {
    //     printf("\033[%d;%dH> %s: [X] <\n", (term_size.rows / 2) + offset,
    //            ((term_size.columns - stringLength) / 2) - 2, string);
    // }
    // else
    // {
    //     printf("\033[%d;%dH%s: [ ]\n", (term_size.rows / 2) + offset,
    //            (term_size.columns - stringLength) / 2, string);
    // }
}

void menu_item(char *string, char *color1, char *color2)
{
    size_t string_size = strlen(string);
    printf("\033[%d;%zdH%s%s%s%s", term_size.rows, term_size.columns - string_size,
           color1, color2, string, RESET);
}

void menu_centered_item(char *string, char *color1, char *color2,
                        int32_t row_offset)
{

    size_t string_size = strlen(string);
    printf("\033[%d;%zdH%s%s%s%s", (term_size.rows / 2) + row_offset,
           (term_size.columns - string_size) / 2, color1, color2, string, RESET);
}

void clear_menu(void) { printf("\033[2J"); }

void cursor_upLeft(void) { printf("\033[H"); }

void hide_cursor(void) { printf("\033[?25l"); }

void show_cursor(void) { printf("\033[?25h"); }

void menu_introduzir_livro(void)
{

    Input inputItems[] = {
        {.label = "Titulo", .input = "", .isCheckbox = 0},
        {.label = "ISBN", .input = "", .isCheckbox = 0},
        {.label = "Quantidade de Examplares", .input = "", .isCheckbox = 0},
        {.label = "Quantidade Disponivel", .input = "", .isCheckbox = 0},
        {.label = "Categoria", .input = "", .isCheckbox = 1, .checkBoxOptions = {"Romances", "Ficção", "Aventura", "Terror", "Biografia", "Outros"}},
    };

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:

        size_livros++;

        livros = realloc(livros, sizeof(Livro) * (size_livros));
        if (livros == NULL)
        {
            fprintf(stderr, "Erro: realloc() retornou NULL\n");
            exit(1);
        }

        livros[size_livros - 1].titulo = (char *)malloc(sizeof(char) * (strlen(inputItems[0].input) + 1));
        if (livros[size_livros - 1].titulo == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }
        copy_str(livros[size_livros - 1].titulo, inputItems[0].input, strlen(inputItems[0].input) + 1);
        copy_str(livros[size_livros - 1].isbn, inputItems[1].input, 14);

        livros[size_livros - 1].quantidade_exemplares = atoi(inputItems[2].input);
        livros[size_livros - 1].quantidade_disponivel = atoi(inputItems[3].input);
        livros[size_livros - 1].categoria = atoi(inputItems[4].input);

        clear_menu();
        menu_centered_item("Livro introduzido com sucesso!", GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

        pressione_qualquer_tecla(3);

        menu_principal();

        break;
    case 1:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

void menu_introduzir_emprestimo(void)
{
    Input inputItems[] = {
        {.label = "Número de CC", .input = "", .isCheckbox = 0},
        {.label = "ISBN", .input = "", .isCheckbox = 0}};

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:

        int num_cc = atoi(inputItems[0].input);

        // Verificar se o livro existe
        int livroIndex = -1;
        for (size_t i = 0; i < size_livros; i++)
        {
            if (strcmp(livros[i].isbn, inputItems[1].input) == 0)
            {
                livroIndex = i;
                break;
            }
        }
        if (livroIndex == -1)
        {
            clear_menu();
            menu_centered_item("Livro não encontrado!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            menu_principal();
            break;
        }

        // Verificar se o livro está disponivel
        if (livros[livroIndex].quantidade_disponivel == 0)
        {
            clear_menu();
            menu_centered_item("Livro não disponivel!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            menu_principal();
            break;
        }

        // Verificar se o utilizador tem mais de 3 livros emprestados
        int livros_emprestados = 0;
        for (size_t i = 0; i < size_emprestimos; i++)
        {
            if (emprestimos[i].num_cc == num_cc)
            {
                livros_emprestados++;
            }
        }
        if (livros_emprestados >= 3)
        {
            clear_menu();
            menu_centered_item("O utilizador já tem 3 ou mais livros emprestados!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            menu_principal();
            break;
        }

        size_emprestimos++;

        emprestimos = realloc(emprestimos, sizeof(Emprestimo) * (size_emprestimos));
        pessoas_requisicoes = realloc(pessoas_requisicoes, sizeof(PessoaRequisicoes) * (size_emprestimos));

        if (emprestimos == NULL || pessoas_requisicoes == NULL)
        {
            fprintf(stderr, "Erro: realloc() retornou NULL\n");
            exit(1);
        }

        emprestimos[size_emprestimos - 1].num_cc = num_cc;
        emprestimos[size_emprestimos - 1].livro = &livros[livroIndex];
        emprestimos[size_emprestimos - 1].ja_devolvido = 0;
        emprestimos[size_emprestimos - 1].data_emprestimo = current_datetime;
        // guardar a data de de devolução como 00-00-0000
        struct tm data_devolucao = {0};
        int dia, mes, ano;
        sscanf("00-00-0000", "%d-%d-%d", &dia, &mes, &ano);
        data_devolucao.tm_mday = dia;
        data_devolucao.tm_mon = mes - 1;
        data_devolucao.tm_year = ano - 1900;
        emprestimos[size_emprestimos - 1].data_devolucao = data_devolucao;

        copy_str(emprestimos[size_emprestimos - 1].uuid, uuid_gen(), 37);

        livros[livroIndex].quantidade_disponivel--;
        livros[livroIndex].num_requisicoes++;

        save_livros_array(livros, size_livros, STOCK_JSON_FILE);

        clear_menu();
        menu_centered_item("Emprestimo introduzido com sucesso!", GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

        pressione_qualquer_tecla(3);

        menu_principal();

        break;

    case 1:

        menu_principal();
        break;

    default:
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

void menu_listar_livros(void)
{

    if (size_livros == 0)
    {
        clear_menu();
        menu_centered_item("Não há livros para listar", UNDERLINE, "", 0);
        pressione_qualquer_tecla(2);

        menu_principal();
        return;
    }

    int32_t result = arrow_menu_filter(livros, size_livros);
    clear_menu();

    // show the selected artigo
    if (result != -1)
    {

        char isbn[40];
        char titulo[40];
        char autor[40];
        char quantidade_e[40];
        char quantidade_d[40];
        char num_requisicoes[40];
        char categoria[40];

        sprintf_s(isbn, 40, "ISBN: %s", livros[result].isbn);
        sprintf_s(titulo, 40, "Título: %s", livros[result].titulo);
        sprintf_s(autor, 40, "Autor: %s", livros[result].autor);
        sprintf_s(quantidade_e, 40, "Quantidade Exemplares: %d", livros[result].quantidade_exemplares);
        sprintf_s(quantidade_d, 40, "Quantidade Disponível: %d", livros[result].quantidade_disponivel);
        sprintf_s(num_requisicoes, 40, "Requisições: %d", livros[result].num_requisicoes);
        sprintf_s(categoria, 40, "Categoria: %s", categoria_to_str(livros[result].categoria));

        // TODO: Centrar isto melhor verticalmente
        menu_centered_item(isbn, "", "", 0);
        menu_centered_item(titulo, "", "", 1);
        menu_centered_item(autor, "", "", 2);
        menu_centered_item(quantidade_e, "", "", 3);
        menu_centered_item(quantidade_d, "", "", 4);
        menu_centered_item(categoria, "", "", 5);

        pressione_qualquer_tecla(7);
        menu_principal();
    }
    else
    {
        menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
    }
}

void menu_modificar(void)
{
    if (size_livros == 0)
    {
        clear_menu();
        menu_centered_item("Não há livros para modificar", UNDERLINE, "", 0);

        pressione_qualquer_tecla(2);
        menu_principal();
        return;
    }

    // use arrow_menu to select an already existing artigo
    // char *artigosOptions[size_artigos];
    char **livrosOptions = (char **)malloc(sizeof(char *) * size_livros);
    if (livrosOptions == NULL)
    {
        fprintf(stderr, "Erro: malloc() retornou NULL\n");
        exit(1);
    }
    for (size_t i = 0; i < size_livros; i++)
    {
        livrosOptions[i] = livros[i].titulo;
    }
    int selectedArtigo = arrow_menu(livrosOptions, size_livros);

    char isbn[40];
    char titulo[40];
    char autor[40];
    char quantidade_e[40];
    char quantidade_d[40];
    char categoria[40];

    sprintf_s(isbn, 40, "%s", livros[selectedArtigo].isbn);
    sprintf_s(titulo, 40, "%s", livros[selectedArtigo].titulo);
    sprintf_s(autor, 40, "%s", livros[selectedArtigo].autor);
    sprintf_s(quantidade_e, 40, "%d", livros[selectedArtigo].quantidade_exemplares);
    sprintf_s(quantidade_d, 40, "%d", livros[selectedArtigo].quantidade_disponivel);
    sprintf_s(categoria, 40, "%d", livros[selectedArtigo].categoria);

    Input inputItems[] = {
        {.label = "Titulo", .input = "", .isCheckbox = 0},
        {.label = "Autor", .input = "", .isCheckbox = 0},
        {.label = "ISBN", .input = "", .isCheckbox = 0},
        {.label = "Quantidade de Examplares", .input = "", .isCheckbox = 0},
        {.label = "Quantidade Disponivel", .input = "", .isCheckbox = 0},
        {.label = "Categoria", .input = "", .isCheckbox = 1, .checkBoxOptions = {"Romances", "Ficção", "Aventura", "Terror", "Biografia", "Outros"}},
    };

    // Copy the content from your character arrays to the input field

    copy_str(inputItems[0].input, titulo, strlen(titulo) + 1);
    copy_str(inputItems[1].input, autor, strlen(autor) + 1);
    copy_str(inputItems[2].input, isbn, strlen(isbn) + 1);
    copy_str(inputItems[3].input, quantidade_e, strlen(quantidade_e) + 1);
    copy_str(inputItems[4].input, quantidade_d, strlen(quantidade_d) + 1);
    copy_str(inputItems[5].input, categoria, strlen(categoria) + 1);

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
        copy_str(livros[selectedArtigo].titulo, inputItems[0].input, strlen(inputItems[0].input) + 1);
        copy_str(livros[selectedArtigo].autor, inputItems[1].input, strlen(inputItems[1].input) + 1);
        copy_str(livros[selectedArtigo].isbn, inputItems[2].input, strlen(inputItems[2].input) + 1);
        livros[selectedArtigo].quantidade_exemplares = atoi(inputItems[3].input);
        livros[selectedArtigo].quantidade_disponivel = atoi(inputItems[4].input);
        livros[selectedArtigo].categoria = atoi(inputItems[5].input);

        save_livros_array(livros, size_livros, STOCK_JSON_FILE);

        free(livrosOptions);

        clear_menu();
        menu_centered_item("Artigo modificado com sucesso!", GREEN, UNDERLINE, 0);

        pressione_qualquer_tecla(2);

        menu_principal();
        break;
    case 1:
        free(livrosOptions);
        menu_principal();
        break;
    default:
        free(livrosOptions);
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

int32_t get_category_count(void)
{
    int32_t categoryCount = 0;
    for (size_t i = 0; i < size_artigos; i++)
    {
        if ((int32_t)artigos[i].categoria > categoryCount)
        {
            categoryCount = artigos[i].categoria;
        }
    }
    return categoryCount + 1;
}

void menu_estatisticas(void)
{
    char *options[] = {
        "Stock por Categoria", // Alto e baixo preços | Quantidade maior e menor
        "Vendas",
        "Quebras", // Lixo | menu btop filtrar por categoria e total
        "Voltar"};

    int32_t totalOptions = (int)LENGTH(options);
    int32_t result = arrow_menu(options, totalOptions);
    switch (result)
    {
    case 0:
        if (size_artigos == 0)
        {
            clear_menu();
            menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
            enableRawMode();
            getchar();
            disableRawMode();
#elif _WIN32
            _getch(); // ler qualquer tecla no windows
#endif
            menu_principal();
            return;
        }
        cursor_upLeft();
        clear_menu();

        int32_t categoriaCount = get_category_count();

        int32_t *categoryCounts = (int32_t *)malloc(sizeof(int32_t) * categoriaCount);
        double *categoryTotalPrices = (double *)malloc(sizeof(double) * categoriaCount);
        if (categoryCounts == NULL || categoryTotalPrices == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }

        for (int32_t i = 0; i < categoriaCount; i++)
        {
            categoryCounts[i] = 0;
            categoryTotalPrices[i] = 0.0;
        }
        double precoTotal = 0.0;

        for (size_t i = 0; i < size_artigos; i++)
        {
            categoryCounts[artigos[i].categoria] += artigos[i].quantidade;
            for (uint64_t j = 0; j < artigos[i].quantidade; j++)
            {
                precoTotal += artigos[i].preco;
                categoryTotalPrices[artigos[i].categoria] += artigos[i].preco;
            }
        }

        char header[60];
#ifdef __unix__
        sprintf(header, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Preço");
#elif _WIN32
        sprintf_s(header, 60, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Preço");
#endif
        menu_centered_item(header, "", "", 0);

        char linhas[60] = "----------------------|------------|---------\n";
        menu_centered_item(linhas, "", "", 1);
        for (int32_t i = 0; i < categoriaCount; i++)
        {
            char *categoriaStr = categoria_to_str(i);
            char quantidade[40];
            char precoCategoriaStr[40];
            char linha[120];
#ifdef __unix__
            sprintf(quantidade, "%10d", categoryCounts[i]);
            sprintf(precoCategoriaStr, "%7.2f€", categoryTotalPrices[i]);
            sprintf(linha, "%-22s | %s | %s", categoriaStr, quantidade, precoCategoriaStr);
#elif _WIN32
            sprintf_s(quantidade, 40, "%10d", categoryCounts[i]);
            sprintf_s(precoCategoriaStr, 40, "%7.2f€", categoryTotalPrices[i]);
            sprintf_s(linha, 120, "%-22s | %s | %s", categoriaStr, quantidade, precoCategoriaStr);
#endif
            menu_centered_item(linha, "", "", i + 2);
        }

        menu_centered_item(linhas, "", "", categoriaCount + 2);
        // Print total price
        char precoTotalStr[40];
#ifdef __unix__
        sprintf(precoTotalStr, "Preço Total: %.2f\n", precoTotal);
#elif _WIN32
        sprintf_s(precoTotalStr, 40, "Preço Total: %.2f\n", precoTotal);
#endif
        menu_centered_item(precoTotalStr, BOLD, UNDERLINE, categoriaCount + 3);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", categoriaCount + 5);
#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();

        free(categoryCounts);
        free(categoryTotalPrices);

        // make a table with the quantity of each category plus the total

        break;
    case 1:
    {

        clear_menu();
        // Sem tempo para fazer isto direito :(
        if (size_artigos_vendidos == 0)
        {
            menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
            enableRawMode();
            getchar();
            disableRawMode();
#elif _WIN32
            _getch(); // ler qualquer tecla no windows

#endif
            menu_principal();
            break;
        }

        // Calcular media de todos os artigos vendidos

        double precoTotal = 0.0;
        for (size_t i = 0; i < size_artigos_vendidos; i++)
        {
            precoTotal += artigos_vendidos[i].preco * artigos_vendidos[i].quantidade;
        }

        double precoMedio = precoTotal / size_artigos_vendidos;

        char precoTotalStr[60];
#ifdef __unix__
        sprintf(precoTotalStr, "     Média dos preços dos artigos vendidos: %.2f€\n", precoMedio);

#elif _WIN32
        sprintf_s(precoTotalStr, 60, "     Média dos preços das vendas: %.2f€\n", precoMedio);
#endif

        menu_centered_item(precoTotalStr, BOLD, "", 0);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        break;
    }

    break;
    case 2:
    {
        clear_menu();
        int32_t categoriaCount = get_category_count();

        int32_t *categoryCounts = (int32_t *)malloc(sizeof(int32_t) * categoriaCount);
        double *categoryTotalPrices = (double *)malloc(sizeof(double) * categoriaCount);
        if (categoryCounts == NULL || categoryTotalPrices == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }

        for (int32_t i = 0; i < categoriaCount; i++)
        {
            categoryCounts[i] = 0;
            categoryTotalPrices[i] = 0.0;
        }

        for (size_t i = 0; i < size_artigos; i++)
        {
            categoryCounts[artigos[i].categoria] += artigos[i].quantidade;
            for (uint64_t j = 0; j < artigos[i].quantidade; j++)
            {
                categoryTotalPrices[artigos[i].categoria] += artigos[i].preco;
            }
        }

        int32_t *trashedCounts = (int32_t *)calloc(categoriaCount, sizeof(int32_t));

        double trashTotal = 0.0;
        for (int32_t i = 0; i < categoriaCount; i++)
        {
            double trashPercentage;
            switch (i)
            {
            case 0: // Ramos
                trashPercentage = 0.07;
                break;
            case 1: // Arranjos
                trashPercentage = 0.03;
                break;
            default:
                trashPercentage = 0.04;
                break;
            }
            trashedCounts[i] = (int32_t)round(categoryCounts[i] * trashPercentage);
            if (categoryCounts[i] != 0)
            {
                trashTotal += trashedCounts[i] * categoryTotalPrices[i] / categoryCounts[i];
            }
        }

        // Print the header of the table
        char header[60];
#ifdef __unix__
        sprintf(header, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Desperdício");
#elif _WIN32
        sprintf_s(header, 60, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Desperdício");
#endif
        menu_centered_item(header, "", "", 0);

        // Print the line separator
        char linhas[60];
#ifdef __unix__
        sprintf(linhas, "--------------------|------------|------------\n");
#elif _WIN32
        sprintf_s(linhas, 60, "--------------------|------------|------------\n");
#endif
        menu_centered_item(linhas, "", "", 1);

        // Print each category with its count and total price
        for (int32_t i = 0; i < categoriaCount; i++)
        {
            char *categoriaStr = categoria_to_str(i);
            char quantidade[40];
            char desperdicio[40];
            char linha[120];
#ifdef __unix__
            sprintf(quantidade, "%10d", categoryCounts[i]);
            sprintf(desperdicio, "%10d", trashedCounts[i]);
            sprintf(linha, "%-17s | %s | %s", categoriaStr, quantidade, desperdicio);
#elif _WIN32
            sprintf_s(quantidade, 40, "%10d", categoryCounts[i]);
            sprintf_s(desperdicio, 40, "%10d", trashedCounts[i]);
            sprintf_s(linha, 120, "%-17s | %s | %s", categoriaStr, quantidade, desperdicio);
#endif
            menu_centered_item(linha, "", "", i + 2);
        }

        menu_centered_item(linhas, "", "", categoriaCount + 2);

        // Print total estimated loss
        char perdaEstimadaStr[40];
#ifdef __unix__
        sprintf(perdaEstimadaStr, "Perda Estimada: %.2f€\n", trashTotal);
#elif _WIN32
        sprintf_s(perdaEstimadaStr, 40, "Perda Estimada: %.2f€\n", trashTotal);
#endif
        menu_centered_item(perdaEstimadaStr, BOLD, UNDERLINE, categoriaCount + 3);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", categoriaCount + 5);
#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();

        free(categoryCounts);
        free(categoryTotalPrices);

        // make a table with the quantity of each category plus the total

        free(trashedCounts);
        break;
    }

    case 3:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: arrow_menu() retornou %d\n", result);
        exit(1);
        break;
    }
}
void menu_simular_vendas(void)
{

    if (size_artigos == 0)
    {
        clear_menu();
        menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        return;
    }

    // use arrow_menu to select an already existing artigo
    // char *artigosOptions[size_artigos];
    char **artigosOptions = (char **)malloc(sizeof(char *) * size_artigos);
    if (artigosOptions == NULL)
    {
        fprintf(stderr, "Erro: malloc() retornou NULL\n");
        exit(1);
    }
    for (size_t i = 0; i < size_artigos; i++)
    {
        artigosOptions[i] = artigos[i].nome;
    }
    int32_t selectedArtigo = arrow_menu(artigosOptions, size_artigos);

    char preco[40];
    char quantidade[40];
    char nome[40];
    char categoria[40];
#ifdef _WIN32
    sprintf_s(preco, 40, "%.2f", artigos[selectedArtigo].preco);
    sprintf_s(nome, 40, "%s", artigos[selectedArtigo].nome);
    sprintf_s(quantidade, 40, "%lld", artigos[selectedArtigo].quantidade);
    sprintf_s(categoria, 40, "%d", artigos[selectedArtigo].categoria);

#elif __unix__
    sprintf(preco, "%.2f", artigos[selectedArtigo].preco);
    sprintf(nome, "%s", artigos[selectedArtigo].nome);
    sprintf(quantidade, "%ld", artigos[selectedArtigo].quantidade);
    sprintf(categoria, "%d", artigos[selectedArtigo].categoria);
#endif

    Input inputItems[] = {
        {.label = "Nome", .isCheckbox = 0},
        {.label = "Preço (€)", .isCheckbox = 0},
        {.label = "Quantidade", .isCheckbox = 0},
        {.label = "Categoria", .isCheckbox = 1, .checkBoxOptions = {"Ramos", "Arranjos", "Jarros", "CentrosMesa", "OutrasFlores"}},
    };

    // Copy the content from your character arrays to the input field

    copy_str(inputItems[0].input, nome, strlen(nome) + 1);
    copy_str(inputItems[1].input, preco, strlen(preco) + 1);
    copy_str(inputItems[2].input, quantidade, strlen(quantidade) + 1);
    copy_str(inputItems[3].input, categoria, strlen(categoria) + 1);

    int32_t result = input_menu(inputItems, LENGTH(inputItems), 1);
    switch (result)
    {
    case 0:
        // realloc artigos_vendidos
        size_artigos_vendidos++;
        artigos_vendidos = realloc(artigos_vendidos, sizeof(Artigo) * (size_artigos_vendidos));
        if (artigos_vendidos == NULL)
        {
            fprintf(stderr, "Erro: realloc() retornou NULL\n");
            exit(1);
        }

        // copy the selected artigo to artigos_vendidos
        artigos_vendidos[size_artigos_vendidos - 1].nome = (char *)malloc(sizeof(char) * (strlen(artigos[selectedArtigo].nome) + 1));
        if (artigos_vendidos[size_artigos_vendidos - 1].nome == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }

        copy_str(artigos_vendidos[size_artigos_vendidos - 1].nome, artigos[selectedArtigo].nome, strlen(artigos[selectedArtigo].nome) + 1);
        artigos_vendidos[size_artigos_vendidos - 1].preco = artigos[selectedArtigo].preco;
        artigos_vendidos[size_artigos_vendidos - 1].quantidade = atoi(inputItems[2].input);
        artigos_vendidos[size_artigos_vendidos - 1].categoria = artigos[selectedArtigo].categoria;
        copy_str(artigos_vendidos[size_artigos_vendidos - 1].uuid, uuid_gen(), 37);

        // subtract the quantity from the selected artigo
        artigos[selectedArtigo].quantidade -= atoi(inputItems[2].input);

        // save the artigos array to the json file
        save_artigos_array(artigos, size_artigos, STOCK_JSON_FILE);

        // save the artigos_vendidos array to the json file
        save_artigos_array(artigos_vendidos, size_artigos_vendidos, VENDAS_JSON_FILE);

        free(artigosOptions);
        clear_menu();

        menu_centered_item("Venda simulada com sucesso!", GREEN, UNDERLINE, 0);
        // total a pagar

        char precoTotalStr[40];
#ifdef __unix__
        sprintf(precoTotalStr, "Total a pagar: %.2f€\n", artigos_vendidos[size_artigos_vendidos - 1].preco * artigos_vendidos[size_artigos_vendidos - 1].quantidade);
#elif _WIN32
        sprintf_s(precoTotalStr, 40, "Total a pagar: %.2f€\n", artigos_vendidos[size_artigos_vendidos - 1].preco * artigos_vendidos[size_artigos_vendidos - 1].quantidade);
#endif
        menu_centered_item(precoTotalStr, BOLD, UNDERLINE, 1);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 2);
#ifdef __unix__ // ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        break;
    case 1:
        free(artigosOptions);
        menu_principal();
        break;
    default:
        free(artigosOptions);
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);

        break;
    }
}

// return 1 se a data for valida, 0 se não for
int verificar_data(int dia, int mes, int ano)
{

    if (ano >= 1900 && ano <= 9999)
    {
        if (mes >= 1 && mes <= 12)
        {
            if (mes == 2)
            {
                if (ano % 4 == 0)
                {
                    if (dia >= 1 && dia <= 29)
                    {
                        return 1;
                    }
                }
                else
                {
                    if (dia >= 1 && dia <= 28)
                    {
                        return 1;
                    }
                }
            }
            else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
            {
                if (dia >= 1 && dia <= 30)
                {
                    return 1;
                }
            }
            else
            {
                if (dia >= 1 && dia <= 31)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void mudar_data(void)
{
    Input inputItems[] = {
        {.label = "Dia", .input = "", .isCheckbox = 0},
        {.label = "Mês", .input = "", .isCheckbox = 0},
        {.label = "Ano", .input = "", .isCheckbox = 0},
    };

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
        int dia = atoi(inputItems[0].input);
        int mes = atoi(inputItems[1].input);
        int ano = atoi(inputItems[2].input);

        if (verificar_data(dia, mes, ano) == 0)
        {
            clear_menu();
            menu_centered_item("Data inválida!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            mudar_data();
            return;
        }

        // mudar current_datetime

        current_datetime.tm_mday = dia;
        current_datetime.tm_mon = mes - 1;
        current_datetime.tm_year = ano - 1900;
        // TODO ver data json e tal

        clear_menu();
        menu_centered_item("Data alterada com sucesso!", GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

        pressione_qualquer_tecla(3);

        menu_principal();

        break;
    case 1:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

void menu_principal(void)
{

    char *options[] = {
        "Introduzir informação sobre um livro",
        "Introduzir empréstimo",
        "Registar devolução",
        "Listar Livros (ISBN)",
        "Modificar Livro",
        "Estatisticas",
        "Mudar Data"
        "Sair"};

    int totalOptions = (int)LENGTH(options);
    int result = arrow_menu(options, totalOptions);
    clear_menu();
    switch (result)
    {
    case 0:
        menu_introduzir_livro();
        break;
    case 1:
        menu_introduzir_emprestimo();
        break;
    case 2:
        menu_listar_livros();
        break;
    case 3:
        menu_modificar();
        break;
    case 4:
        menu_estatisticas();
        break;
    case 5:
        menu_simular_vendas();
        break;
    case 6:
        mudar_data();
        break;
    case 7:
        return;
        break;
    default:
        exit(1);
        break;
    }
}