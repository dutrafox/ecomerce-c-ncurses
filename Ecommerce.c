#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Estrutura Dimensao
typedef struct Dimensao{
	int altura;
	int largura;
	int comprimento;
} DIMENSAO;

//Estrutura Eletro-eletronicos
typedef struct Eletro{
	int codigo;
	char descricao[31];
	struct Dimensao dimensEletro;
	float preco;
	int estoque;
	char cor[10];
} ELETRO;

//Estrutura Vestuario
typedef struct Vestuario{
	int codigo;
	char descricao[31];
	char tamanho[3];
	float preco;
	int estoque;
	char cor[10];
	char genero[10];
} VESTUARIO;

//Estrutura Usuarios
typedef struct Usuario{
	int codigo;
	char nome[16];
	char ultimoSobrenome[16];
	char logradouro[51];
	char numero[8]; //Usei char para numeros como 105A ou 1005-1
	char complemento[21];
	int cep;
	char categoria[8];
} USUARIO;

//Prototipos das Funções
void lerUsuariosTexto(char arq[15]);

//Variavel globais
USUARIO usuarios[200]; //O q vc acha? Quantos usuarios o maximo?

//Funcao main
int main(){
	lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");
}

//Funcao lerUsuariosTexto
//RODRIGO COMENTEI ESTA FUNÇÃO PQ ELA DEVERIA FUNCIONAR SE O ARQUIVO USUARIOS.TXT ESTIVESSE COMO NA ESPECIFICAÇAO, MAS ELE NÃO ESTA ENTÃO POSTEI UMA MENSAGEM NO FORUM
/*
void lerUsuariosTexto(char arq[15]){

	FILE *fp;
	char linha[140];//Valor 139 escolhido por ser a soma do tamanho maximo de todos os dados do usuario
	if((fp = fopen(arq, "r")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	int i = 0;
	while(fgets(linha,140,fp) != NULL){
		int j = 0;
		char *pch;
		pch = strtok(linha," ,");
		while(pch != NULL){
			switch(j){
				case 0:
					usuarios[i].codigo = atoi(pch);
					break;
				case 1:
					strcpy(usuarios[i].nome, pch);
					break;
				case 2:
					strcpy(usuarios[i].ultimoSobrenome, pch);
					break;
				case 3:
					strcpy(usuarios[i].logradouro, pch);
					break;
				case 4:
					strcpy(usuarios[i].numero, pch);
					break;
				case 5:
					strcpy(usuarios[i].complemento, pch);
					break;
				case 6:
					usuarios[i].cep = atoi(pch);
					break;
				case 7:
					strcpy(usuarios[i].categoria, pch);
			}
			printf("%s\n", pch);
			pch = strtok(NULL, " ,");
			j++;
		}
		i++;
	}
}
*/
