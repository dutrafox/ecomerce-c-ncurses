#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <curses.h>

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

//Estrutura Endereco
typedef struct Endereco{
	char logradouro[51];
	char numero[8]; //Usei char para numeros como 105A ou 1005-1
	char complemento[21];
	int cep;
} ENDERECO;

//Estrutura Usuarios
typedef struct Usuario{
	int codigo;
	char nome[16];
	char ultimoSobrenome[16];
	char categoria[8];
	struct Endereco endereco;
} USUARIO;

//Prototipos das Funcoes
int carrinhoVest(int /*numero do cadastro*/, int /*outro parametro*/ );
int carrinhoEletro(/*algum parametro que eu ainda não pensei*/);
void menuCliente(int opcao); //Menu do cliente
void menuGerente(int opcao);//Menu do gerente(assumindo que a gente vai por o main todo em funcoes)
void lerUsuariosTexto(char arq[15]);

//Variavel globais
USUARIO usuarios[50]; //Sim 50 e o maximo de logados, mas aqui vamos armazenar todos os usuarios, independente de estar logado ou nao, por isto acho que nao podemos assumir que teremos apenas 50 usuurios, bom acho entao que podemos usar alocacao dinamica, pois o vetor nao precisa ter tamanho com alocacao dinamica

//Funcao main
int main(){
	lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");
	initscr();

	start_color();

	    refresh();    //Atualiza a tela
	    getch();      //Fica esperando que o usuário aperte alguma tecla
}

//Funçao para ler os usuarios do arquivo em modo texto e gravar em um vetor de usuarios
void lerUsuariosTexto(char arq[15]){

	FILE *fp;
	char linha[140];//Valor 140 escolhido por ser a soma do tamanho maximo de todos os dados do usuario
	if((fp = fopen(arq, "r")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	int i = 0;
	while(fgets(linha,140,fp) != NULL){
		int j = 0;
		char *pch;
		pch = strtok(linha,",");
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
					strcpy(usuarios[i].endereco.logradouro, pch);
					break;
				case 4:
					strcpy(usuarios[i].endereco.numero, pch);
					break;
				case 5:
					strcpy(usuarios[i].endereco.complemento, pch);
					break;
				case 6:
					usuarios[i].endereco.cep = atoi(pch);
					break;
				case 7:
					strcpy(usuarios[i].categoria, pch);
			}
			printf("%s\n", pch);
			pch = strtok(NULL, ",");
			j++;
		}
		i++;
	}
}

//Nao tinha percebido mas tranquilo
