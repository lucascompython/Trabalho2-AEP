#ifndef MAIN_H
#define MAIN_H

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define UNUSED(x) (void)(x)

#include <time.h>

typedef enum
{
  Romances,
  Ficcao,
  Aventura,
  Terror,
  Biografia,
  Outros
} CategoriaLivro;

typedef struct
{
  char isbn[14];
  char *titulo;
  char *autor;
  int quantidade_exemplares;
  int quantidade_disponivel;
  int num_requisicoes;
  CategoriaLivro categoria;

} Livro;

typedef struct
{
  Livro *livro;
  char uuid[37];
  int num_cc;
  int ja_devolvido; // 0 - não devolvido, 1 - devolvido
  struct tm data_emprestimo;
  struct tm data_devolucao;
} Emprestimo;

typedef struct
{
  int num_cc;
  int num_requisicoes;
} PessoaRequisicoes;

int contarEmprestimosPorPessoa(int *index_max_emprestimos);

#endif