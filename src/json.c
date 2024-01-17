#include "json.h"
#include "yyjson.h" // external/yyjson/src
#include "main.h"

CategoriaLivro str_to_categoria(const char *str)
{
    if (strcmp(str, "Romances") == 0)
    {
        return Romances;
    }
    else if (strcmp(str, "Ficção") == 0)
    {
        return Ficcao;
    }
    else if (strcmp(str, "Aventura") == 0)
    {
        return Aventura;
    }
    else if (strcmp(str, "Terror") == 0)
    {
        return Terror;
    }
    else if (strcmp(str, "Biografia") == 0)
    {
        return Biografia;
    }
    else if (strcmp(str, "Outros") == 0)
    {
        return Outros;
    }

    else
    {
        fprintf(stderr, "Categoria inválida\n");
        exit(1);
    }
}

char *categoria_to_str(CategoriaLivro categoria)
{
    switch (categoria)
    {
    case Romances:
        return "Romances";
    case Ficcao:
        return "Ficção";
    case Aventura:
        return "Aventura";
    case Terror:
        return "Terror";
    case Biografia:
        return "Biografia";
    case Outros:
        return "Outros";
    default:
        fprintf(stderr, "Categoria inválida\n");
        exit(1);
    }
}

void free_livros_array(Livro *livros, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(livros[i].titulo);
        free(livros[i].autor);
    }
    free(livros);
}

void copy_str(char *dest, const char *src, size_t size)
{
#ifdef __unix__
    strncpy(dest, src, size);
#elif _WIN32
    strcpy_s(dest, size, src);
#endif
}

void create_json_file(const char *json_file)
{

#ifdef __unix__
    FILE *f = fopen(json_file, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao criar o ficheiro JSON: (%s)\n", json_file);
        exit(1);
    }
#elif _WIN32
    FILE *f;
    errno_t err = fopen_s(&f, json_file, "w");
    if (err != 0)
    {
        fprintf(stderr, "Erro ao criar o ficheiro JSON: (%s)\n", json_file);
        exit(1);
    }
#endif
    fprintf(f, "{}");
    fclose(f);
}

Livro *get_livros_array(size_t *size_livros, const char *json_file)
{
    yyjson_doc *doc = yyjson_read_file(json_file, 0, NULL, NULL);
    if (!doc)
    {
        create_json_file(json_file);
        doc = yyjson_read_file(json_file, 0, NULL, NULL);
    }

    yyjson_val *root = yyjson_doc_get_root(doc);

    size_t size = yyjson_obj_size(root);
    *size_livros = size;

    Livro *livros = (Livro *)malloc(sizeof(Livro) * size);
    if (livros == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array de livros\n");
        exit(1);
    }

    size_t idx, max;
    yyjson_val *key, *val;

    yyjson_obj_foreach(root, idx, max, key, val)
    {
        const char *isbn = yyjson_get_str(key);

        const char *titulo = yyjson_get_str(yyjson_obj_get(val, "titulo"));
        const char *autor = yyjson_get_str(yyjson_obj_get(val, "autor"));

        int quantidade_exemplares = yyjson_get_int(yyjson_obj_get(val, "quantidade_exemplares"));
        int quantidade_disponivel = yyjson_get_int(yyjson_obj_get(val, "quantidade_disponivel"));
        int num_requisicoes = yyjson_get_int(yyjson_obj_get(val, "num_requisicoes"));
        const char *categoria = yyjson_get_str(yyjson_obj_get(val, "categoria"));

        size_t titulo_size = strlen(titulo) + 1;
        size_t autor_size = strlen(autor) + 1;

        livros[idx].titulo = (char *)malloc(titulo_size);
        if (livros[idx].titulo == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o titulo do artigo\n");
            exit(1);
        }

        livros[idx].autor = (char *)malloc(autor_size);
        if (livros[idx].autor == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o autor do artigo\n");
            exit(1);
        }

        copy_str(livros[idx].isbn, isbn, 14);
        copy_str(livros[idx].titulo, titulo, titulo_size);
        copy_str(livros[idx].autor, autor, autor_size);

        livros[idx].quantidade_exemplares = quantidade_exemplares;
        livros[idx].quantidade_disponivel = quantidade_disponivel;
        livros[idx].num_requisicoes = num_requisicoes;
        livros[idx].categoria = str_to_categoria(categoria);
    }
    yyjson_doc_free(doc);
    return livros;
}

