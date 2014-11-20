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

//Prototipos das Funções
int CarrinhoVest(int /*número do cadastro*/, int /*outro parametro*/ );
int CarrinhoEletro(/*algum parâmetro que eu ainda não pensei*/);
void MenuCliente(int opcao); //Menu do cliente
void MenuGerente(int opçao);//Menu do gerente(assumindo que a gente vai por o main todo em funções)


//Variavel globais
USUARIO usuarios[50]; //Sim 50 é maximo de logados, mas aqui vamos armazenar todos os usuarios, independente de estar logado ou não, por isto acho que não podemos assumir que teremos apenas 50 usuários, bom acho então que podemos usar alocação dinamica, pois o vetor não precisa ter tamanho com alocação dinâmica

//Funcao main
int main(){
}

//Funcao lerUsuariosTexto
int lerUsuariosTexto(char arq[15]){
	FILE *fp;
	char linha[140];//Valor 140 escolhido por ser a soma do tamanho maximo de todos os dados do usuario
	if((fp = fopen(arq, "r")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	while(fgets(linha,MAX,fp) != NULL){

	}
}

//Não tinha percebido mas tranquilo
