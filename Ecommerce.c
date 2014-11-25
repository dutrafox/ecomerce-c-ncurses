#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>
#include <panel.h>

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
	char *logradouro;
	char *numero; //Usei char para numeros como 105A ou 1005-1
	char *complemento;
	int cep;
} ENDERECO;

//Estrutura Usuarios
typedef struct Usuario{
	int codigoUsuario;
	char *nomeUsuario;
	char *sobrenomeUsuario;
	char categoria[8];
	struct Endereco endereco;
} USUARIO;	

//Estrutura Carrinho
typedef struct Carrinho{
    int CodigoCliente;
    int CodigoProduto;
    int Aberto;
    char CategoriaProduto[10];
} CARRINHO;


//Prototipos das Funcoes
int carrinhoVest(int /*numero do cadastro*/, int /*outro parametro*/ );
int carrinhoEletro(/*algum parametro que eu ainda nao pensei*/);
MENU *criarMenu(char *opcoes[], int nOpcoes); //Recebe como parametro as opcoes e exibe o menu
void lerUsuariosTexto(char arq[15]);//Le dados do usuario de um arquivo de texto
JANELA *criarJanela(int linhas, int colunas, int inicioy, int iniciox);
void definirMenu(WINDOW *janela, MENU *menu, int linhas, int colunas);
void destruirJanela(JANELA *janelaLocal);
void inicializaNCURSES();
void menuCadastrado();
void MenuAbrir(int *NumeroCadastro);//abre um log já existente
void MenuGerente(int opcao);
void MenuCliente(int opcao);
void MenuPesquisar(int opcao, char /*parametro da pesquisa*/);//pesquisa item por tipo e nome
void MenuRelatorios(/*parametros a definir*/);//emite relátorio de várias coisas
void MenuTrocaUsuario(JANELA *janela);//é aquela funçao besta, tá pronta já
int MenuSair(); //Parametro 
void MenuExcluir(); //faltam parametros ainda para excluir produtos do carrinho
void MenuVisualizar(); //listar o carrinho do cliente
void MenuSuspender();//apenas salvar o carrinho com fwrite
void MenuFechar(); //salvar e fechar o carrinho
void MenuCancelar(int CodigoCliente); //cancelar 1 item do carrinho
void MenuInserir();//inserir com o codigo, um item no carrinho
void TelaLogin();


//Variavel globais
USUARIO *usuarios;
CARRINHO carrinhos[20];

