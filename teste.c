#include <stdio.h>
#include <time.h>

int verificar_data(int dia, int mes, int ano)
{
    // Verificar se o ano é bissexto
    // Verificar se o dia é valido para o mes (ex: 31 de fevereiro; 31 de abril)
    // return 1 se a data for valida, 0 se não for

    // ano
    if (ano > 0)
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

int main(void)
{

    printf("%d\n", verificar_data(29, 02, 2013)); // 1

    time_t t;
    time(&t);
    struct tm tm = *localtime(&t);

    // change month to february
    tm.tm_mon = 1;

    // print the date and time (hour, min, sec)
    printf("%d-%d-%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);

        return 0;
}