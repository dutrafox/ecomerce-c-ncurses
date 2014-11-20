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

//Prototipos das Fun��es
int CarrinhoVest(int /*n�mero do cadastro*/, int /*outro parametro*/ );
int CarrinhoEletro(/*algum par�metro que eu ainda n�o pensei*/);
void MenuCliente(int opcao); //Menu do cliente
void MenuGerente(int op�ao);//Menu do gerente(assumindo que a gente vai por o main todo em fun��es)


//Variavel globais
USUARIO usuarios[50]; //mudei para 50 pois nas especifica��es ele diz que poder�o se conectar 50 usu�rios simult�neos(que ainda n�o fizeram check out)

//Funcao main
int main(){
}

//Funcao lerUsuariosTexto
int lerUsuariosTexto(char arq[15]){
	FILE *fp;
	char linha[140];//Valor 139 escolhido por ser a soma do tamanho maximo de todos os dados do usuario
	if((fp = fopen(arq, "r")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	while(fgets(linha,MAX,fp) != NULL){

	}
}

//n�o sei se tu percebeu, mas ele pede pra salvar o log de compras com data e hora, ent�o a gente vai ter que usar clock