//Funcao main
int main(){
	lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");

	inicializaNCURSES();
	mvprintw(0, (COLS-8)/2,"ECOMMERCE");
	mvprintw(LINES-2, COLS-27, "Pressione ESC para sair");
	noecho();
	
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

MENU *criarMenu(char *opcoes[], int nOpcoes){
	ITEM **itens;
	MENU *menu;
	int i;
	ITEM *item;

	itens = (ITEM **) calloc(nOpcoes + 1, sizeof(ITEM *));

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
	free(janelaLocal);
}

void TelaLogin(){
	PANEL *paineis[1];
	
	echo();	

	JANELA *janelaCodigoUsuario;
	refresh();
	janelaCodigoUsuario = criarJanela(5 , 40, (LINES-6)/2, (COLS-40)/2);
	mvwprintw(janelaCodigoUsuario->janela, 2, 5, "Informe seu codigo: ");
	paineis[0] = new_panel(janelaCodigoUsuario->janela);

	update_panels();
	doupdate();
}

void menuCadastrado(){
	attron(COLOR_PAIR(4));

	PANEL *paineis[2];

	JANELA *janelaCadastrado;
	refresh();
	janelaCadastrado = criarJanela(6, 40, (LINES-6)/2, (COLS-40)/2);
	mvwprintw(janelaCadastrado->janela, 1, (janelaCadastrado->colunas-19)/2, "Ja possui cadastro?");
	paineis[0] = new_panel(janelaCadastrado->janela);

	JANELA *janelaMenuCadastrado;
	refresh();
	janelaMenuCadastrado = criarJanela(1, 9, janelaCadastrado->inicioy+(janelaCadastrado->linhas/2), janelaCadastrado->iniciox+((janelaCadastrado->colunas-11)/2));
	
	char *opcoesMenuCadastrado[] = {"SIM", "NAO"};
	MENU *menuCadastrado;
	menuCadastrado = criarMenu(opcoesMenuCadastrado, 2);
	definirMenu(janelaMenuCadastrado->janela, menuCadastrado, 1, 2);
	
	post_menu(menuCadastrado);
	paineis[1] = new_panel(janelaMenuCadastrado->janela);
	update_panels();
	doupdate();

	int c, i=0, j;

	while(c = getch()){ //27 = Tecla ESC
		switch(c){
			case KEY_LEFT:
				menu_driver(menuCadastrado, REQ_LEFT_ITEM);
				i--;
				break;
			case KEY_RIGHT:
				menu_driver(menuCadastrado, REQ_RIGHT_ITEM);
				i++;
				break;
			case 9: // 9 = Tecla TAB
				if(i < 1){
					menu_driver(menuCadastrado, REQ_NEXT_MATCH);
					i++;
				}else{
					menu_driver(menuCadastrado, REQ_PREV_MATCH);
					i--;
				}
				break;
			case 10: // 10 = Tecla ENTER
				if(i == 0){
					hide_panel(paineis[0]);
					hide_panel(paineis[1]);
					destruirJanela(janelaMenuCadastrado);
					destruirJanela(janelaCadastrado);
					TelaLogin();
				}else{
					
				}
				break;
			case 27: //27 = Tecla ESC
				j = MenuSair();
				if(j == 0){
					update_panels();
					doupdate();
				}
		}
		wrefresh(janelaMenuCadastrado->janela);
	}
}

//Funçao para ler os usuarios do arquivo em modo texto e gravar em um vetor de usuarios
void lerUsuariosTexto(char arq[15]){
	FILE *fp;
	int numUsuarios=0;
	char linha[140];//Valor 140 escolhido por ser a soma do tamanho maximo de todos os dados do usuario
	if((fp = fopen(arq, "r")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	while(fgets(linha,140,fp) != NULL) numUsuarios++;
	if((usuarios = (USUARIO *) malloc(numUsuarios * sizeof(USUARIO))) == NULL){
		printf("Erro ao alocar memoria\n");
		exit(1);
	}
	int i = 0;
	rewind(fp);
	while(fgets(linha,140,fp) != NULL){
		int j = 0;
		char *pch;
		pch = strtok(linha,",");
		while(pch != NULL){
			switch(j){
				case 0:
					usuarios[i].codigoUsuario = atoi(pch);
					break;
				case 1:{
					int tamNome = strlen(pch);
					usuarios[i].nomeUsuario = (char *) malloc(tamNome * sizeof(char));
					strcpy(usuarios[i].nomeUsuario, pch);
					break;}
				case 2:{
					int tamSobrenome = strlen(pch);
					usuarios[i].sobrenomeUsuario = (char *) malloc(tamSobrenome * sizeof(char));
					strcpy(usuarios[i].sobrenomeUsuario, pch);
					break;}
				case 3:{
					int tamLogradouro = strlen(pch);
					usuarios[i].endereco.logradouro = (char *) malloc(tamLogradouro * sizeof(char));
					strcpy(usuarios[i].endereco.logradouro, pch);
					break;}
				case 4:{
					int tamNumero = strlen(pch);
					usuarios[i].endereco.numero = (char *) malloc(tamNumero * sizeof(char));
					strcpy(usuarios[i].endereco.numero, pch);
					break;}
				case 5:{
					int tamComplemento = strlen(pch);
					usuarios[i].endereco.complemento = (char *) malloc(tamComplemento * sizeof(char));
					strcpy(usuarios[i].endereco.complemento, pch);
					break;}
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
	fclose(fp);
}


void MenuAbrir(int *NumeroCadastro){
	/*
        FILE *fp;
	if((fp = fopen("carrinho_usuario.sav", "rb")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	//deixei o resto por fazer porque precisa da estrutura do carrinho de compras
	*/
}


void MenuGerente(int opcao) { //menu de acesso exclusivo do gerente
	/*
    	switch(opcao){
		case '1':
		    MenuPesquisar(opcao, "busca"//parametro da pesquisa);
		    break;
		case '2':
		    MenuRelatorios();
		    break;
		case '3':
		    MenuTrocaUsuario(janela);
		    break;
		case '4':
		   MenuSair();
		   break;
        }
	*/
}


void MenuCliente(int opcao){ //menu de acesso do cliente
	/*
    	switch(opcao){
		case '1':
		    MenuAbrir(*NumeroCadastro);
		    break;
		case '2':
		    MenuPesquisar(opcao, "busca"//parametro da pesquisa);
		    break;
		case '3':
		    MenuInserir();
		    break;
		case '4':
		    MenuExcluir();
		    break;
		case '5':
		    MenuVisualizar();
		    break;
		case '6':
		    MenuSuspender();
		    break;
		case '7':
		    MenuFechar();
		    break;
		case '8':
		    MenuCancelar();
		case '9':
		    MenuTrocaUsuario(janela);
		    break;
		case '10':
		    MenuSair();
		    break;
    	}
	*/
}

void MenuTrocaUsuario(JANELA *janela){
    destruirJanela(janela);
}

int MenuSair(){
	PANEL *paineis[3];

	JANELA *janelaFundo;
	refresh();
	janelaFundo = criarJanela(LINES-1, COLS, 1, 0);
	paineis[0] = new_panel(janelaFundo->janela);

	JANELA *janelaSair;
	refresh();
	janelaSair = criarJanela(6, 40, (LINES-6)/2, (COLS-40)/2);
	mvwprintw(janelaSair->janela, 1, (janelaSair->colunas-22)/2, "Deseja realmente sair?");
	paineis[1] = new_panel(janelaSair->janela);

	JANELA *janelaMenuSair;
	refresh();
	janelaMenuSair = criarJanela(1, 9, janelaSair->inicioy+(janelaSair->linhas/2), janelaSair->iniciox+((janelaSair->colunas-11)/2));
	
	char *opcoesMenuSair[] = {"SIM", "NAO"};
	MENU *menuSair;
	menuSair = criarMenu(opcoesMenuSair, 2);
	definirMenu(janelaMenuSair->janela, menuSair, 1, 2);
	
	post_menu(menuSair);
	paineis[2] = new_panel(janelaMenuSair->janela);
	update_panels();
	doupdate();

	int d, j=0, i=0;

	while(d = getch()){
		switch(d){
			case KEY_LEFT:
				menu_driver(menuSair, REQ_LEFT_ITEM);
				j--;
				break;
			case KEY_RIGHT:
				menu_driver(menuSair, REQ_RIGHT_ITEM);
				j++;
				break;
			case 9: // 9 = Tecla TAB
				if(j < 1){
					menu_driver(menuSair, REQ_NEXT_MATCH);
					j++;
				}else{
					menu_driver(menuSair, REQ_PREV_MATCH);
					j--;
				}
				break;
			case 10: // 10 = Tecla ENTER	
				if(j == 0){
					endwin();
					exit(0);
				}else{
					hide_panel(paineis[0]);
					hide_panel(paineis[1]);
					hide_panel(paineis[2]);
					destruirJanela(janelaFundo);
					destruirJanela(janelaSair);
					destruirJanela(janelaMenuSair);
					return 0;
				}
		}
		wrefresh(janelaMenuSair->janela);
	}
}