Emprestimo *get_emprestimos_array(size_t *size_emprestimos, const char *json_file)
{
    yyjson_doc *doc = yyjson_read_file(json_file, 0, NULL, NULL);
    if (!doc)
    {
        create_json_file(json_file);
        doc = yyjson_read_file(json_file, 0, NULL, NULL);
    }

    yyjson_val *root = yyjson_doc_get_root(doc);

    size_t size = yyjson_obj_size(root);
    *size_emprestimos = size;

    Emprestimo *emprestimos = (Emprestimo *)malloc(sizeof(Emprestimo) * size);
    if (emprestimos == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array de emprestimos\n");
        exit(1);
    }

    size_t idx, max;
    yyjson_val *key, *val;

    yyjson_obj_foreach(root, idx, max, key, val)
    {
        const char *uuid = yyjson_get_str(key);

        int num_cc = yyjson_get_int(yyjson_obj_get(val, "num_cc"));
        int ja_devolvido = yyjson_get_int(yyjson_obj_get(val, "ja_devolvido"));

        const char *data_emprestimo_str = yyjson_get_str(yyjson_obj_get(val, "data_emprestimo"));
        const char *data_devolucao_str = yyjson_get_str(yyjson_obj_get(val, "data_devolucao"));

        struct tm data_emprestimo = {0};
        struct tm data_devolucao = {0};

        // strptime deixou de ser standard em C11
        int d, m, y;
        sscanf(data_emprestimo_str, "%d-%d-%d", &d, &m, &y);
        data_emprestimo.tm_mday = d;
        data_emprestimo.tm_mon = m - 1;
        data_emprestimo.tm_year = y - 1900;

        sscanf(data_devolucao_str, "%d-%d-%d", &d, &m, &y);
        data_devolucao.tm_mday = d;
        data_devolucao.tm_mon = m - 1;
        data_devolucao.tm_year = y - 1900;

        copy_str(emprestimos[idx].uuid, uuid, 37);
        emprestimos[idx].num_cc = num_cc;
        emprestimos[idx].ja_devolvido = ja_devolvido;
        emprestimos[idx].data_emprestimo = data_emprestimo;
        emprestimos[idx].data_devolucao = data_devolucao;

        // ler o livro
        yyjson_val *livro = yyjson_obj_get(val, "livro");
        const char *isbn = yyjson_get_str(yyjson_obj_get(livro, "isbn"));
        const char *titulo = yyjson_get_str(yyjson_obj_get(livro, "titulo"));
        const char *autor = yyjson_get_str(yyjson_obj_get(livro, "autor"));
        int quantidade_exemplares = yyjson_get_int(yyjson_obj_get(livro, "quantidade_exemplares"));
        int quantidade_disponivel = yyjson_get_int(yyjson_obj_get(livro, "quantidade_disponivel"));
        int num_requisicoes = yyjson_get_int(yyjson_obj_get(livro, "num_requisicoes"));
        const char *categoria = yyjson_get_str(yyjson_obj_get(livro, "categoria"));

        size_t titulo_size = strlen(titulo) + 1;
        size_t autor_size = strlen(autor) + 1;

        emprestimos[idx].livro = (Livro *)malloc(sizeof(Livro));
        if (emprestimos[idx].livro == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o livro do emprestimo\n");
            exit(1);
        }

        emprestimos[idx].livro->titulo = (char *)malloc(titulo_size);
        if (emprestimos[idx].livro->titulo == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o titulo do livro do emprestimo\n");
            exit(1);
        }

        emprestimos[idx].livro->autor = (char *)malloc(autor_size);
        if (emprestimos[idx].livro->autor == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o autor do livro do emprestimo\n");
            exit(1);
        }

        copy_str(emprestimos[idx].livro->isbn, isbn, 14);
        copy_str(emprestimos[idx].livro->titulo, titulo, titulo_size);
        copy_str(emprestimos[idx].livro->autor, autor, autor_size);

        emprestimos[idx].livro->quantidade_exemplares = quantidade_exemplares;
        emprestimos[idx].livro->quantidade_disponivel = quantidade_disponivel;
        emprestimos[idx].livro->num_requisicoes = num_requisicoes;
        emprestimos[idx].livro->categoria = str_to_categoria(categoria);
    }
    yyjson_doc_free(doc);
    return emprestimos;
}

