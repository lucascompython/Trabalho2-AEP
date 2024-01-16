#ifndef JSON_H
#define JSON_H

#include "main.h"
#include <stdlib.h>
#define STOCK_JSON_FILE "stock.json"
#define EMPRESTIMOS_JSON_FILE "emprestimos.json"

Livro *get_livros_array(size_t *size_livros, const char *json_file);
Emprestimo *get_emprestimos_array(size_t *size_emprestimos, const char *json_file);
void free_livros_array(Livro *livros, size_t size);
CategoriaLivro str_to_categoria(const char *str);
char *categoria_to_str(CategoriaLivro categoria);
void save_livros_array(Livro *livros, size_t size_livros, const char *json_file);
void save_emprestimos_array(Emprestimo *emprestimos, size_t size_emprestimos,
                            const char *json_file);
void copy_str(char *dest, const char *src, size_t size);

#endif // JSON_H
