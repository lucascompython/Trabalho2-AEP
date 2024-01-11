# Intruções para compilar e executar o programa


## Dependencias
Neste projeto usamos [yyjson](https://github.com/ibireme/yyjson). Para instalar execute:
```bash
git submodule update --init --recursive
```

## Linux

Instalar `clang` e `make` e depois:

```bash
make run # compilar e executar
```

## Windows

Neste projeto estamos a utilizar o `MSVC`, por isso é necessário ter o Visual Studio instalado e executar os comandos a partir do "Develop Powershell/Command Prompt for VS".  
Não funciona em terminais que não suportam códigos de escape ANSI. Recomendamos o [Windows Terminal](https://apps.microsoft.com/detail/9N0DX20HK701).

Se quiser utilizar o `make`, é preciso instala-lo com:

```ps1
choco install make
```

E depois executar:

```ps1
make run # compilar e executar
```

OU  
Sem o `make`, executar:

```ps1
./make.ps1 -run
# Caso deia erro:
powershell.exe -NoProfile -ExecutionPolicy ByPass -File make.ps1 -run
```