void save_livros_array(Livro *livros, size_t size, const char *json_file)
{
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    for (size_t i = 0; i < size; i++)
    {
        yyjson_mut_val *key = yyjson_mut_strn(doc, livros[i].isbn, strlen(livros[i].isbn));
        yyjson_mut_val *obj = yyjson_mut_obj(doc);

        yyjson_mut_val *titulo = yyjson_mut_strn(doc, livros[i].titulo, strlen(livros[i].titulo));
        yyjson_mut_val *autor = yyjson_mut_strn(doc, livros[i].autor, strlen(livros[i].autor));
        yyjson_mut_val *quantidade_exemplares = yyjson_mut_int(doc, livros[i].quantidade_exemplares);
        yyjson_mut_val *quantidade_disponivel = yyjson_mut_int(doc, livros[i].quantidade_disponivel);
        yyjson_mut_val *num_requisicoes = yyjson_mut_int(doc, livros[i].num_requisicoes);
        yyjson_mut_val *categoria = yyjson_mut_strn(doc, categoria_to_str(livros[i].categoria), strlen(categoria_to_str(livros[i].categoria)));

        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "titulo"), titulo);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "autor"), autor);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "quantidade_exemplares"), quantidade_exemplares);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "quantidade_disponivel"), quantidade_disponivel);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "num_requisicoes"), num_requisicoes);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "categoria"), categoria);

        yyjson_mut_obj_add(root, key, obj);
    }

    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_write_file(json_file, doc, YYJSON_WRITE_PRETTY, NULL, NULL);
    yyjson_mut_doc_free(doc);
}

void save_emprestimos_array(Emprestimo *emprestimos, size_t size, const char *json_file)
{

    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);

    for (size_t i = 0; i < size; i++)
    {
        yyjson_mut_val *key = yyjson_mut_strn(doc, emprestimos[i].uuid, strlen(emprestimos[i].uuid));
        yyjson_mut_val *obj = yyjson_mut_obj(doc);

        yyjson_mut_val *num_cc = yyjson_mut_int(doc, emprestimos[i].num_cc);
        yyjson_mut_val *ja_devolvido = yyjson_mut_int(doc, emprestimos[i].ja_devolvido);

        // save date as str %d-%m-%Y
        char data_emprestimo[11];
        char data_devolucao[11];
        strftime(data_emprestimo, 11, "%d-%m-%Y", &emprestimos[i].data_emprestimo);

        // check if data_devolucao is valid
        if (emprestimos[i].ja_devolvido)
        {
            strftime(data_devolucao, 11, "%d-%m-%Y", &emprestimos[i].data_devolucao);
        }
        else
        {
            strcpy(data_devolucao, "00-00-0000");
        }

        yyjson_mut_val *data_emprestimo_val = yyjson_mut_strn(doc, data_emprestimo, 10);
        yyjson_mut_val *data_devolucao_val = yyjson_mut_strn(doc, data_devolucao, 10);

        yyjson_mut_val *livro = yyjson_mut_obj(doc);

        yyjson_mut_val *isbn = yyjson_mut_strn(doc, emprestimos[i].livro->isbn, strlen(emprestimos[i].livro->isbn));
        yyjson_mut_val *titulo = yyjson_mut_strn(doc, emprestimos[i].livro->titulo, strlen(emprestimos[i].livro->titulo));
        yyjson_mut_val *autor = yyjson_mut_strn(doc, emprestimos[i].livro->autor, strlen(emprestimos[i].livro->autor));
        yyjson_mut_val *quantidade_exemplares = yyjson_mut_int(doc, emprestimos[i].livro->quantidade_exemplares);
        yyjson_mut_val *quantidade_disponivel = yyjson_mut_int(doc, emprestimos[i].livro->quantidade_disponivel);
        yyjson_mut_val *num_requisicoes = yyjson_mut_int(doc, emprestimos[i].livro->num_requisicoes);
        yyjson_mut_val *categoria = yyjson_mut_strn(doc, categoria_to_str(emprestimos[i].livro->categoria), strlen(categoria_to_str(emprestimos[i].livro->categoria)));

        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "isbn"), isbn);
        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "titulo"), titulo);
        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "autor"), autor);
        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "quantidade_exemplares"), quantidade_exemplares);
        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "quantidade_disponivel"), quantidade_disponivel);
        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "num_requisicoes"), num_requisicoes);
        yyjson_mut_obj_add(livro, yyjson_mut_str(doc, "categoria"), categoria);

        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "num_cc"), num_cc);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "ja_devolvido"), ja_devolvido);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "data_emprestimo"), data_emprestimo_val);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "data_devolucao"), data_devolucao_val);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "livro"), livro);

        yyjson_mut_obj_add(root, key, obj);
    }
    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_write_file(json_file, doc, YYJSON_WRITE_PRETTY, NULL, NULL);
    yyjson_mut_doc_free(doc);
}