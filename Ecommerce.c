#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Dimensao{
	int altura;
	int largura;
	int comprimento;
} Dimensao;

typedef struct Eletro{
	int codigo;
	char descricao[31];
	struct Dimensao dimensEletro;
	float preco;
	int estoque;
	char cor[10];
} Eletro;

typedef struct Vestuario{
	int codigo;
	char descricao[31];
	char tamanho[3];
	float preco;
	int estoque;
	char cor[10];
	char genero[10];
} Vestuario;

int main(){
}
