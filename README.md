# Mandelbrot

Mandelbrot é um gerador de imagens do conjunto de Mandelbrot feito em C. Ele calcula, para cada ponto do plano complexo, se a órbita da iteração z = z² + c diverge ou permanece limitada, usando quatro estratégias diferentes de execução: sequencial, OpenMP e duas variações com Pthreads.

## Funcionalidades

* Geração da imagem do conjunto de Mandelbrot na região real [-2,0; 1,0] e imaginária [-1,5; 1,5].
* Implementação Serial (sequencial, sem paralelismo).
* Implementação paralela com OpenMP.
* Implementação paralela com Pthreads, dividindo as linhas da imagem estaticamente entre as threads.
* Implementação paralela com Pthreads, usando fila dinâmica de trabalho com mutex.
* Geração de times.txt com o tempo de execução de cada implementação.
* Validação de todos os argumentos de entrada, com mensagens de erro apropriadas.

## Pré-requisitos

* Linux ou WSL no Windows (o projeto foi desenvolvido e testado no Windows com WSL2, distribuição Ubuntu).
* gcc.
* make.

O programa usa Pthreads e OpenMP, então precisa de um ambiente Linux/Unix (ou WSL equivalente) com suporte a essas bibliotecas.

## Como compilar

Use o make:

    make

Isso gera o executável mandelbrot.

Também é possível compilar diretamente com:

    gcc -Wall -Wextra -O2 -o mandelbrot mandelbrot.c -fopenmp -pthread -lm

Para limpar o executável e os arquivos gerados:

    make clean

## Como executar

    ./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]

Exemplo:

    ./mandelbrot 1000 800 1000 8

Isso gera quatro arquivos de imagem (mandelbrot_gsg4_serial.pgm, mandelbrot_gsg4_openmp.pgm, mandelbrot_gsg4_pthreads1.pgm e mandelbrot_gsg4_pthreads2.pgm) e o arquivo times.txt com o tempo de cada implementação.

## Parâmetros

* largura: largura da imagem em pixels (inteiro positivo).
* altura: altura da imagem em pixels (inteiro positivo).
* max_iteracoes: número máximo de iterações por ponto (inteiro positivo).
* num_threads: número de threads usadas nas implementações paralelas (inteiro positivo).

## Como testar

Para validar rapidamente as quatro implementações de uma vez, incluindo a comparação de que todos os arquivos de saída são idênticos:

    make clean
    make
    ./mandelbrot 1000 800 1000 8
    diff mandelbrot_gsg4_serial.pgm mandelbrot_gsg4_openmp.pgm
    diff mandelbrot_gsg4_serial.pgm mandelbrot_gsg4_pthreads1.pgm
    diff mandelbrot_gsg4_serial.pgm mandelbrot_gsg4_pthreads2.pgm

Nenhuma saída do diff significa que os quatro arquivos são idênticos.

Para testar casos de erro (número incorreto de argumentos, valores inválidos ou negativos, etc.), rode o programa sem argumentos ou com valores inválidos:

    ./mandelbrot
    ./mandelbrot -10 100 100 4
    ./mandelbrot abc 100 100 4

As sessões de teste reais, incluindo os casos de sucesso e de erro, ficam registradas em evidencias.log, gravado com script -a evidencias.log.

## Estrutura do projeto

    mandelbrot.c
    mandelbrot.h
    Makefile
    evidencias.log
    README.md

* mandelbrot.c: código-fonte principal, com o núcleo matemático e as quatro implementações (Serial, OpenMP, Pthreads1, Pthreads2).
* mandelbrot.h: declarações de structs e protótipos das funções.
* Makefile: automatiza a compilação e a limpeza.
* evidencias.log: registro de testes feitos no terminal.

