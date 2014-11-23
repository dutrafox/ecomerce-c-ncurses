#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

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
int carrinhoEletro(/*algum parametro que eu ainda nao pensei*/);
MENU *criarMenu(char *opcoes[]); //Recebe como parametro as opcoes e exibe o menu
void lerUsuariosTexto(char arq[15]);//Le dados do usuario de um arquivo de texto
WINDOW *criarJanela(int linhas, int colunas, int inicioy, int iniciox);
void definirMenu(WINDOW *janela, MENU *menu, int linhas, int colunas);
void destruirJanela(WINDOW *janelaLocal);
void inicializaNCURSES();
void menuCadastrado();

//Variavel globais
USUARIO usuarios[50];

//Funcao main
int main(){
	//lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");

	inicializaNCURSES();
	mvprintw(0, (COLS-8)/2,"ECOMMERCE");
	noecho(); 

	WINDOW *janelaMenuCadastrado;
	attron(COLOR_PAIR(4));
	refresh();
	janelaMenuCadastrado = criarJanela(6, 40, (LINES-6)/2, (COLS-40)/2);

	refresh();

	char *opcoes[] = {"SIM", "NAO"};
	MENU *menuCadastrado;
	menuCadastrado = criarMenu(opcoes);
	definirMenu(janelaMenuCadastrado, menuCadastrado, 1, 2);
	
	post_menu(menuCadastrado);
	wrefresh(janelaMenuCadastrado);
	int c;

	while((c = getch()) != 10){
		switch(c){
			case KEY_LEFT:
				menu_driver(menuCadastrado, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(menuCadastrado, REQ_RIGHT_ITEM);
				break;
		}
		wrefresh(janelaMenuCadastrado);
	}	

	endwin();
}

void menuCadastrado(){
	
}

void inicializaNCURSES(){
	initscr();
	start_color();

	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	bkgd(COLOR_PAIR(1));
	keypad(stdscr, TRUE);	
	cbreak();
}

void definirMenu(WINDOW *janela, MENU *menu, int linhas, int colunas){
	menu_opts_off(menu, O_SHOWDESC);
	set_menu_win(menu, janela);
	set_menu_sub(menu, janela);
	set_menu_format(menu, linhas, colunas);
	set_menu_mark(menu, " * ");
}

MENU *criarMenu(char *opcoes[]){
	ITEM **itens;
	MENU *menu;
	int nOpcoes, i;
	ITEM *item;
	
	nOpcoes = ARRAY_SIZE(*opcoes);
	itens = (ITEM **)calloc(nOpcoes + 1, sizeof(ITEM *));	
		
	for(i = 0; i < nOpcoes; i++){
		itens[i] = new_item(opcoes[i], opcoes[i]);
	}
	itens[nOpcoes] = (ITEM *)NULL;

	menu = new_menu(itens);
	return menu;
}

WINDOW *criarJanela(int linhas, int colunas, int inicioy, int iniciox){
	WINDOW *janelaLocal;

	janelaLocal = newwin(linhas, colunas, inicioy, iniciox);
	box(janelaLocal, 0 , 0);
	wrefresh(janelaLocal);
	return janelaLocal;
}

void destruirJanela(WINDOW *janelaLocal){
	wborder(janelaLocal, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(janelaLocal);
	delwin(janelaLocal);
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
			pch = strtok(NULL, ",");
			j++;
		}
		i++;
	}
}
