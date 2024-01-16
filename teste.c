// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>

// typedef enum
// {
//     Romances,
//     Ficcao,
//     Aventura,
//     Terror,
//     Biografia,
//     Outros
// } CategoriaLivro;

// typedef struct
// {
//     char isbn[14];
//     char *titulo;
//     char *autor;
//     int quantidade_exemplares;
//     int quantidade_disponivel;
//     int num_requisicoes;
//     CategoriaLivro categoria;
// } Livro;

// typedef struct
// {
//     Livro *livro;
//     char uuid[37];
//     long long num_cc;
//     int ja_devolvido;
//     struct tm data_emprestimo;
//     struct tm data_devolucao;
// } Emprestimo;

// typedef struct
// {
//     long long num_cc;
//     int num_emprestimos;
// } PessoaEmprestimos;

// int contarEmprestimosPorPessoa(Emprestimo *emprestimos, int num_emprestimos, PessoaEmprestimos **pessoas, int *index_max_emprestimos)
// {
//     *pessoas = (PessoaEmprestimos *)malloc(num_emprestimos * sizeof(PessoaEmprestimos));

//     // Inicializar a matriz de pessoas
//     for (int i = 0; i < num_emprestimos; i++)
//     {
//         (*pessoas)[i].num_cc = emprestimos[i].num_cc;
//         (*pessoas)[i].num_emprestimos = 0;
//     }

//     // Contar o número de empréstimos por pessoa
//     for (int i = 0; i < num_emprestimos; i++)
//     {
//         long long num_cc = emprestimos[i].num_cc;

//         // Encontrar a entrada correspondente em pessoas
//         for (int j = 0; j < num_emprestimos; j++)
//         {
//             if ((*pessoas)[j].num_cc == num_cc)
//             {
//                 (*pessoas)[j].num_emprestimos++;
//                 break;
//             }
//         }
//     }

//     // Encontrar a pessoa com mais empréstimos
//     int max_emprestimos = 0;

//     for (int i = 0; i < num_emprestimos; i++)
//     {
//         if ((*pessoas)[i].num_emprestimos > max_emprestimos)
//         {
//             max_emprestimos = (*pessoas)[i].num_emprestimos;
//             *index_max_emprestimos = i;
//         }
//     }

//     return max_emprestimos;
// }

// int main()
// {
//     // Assuming you have an array of Emprestimo representing the loans
//     Emprestimo *emprestimos;
//     emprestimos = (Emprestimo *)malloc(sizeof(Emprestimo) * 10);
//     emprestimos[0] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     emprestimos[1] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     emprestimos[2] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     emprestimos[4] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     emprestimos[5] = (Emprestimo){.num_cc = 915694209, .ja_devolvido = 0};
//     emprestimos[6] = (Emprestimo){.num_cc = 915694209, .ja_devolvido = 0};
//     emprestimos[7] = (Emprestimo){.num_cc = 915694209, .ja_devolvido = 0};
//     emprestimos[8] = (Emprestimo){.num_cc = 915694209, .ja_devolvido = 0};
//     emprestimos[9] = (Emprestimo){.num_cc = 915694209, .ja_devolvido = 0};

//     int num_emprestimos = 10;

//     PessoaEmprestimos *pessoas;
//     int index_pessoa_max_emprestimos;

//     int max_emprestimos = contarEmprestimosPorPessoa(emprestimos, num_emprestimos, &pessoas, &index_pessoa_max_emprestimos);

//     // Display the result
//     if (max_emprestimos > 0)
//     {
//         printf("Person with most loans:\n");
//         printf("Num CC: %lld\n", pessoas[index_pessoa_max_emprestimos].num_cc);
//         printf("Number of loans: %d\n", pessoas[index_pessoa_max_emprestimos].num_emprestimos);
//     }
//     else
//     {
//         printf("No loans found.\n");
//     }

//     printf("\n\n");

//     // adicionar mais empréstimos
//     emprestimos = (Emprestimo *)realloc(emprestimos, sizeof(Emprestimo) * (num_emprestimos + 3));
//     emprestimos[num_emprestimos] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     emprestimos[num_emprestimos + 1] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     emprestimos[num_emprestimos + 2] = (Emprestimo){.num_cc = 123456789, .ja_devolvido = 0};
//     num_emprestimos += 3;

//     // Recontar os empréstimos
//     max_emprestimos = contarEmprestimosPorPessoa(emprestimos, num_emprestimos, &pessoas, &index_pessoa_max_emprestimos);

//     // Display the result
//     printf("---------------------\n");
//     if (max_emprestimos > 0)
//     {
//         printf("Person with most loans:\n");
//         printf("Num CC: %lld\n", pessoas[index_pessoa_max_emprestimos].num_cc);
//         printf("Number of loans: %d\n", pessoas[index_pessoa_max_emprestimos].num_emprestimos);
//     }
//     else
//     {
//         printf("No loans found.\n");
//     }
//     printf("---------------------\n\n");

//     for (int i = 0; i < num_emprestimos; i++)
//     {
//         printf("Num CC: %lld\n", pessoas[i].num_cc);
//         printf("Number of loans: %d\n", pessoas[i].num_emprestimos);
//     }

//     // Free the dynamically allocated memory
//     free(pessoas);

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    const char *dateString = "00-00-0000";
    int day, month, year;

    struct tm tm = {0};

    // Use sscanf to parse the string
    sscanf(dateString, "%d-%d-%d", &day, &month, &year);

    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;

    // Print the parsed values
    printf("Date: %02d-%02d-%04d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return 0;
}
