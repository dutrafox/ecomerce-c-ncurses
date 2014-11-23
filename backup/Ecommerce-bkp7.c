#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//Estrutura Janela
typedef struct Janela{
	int linhas;
	int colunas;
	int inicioy;
	int iniciox;
	WINDOW *janela;
} JANELA;

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
JANELA *criarJanela(int linhas, int colunas, int inicioy, int iniciox);
void definirMenu(WINDOW *janela, MENU *menu, int linhas, int colunas);
void destruirJanela(JANELA *janelaLocal);
void inicializaNCURSES();
void menuCadastrado();

//Variavel globais
USUARIO usuarios[50];

//Funcao main
int main(){
	//lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");
	inicializaNCURSES();
	mvprintw(0, (COLS-8)/2,"ECOMMERCE");
	mvprintw(LINES-2, COLS-27, "Pressione ESC para sair");
	noecho();

	attron(COLOR_PAIR(4));	
	
	menuCadastrado();
	
	endwin();
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
	set_menu_mark(menu, "*");
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

// NEM PENSA EM MEXER AQUI QUE TA FUNCIONANDO!!!!
JANELA *criarJanela(int linhas, int colunas, int inicioy, int iniciox){
	JANELA *janela;
	if ((janela = (JANELA *) malloc (sizeof (JANELA))) == NULL) return NULL;
	if ((janela->linhas = (int *) malloc ( sizeof (int))) == NULL) return NULL;
	if ((janela->colunas = (int *) malloc ( sizeof (int))) == NULL) return NULL;
	if ((janela->inicioy = (int *) malloc ( sizeof (int))) == NULL) return NULL;
	if ((janela->iniciox = (int *) malloc ( sizeof (int))) == NULL) return NULL;
	if ((janela->janela = (WINDOW *) malloc ( sizeof (WINDOW))) == NULL) return NULL; 

	janela->linhas = linhas;
	janela->colunas = colunas;
	janela->inicioy = inicioy;
	janela->iniciox = iniciox;

	WINDOW *janelaLocal;

	janelaLocal = newwin(linhas, colunas, inicioy, iniciox);
	box(janelaLocal, 0 , 0);
	wrefresh(janelaLocal);
	janela->janela = janelaLocal;
	return janela;
}

void destruirJanela(JANELA *janelaLocal){
	wborder(janelaLocal->janela, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(janelaLocal->janela);
	delwin(janelaLocal->janela);
}

void menuCadastrado(){
	JANELA *janelaCadastrado;
	refresh();
	janelaCadastrado = criarJanela(6, 40, (LINES-6)/2, (COLS-40)/2);
	mvwprintw(janelaCadastrado->janela, 1, (janelaCadastrado->colunas-19)/2, "Ja possui cadastro?");
	wrefresh(janelaCadastrado->janela);

	JANELA *janelaMenuCadastrado;
	refresh();
	janelaMenuCadastrado = criarJanela(1, 9, janelaCadastrado->inicioy+(janelaCadastrado->linhas/2), janelaCadastrado->iniciox+((janelaCadastrado->colunas-11)/2));
	
	char *opcoes[] = {"SIM", "NAO"};
	MENU *menuCadastrado;
	menuCadastrado = criarMenu(opcoes);
	definirMenu(janelaMenuCadastrado->janela, menuCadastrado, 1, 2);
	
	post_menu(menuCadastrado);
	wrefresh(janelaMenuCadastrado->janela);
	int c, i=0;

	while((c = getch()) != 27){
		switch(c){
			case KEY_LEFT:
				menu_driver(menuCadastrado, REQ_LEFT_ITEM);
				i--;
				break;
			case KEY_RIGHT:
				menu_driver(menuCadastrado, REQ_RIGHT_ITEM);
				i++;
				break;
			case 10:
				if(i == 0){
				}else{
				}
		}
		wrefresh(janelaMenuCadastrado->janela);
	}
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
