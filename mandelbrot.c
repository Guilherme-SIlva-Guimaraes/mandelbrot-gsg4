#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "mandelbrot.h"

void pixel_para_complexo(int px, int py, const Config *cfg,
                          double *cr, double *ci) {
    *cr = RE_MIN + (px / (double)(cfg->largura - 1)) * (RE_MAX - RE_MIN);
    *ci = IM_MIN + (py / (double)(cfg->altura - 1)) * (IM_MAX - IM_MIN);
}

int mandelbrot_iteracoes(double cr, double ci, int max_iteracoes) {
    double zr = 0.0, zi = 0.0;
    int it = 0;
    while (it < max_iteracoes) {
        double zr2 = zr * zr;
        double zi2 = zi * zi;
        if (zr2 + zi2 > 4.0) break;
        double novo_zi = 2.0 * zr * zi + ci;
        double novo_zr = zr2 - zi2 + cr;
        zr = novo_zr;
        zi = novo_zi;
        it++;
    }
    return it;
}

unsigned char normaliza_intensidade(int iteracoes, int max_iteracoes) {
    if (iteracoes >= max_iteracoes) return 255;
    double frac = iteracoes / (double)max_iteracoes;
    int val = (int)(frac * 255.0);
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    return (unsigned char)val;
}

Imagem *imagem_criar(int largura, int altura) {
    Imagem *img = malloc(sizeof(Imagem));
    if (!img) return NULL;
    img->pixels = malloc(largura * altura * sizeof(unsigned char));
    if (!img->pixels) {
        free(img);
        return NULL;
    }
    img->largura = largura;
    img->altura = altura;
    return img;
}

void imagem_destruir(Imagem *img) {
    if (!img) return;
    free(img->pixels);
    free(img);
}

void calcula_serial(Imagem *img, const Config *cfg) {
    for (int py = 0; py < img->altura; py++) {
        for (int px = 0; px < img->largura; px++) {
            double cr, ci;
            pixel_para_complexo(px, py, cfg, &cr, &ci);
            int it = mandelbrot_iteracoes(cr, ci, cfg->max_iteracoes);
            img->pixels[py * img->largura + px] =
                normaliza_intensidade(it, cfg->max_iteracoes);
        }
    }
}

int escreve_pgm(const char *caminho, const Imagem *img) {
    FILE *f = fopen(caminho, "w");
    if (!f) {
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo '%s'\n", caminho);
        return -1;
    }
    for (int py = 0; py < img->altura; py++) {
        for (int px = 0; px < img->largura; px++) {
            if (px > 0) fputc(' ', f);
            fprintf(f, "%d", img->pixels[py * img->largura + px]);
        }
        fputc('\n', f);
    }
    fclose(f);
    return 0;
}

static int parse_int_positivo(const char *s, int *out) {
    if (s == NULL || s[0] == '\0') return -1;
    char *endptr;
    long val = strtol(s, &endptr, 10);
    if (*endptr != '\0') return -1;
    if (val <= 0) return -1;
    if (val > 1000000) return -1;
    *out = (int)val;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s [largura] [altura] [max_iteracoes] [num_threads]\n", argv[0]);
        return 1;
    }

    Config cfg;
    strncpy(cfg.login, "gsg4", sizeof(cfg.login) - 1);
    cfg.login[sizeof(cfg.login) - 1] = '\0';

    if (parse_int_positivo(argv[1], &cfg.largura) != 0) {
        fprintf(stderr, "Erro: largura invalida '%s'\n", argv[1]);
        return 1;
    }
    if (parse_int_positivo(argv[2], &cfg.altura) != 0) {
        fprintf(stderr, "Erro: altura invalida '%s'\n", argv[2]);
        return 1;
    }
    if (parse_int_positivo(argv[3], &cfg.max_iteracoes) != 0) {
        fprintf(stderr, "Erro: max_iteracoes invalido '%s'\n", argv[3]);
        return 1;
    }
    if (parse_int_positivo(argv[4], &cfg.num_threads) != 0) {
        fprintf(stderr, "Erro: num_threads invalido '%s'\n", argv[4]);
        return 1;
    }

    Imagem *img = imagem_criar(cfg.largura, cfg.altura);
    if (!img) {
        fprintf(stderr, "Erro: falha na alocacao de memoria para a imagem\n");
        return 1;
    }

    FILE *ftimes = fopen("times.txt", "w");
    if (!ftimes) {
        fprintf(stderr, "Erro: nao foi possivel criar times.txt\n");
        imagem_destruir(img);
        return 1;
    }

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    calcula_serial(img, &cfg);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double tempo_serial = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    char nome_serial[128];
    snprintf(nome_serial, sizeof(nome_serial), "mandelbrot_%s_serial.pgm", cfg.login);
    if (escreve_pgm(nome_serial, img) != 0) {
        imagem_destruir(img);
        fclose(ftimes);
        return 1;
    }

    fprintf(ftimes, "Serial: %.6f s\n", tempo_serial);

    fclose(ftimes);
    imagem_destruir(img);

    return 0;
}