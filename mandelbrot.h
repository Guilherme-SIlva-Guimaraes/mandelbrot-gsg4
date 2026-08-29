#ifndef MANDELBROT_H
#define MANDELBROT_H

#define RE_MIN -2.0
#define RE_MAX  1.0
#define IM_MIN -1.5
#define IM_MAX  1.5

typedef struct {
    int largura;
    int altura;
    int max_iteracoes;
    int num_threads;
    char login[64];
} Config;

typedef struct {
    unsigned char *pixels;
    int largura;
    int altura;
} Imagem;

void pixel_para_complexo(int px, int py, const Config *cfg,
                          double *cr, double *ci);
int mandelbrot_iteracoes(double cr, double ci, int max_iteracoes);
unsigned char normaliza_intensidade(int iteracoes, int max_iteracoes);

Imagem *imagem_criar(int largura, int altura);
void imagem_destruir(Imagem *img);

void calcula_serial(Imagem *img, const Config *cfg);

int escreve_pgm(const char *caminho, const Imagem *img);

#endif