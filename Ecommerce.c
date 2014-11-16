#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    int altura;
    int largura;
    int profundidade;
} dimensao;

typedef struct{
    int codigo;
    char descricao[31];
    dimensao dimens_eletro;
    float preco;
    int estoque;
    char cor[10];
} eletro;

typedef struct{
    int codigo;
    char descricao[31];
    char tamanho [3];
    float preco;
    int estoque;
    char cor[10];
} vestuario;

int main(){


}

