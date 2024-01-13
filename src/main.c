#include "main.h"
#include "colors.h"
#include "menu.h"
#include "json.h"
#include "term_size.h"
#include "uuid.h"
#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h> // Para a welcome screen _getch() ler todas as teclas
#include <time.h>  // Para o srand() para uuid_gen()
#endif

#ifdef _WIN32
void restoreCursor(
    int sig) // Windows queixa-se que os paremetros não são os mesmos
{
    UNUSED(sig); // unused parameter, tirar warning do windows
    show_cursor();
}
#elif __unix__
void restoreCursor() { show_cursor(); }
#endif

TerminalSize term_size;
Livro *livros;
size_t size_livros;
Emprestimo *emprestimos;
size_t size_emprestimos;

struct tm current_datetime;

void welcome_screen(void)
{
    clear_menu();

    menu_centered_item("Bem vindo ao sistema de gestão de emprestimos", GREEN, BOLD,
                       0);
    menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "",
                       1);

    pressione_qualquer_tecla(2);

    clear_menu();
    menu_principal();
}

/*
Introduzir informação de um livro
Introduzir um emprestimo
Modificar a informação de um livro
Registar a devolução de um livro

Mudar a data (atual) do sistema


Estatisticas
 - Informação de um livro pelo ISBN (como tabela antiga AKA mostrar tudo)
//  - Ver quantos livros estão emprestados por um utilizador
 - Tabela clientes (mostrar por utilizador, livros requisitados, livros devolvidos, livros emprestados)
 -

 - Quantos livros foram emprestados entre duas datas, e quantos foram devolvidos
 - Apresenter a informação da pessoa que requisitou mais livros e quantos ja devolveu (talvez tabela com todos os utilizadores por ordem decrescente)
 - Apresenter quantidade de livros requisitados, por mes de um dado ano
 - Apresentar a informação dos livros menos requisitados
 - Estragos


*/

int main(void)
{
    setlocale(LC_ALL, "Portuguese.UTF8");

    // set current time
    time_t t = time(NULL);
    current_datetime = *localtime(&t);

#ifdef _WIN32
    srand((unsigned int)time(NULL)); // seed para o uuid_gen() no Windows
#endif
    hide_cursor();

    // restaure o cursor quando o programa terminar
    signal(SIGINT, restoreCursor);  // Ctrl + C
    signal(SIGTERM, restoreCursor); // kill
    signal(SIGSEGV, restoreCursor); // Segmentation fault
    atexit(show_cursor);            // Quando exit() é chamado | Não é 'restoreCursor' porque o Windows queixa-se que os paremetros não são os mesmos

    if (get_terminal_size(&term_size) != 0)
    {
        fprintf(stderr, "Erro a conseguir o tamanho do terminal...\n");
        return 1;
    }

    livros = get_livros_array(&size_livros, STOCK_JSON_FILE);
    emprestimos = get_emprestimos_array(&size_emprestimos,
                                        EMPRESTIMOS_JSON_FILE);

    welcome_screen();
    show_cursor(); // Restaurar o cursor ao sair. Não é 'restoreCursor' porque o Windows queixa-se que os paremetros não são os mesmos

    // save_artigos_array(artigos, size_artigos, STOCK_JSON_FILE);
    // save_artigos_array(artigos_vendidos, size_artigos_vendidos,
    //                    VENDAS_JSON_FILE);
    free_livros_array(livros, size_livros);
    free_emprestimos_array(emprestimos, size_emprestimos);

    menu_centered_item("Obrigado por usar o nosso programa!\n", GREEN, BOLD, 0);
    return 0;
}
