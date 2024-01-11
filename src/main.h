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
  CategoriaLivro categoria;

} Livro;

typedef struct
{
  Livro *livro;
  char uuid[37];
  int num_cc;
  int ja_devolvido; // 0 - não devolvido, 1 - devolvido
  time_t data_emprestimo;
  time_t data_devolucao;
} Emprestimo;

#endif