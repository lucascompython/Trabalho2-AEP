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
    {

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
    }
    case 1:
    {

        menu_principal();
        break;
    }
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

void menu_modificar_livro(void)
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
    char num_requisicoes[40];
    char categoria[40];

    sprintf_s(isbn, 40, "%s", livros[selectedArtigo].isbn);
    sprintf_s(titulo, 40, "%s", livros[selectedArtigo].titulo);
    sprintf_s(autor, 40, "%s", livros[selectedArtigo].autor);
    sprintf_s(quantidade_e, 40, "%d", livros[selectedArtigo].quantidade_exemplares);
    sprintf_s(quantidade_d, 40, "%d", livros[selectedArtigo].quantidade_disponivel);
    sprintf_s(num_requisicoes, 40, "%d", livros[selectedArtigo].num_requisicoes);
    sprintf_s(categoria, 40, "%d", livros[selectedArtigo].categoria);

    Input inputItems[] = {
        {.label = "Titulo", .input = "", .isCheckbox = 0},
        {.label = "Autor", .input = "", .isCheckbox = 0},
        {.label = "ISBN", .input = "", .isCheckbox = 0},
        {.label = "Quantidade de Examplares", .input = "", .isCheckbox = 0},
        {.label = "Quantidade Disponivel", .input = "", .isCheckbox = 0},
        {.label = "Número de Requisições", .input = "", .isCheckbox = 0},
        {.label = "Categoria", .input = "", .isCheckbox = 1, .checkBoxOptions = {"Romances", "Ficção", "Aventura", "Terror", "Biografia", "Outros"}},
    };

    // Copy the content from your character arrays to the input field

    copy_str(inputItems[0].input, titulo, strlen(titulo) + 1);
    copy_str(inputItems[1].input, autor, strlen(autor) + 1);
    copy_str(inputItems[2].input, isbn, strlen(isbn) + 1);
    copy_str(inputItems[3].input, quantidade_e, strlen(quantidade_e) + 1);
    copy_str(inputItems[4].input, quantidade_d, strlen(quantidade_d) + 1);
    copy_str(inputItems[5].input, num_requisicoes, strlen(num_requisicoes) + 1);
    copy_str(inputItems[6].input, categoria, strlen(categoria) + 1);

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
        copy_str(livros[selectedArtigo].titulo, inputItems[0].input, strlen(inputItems[0].input) + 1);
        copy_str(livros[selectedArtigo].autor, inputItems[1].input, strlen(inputItems[1].input) + 1);
        copy_str(livros[selectedArtigo].isbn, inputItems[2].input, strlen(inputItems[2].input) + 1);
        livros[selectedArtigo].quantidade_exemplares = atoi(inputItems[3].input);
        livros[selectedArtigo].quantidade_disponivel = atoi(inputItems[4].input);
        livros[selectedArtigo].num_requisicoes = atoi(inputItems[5].input);
        livros[selectedArtigo].categoria = atoi(inputItems[6].input);

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

// int32_t get_category_count(void)
// {
//     int32_t categoryCount = 0;
//     for (size_t i = 0; i < size_artigos; i++)
//     {
//         if ((int32_t)artigos[i].categoria > categoryCount)
//         {
//             categoryCount = artigos[i].categoria;
//         }
//     }
//     return categoryCount + 1;
// }

void menu_quantos_livros_emprestados_por_pessoa(void)
{
    Input inputItems[] = {
        {.label = "Número de CC", .input = "", .isCheckbox = 0},
    };

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
    {

        int num_cc = atoi(inputItems[0].input);

        // Verificar se o utilizador tem mais de 3 livros emprestados
        int livros_emprestados = 0;
        for (size_t i = 0; i < size_emprestimos; i++)
        {
            if (emprestimos[i].num_cc == num_cc)
            {
                livros_emprestados++;
            }
        }

        clear_menu();
        char string[40];
        sprintf_s(string, 40, "O utilizador tem %d livros emprestados", livros_emprestados);
        menu_centered_item(string, GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

        pressione_qualquer_tecla(3);
        menu_principal();
        break;
    }
    case 1:
        menu_principal();
        break;

    default:
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

void menu_livros_emprestados_entre_datas(void) {}
void menu_pessoa_que_requisitou_mais_livros(void) {}
void menu_livros_requisitados_em_cada_mes(void) {}
void menu_livros_requisitados_por_categoria(void) {}
void menu_livros_menos_requisitados(void) {}
void menu_quebras(void) {}

void menu_estatisticas(void)
{
    char *options[] = {
        "Quantos Livros Emprestados por Pessoa",
        "Livros Emprestados Entre Datas",
        "Pessoa Que Requisitou Mais Livros",
        "Livros Requisitados em Cada Mês",
        "Livros Requisitados por Categoria",
        "Livros Menos Requisitados",
        "Quebras", // Lixo | menu btop filtrar por categoria e total
        "Voltar"};

    int totalOptions = (int)LENGTH(options);
    int result = arrow_menu(options, totalOptions);

    switch (result)
    {
    case 0:
        menu_quantos_livros_emprestados_por_pessoa();
        break;
    case 1:
        menu_livros_emprestados_entre_datas();
        break;
    case 2:
        menu_pessoa_que_requisitou_mais_livros();
        break;
    case 3:
        menu_livros_requisitados_em_cada_mes();
        break;
    case 4:
        menu_livros_requisitados_por_categoria();
        break;
    case 5:
        menu_livros_menos_requisitados();
        break;
    case 6:
        menu_quebras();
        break;
    case 7:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: arrow_menu() retornou %d\n", result);
        exit(1);
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
    {
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
    }
    case 1:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

void menu_resgistrar_devolucao(void)
{
    Input inputItems[] = {
        {.label = "Número de CC", .input = "", .isCheckbox = 0},
        {.label = "ISBN", .input = "", .isCheckbox = 0}};

    int result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
    {

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

        // Verificar se o utilizador existe
        int utilizadorIndex = -1;
        for (size_t i = 0; i < size_emprestimos; i++)
        {
            if (emprestimos[i].num_cc == num_cc)
            {
                utilizadorIndex = i;
                break;
            }
        }
        if (utilizadorIndex == -1)
        {
            clear_menu();
            menu_centered_item("Utilizador não encontrado!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            menu_principal();
            break;
        }

        // Verificar se o utilizador tem o livro emprestado
        int emprestimoIndex = -1;
        for (size_t i = 0; i < size_emprestimos; i++)
        {
            if (emprestimos[i].num_cc == num_cc && strcmp(emprestimos[i].livro->isbn, inputItems[1].input) == 0)
            {
                emprestimoIndex = i;
                break;
            }
        }
        if (emprestimoIndex == -1)
        {
            clear_menu();
            menu_centered_item("O utilizador não tem o livro emprestado!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            menu_principal();
            break;
        }

        // Verificar se o livro já foi devolvido
        if (emprestimos[emprestimoIndex].ja_devolvido == 1)
        {
            clear_menu();
            menu_centered_item("O livro já foi devolvido!", RED, UNDERLINE, 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

            pressione_qualquer_tecla(3);
            menu_principal();
            break;
        }

        emprestimos[emprestimoIndex].ja_devolvido = 1;
        emprestimos[emprestimoIndex].data_devolucao = current_datetime;

        livros[livroIndex].quantidade_disponivel++;

        save_livros_array(livros, size_livros, STOCK_JSON_FILE);
        save_emprestimos_array(emprestimos, size_emprestimos, EMPRESTIMOS_JSON_FILE);

        clear_menu();
        menu_centered_item("Devolução registada com sucesso!", GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
        pressione_qualquer_tecla(3);
    }
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
        "Mudar Data",
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
        menu_resgistrar_devolucao();
        break;
    case 3:
        menu_listar_livros();
        break;
    case 4:
        menu_modificar_livro();
        break;
    case 5:
        menu_estatisticas();
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