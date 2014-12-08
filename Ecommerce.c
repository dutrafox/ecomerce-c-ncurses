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
	float altura;
	float largura;
	float comprimento;
} DIMENSAO;

//Estrutura Eletro-eletronicos
typedef struct Eletro{
	int codigo;
	char *descricao;
	struct Dimensao dimensEletro;
	float preco;
	int estoque;
	char *cor;
} ELETRO;

//Estrutura Vestuario
typedef struct Vestuario{
	int codigo;
	char *descricao;
	char tamanho[3];
	float preco;
	int estoque;
	char *cor;
	char genero[10];
} VEST;

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
	char categoriaUsuario[9];
	struct Endereco endereco;
} USUARIO;

//Estrutura Carrinho
typedef struct Carrinho{
    	int CodigoCliente;
    	int CodigoProduto;
    	int quantidade;
		int aberto;
    	char categoria[10];
} CARRINHO;

//Prototipos das Funcoes
//PRONTAS
MENU *criarMenu(char *opcoes[], int nOpcoes); //Recebe como parametro as opcoes e exibe o menu
void lerUsuariosTexto(char arq[30]);//Le dados do usuario de um arquivo de texto
JANELA *criarJanela(int linhas, int colunas, int inicioy, int iniciox);
void definirMenu(WINDOW *janela, MENU *menu, int linhas, int colunas);
void destruirJanela(JANELA *janelaLocal);
void inicializaNCURSES();
void menuCadastrado();
int MenuSair();
void TelaLogin();
void LerCarrinho(int CodigoCliente);
void TelaCriarCadastro();
int salvarUsuarioTexto(char arq[30]);
void LerEletro();
void LerVestuario();
void MenuTrocaUsuario(JANELA *janela);
void TelaCliente(int codigoUsuario);
void TelaGerente(int codigoUsuario);
int TelaContinuaCompra();
void ApagarCompra(int CodigoCliente);

//EM PRODUCAO
void MenuAbrir(int NumeroCadastro);//abre um log já existente
void MenuPesquisar(int opcao, char /*parametro da pesquisa*/);//pesquisa item por tipo e nome
void MenuRelatorios(/*parametros a definir*/);//emite relátorio de várias coisas
void MenuExcluir(int CodigoCliente, int CodigoProduto); //faltam parametros ainda para excluir produtos do carrinho
void MenuVisualizar(); //listar o carrinho do cliente
void MenuSuspender();//apenas salvar o carrinho com fwrite
void MenuFechar(); //salvar e fechar o carrinho
void MenuCancelar(int CodigoCliente); //cancelar 1 item do carrinho
int MenuInserir(int NumeroCadastro, int quant, char cat);//inserir com o codigo, um item no carrinho


//Variavel globais
USUARIO *usuarios;
int numUsuarios=0;
CARRINHO carrinhoEletronico[20];
CARRINHO carrinhoVestuario[20];
int numItensEletro=0;
int numItensVest=0;
ELETRO *eletronico;
VEST *vestuario;

//Funcao main
int main(){
	lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");
	//LerEletro();
	//LerVestuario();

	inicializaNCURSES();
	mvprintw(0, (COLS-8)/2,"ECOMMERCE");
	mvprintw(LINES-2, COLS-27, "Pressione ESC para sair");
	noecho();

	while(1) menuCadastrado();

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
	janelaCodigoUsuario = criarJanela(5 , 40, (LINES-5)/2, (COLS-40)/2);
	mvwprintw(janelaCodigoUsuario->janela, 2, 5, "Informe seu codigo: ");
	paineis[0] = new_panel(janelaCodigoUsuario->janela);

	update_panels();
	doupdate();

	if(LINES % 2 == 0) move(((LINES-6)/2)+2, ((COLS-40)/2)+25); else move(((LINES-6)/2)+3, ((COLS-40)/2)+25);

	int codigoUsuario;
	scanw("%d", &codigoUsuario);

	if(codigoUsuario <= numUsuarios){
		hide_panel(paineis[0]);
		destruirJanela(janelaCodigoUsuario);
		clear();

		if(usuarios[codigoUsuario-1].categoriaUsuario[0] == 'c'){
			TelaCliente(codigoUsuario); 
		}else{
			TelaGerente(codigoUsuario);
		}
	}else{
		hide_panel(paineis[0]);
		destruirJanela(janelaCodigoUsuario);

		JANELA *janelaErro;
		refresh();
		janelaErro = criarJanela(5, 37, (LINES-5)/2, (COLS-37)/2);
		mvwprintw(janelaErro->janela, 1, (janelaErro->colunas-23)/2, "Usuario nao cadastrado!");
		mvwprintw(janelaErro->janela, 3, (janelaErro->colunas-29)/2, "Pressione ENTER para retornar");
		paineis[0] = new_panel(janelaErro->janela);

		update_panels();
		doupdate();

		int c=0, j;
		while((c = getch()) != 10){
			switch(c){
				case 27: //27 = Tecla ESC
					j = MenuSair();
					if(j == 0){
						update_panels();
						doupdate();
					}
			}
			wrefresh(janelaErro->janela);
		}
		if(c == 10){
			hide_panel(paineis[0]);
			destruirJanela(janelaErro);
		}
	}
}

void TelaCriarCadastro(){
	PANEL *paineis[1];
	int i;

	if((usuarios = (USUARIO *) realloc(usuarios, (numUsuarios+1) * sizeof(USUARIO))) == NULL){
		printw("Erro ao alocar memoria");
		exit(1);
	}

	echo();

	JANELA *janelaCriarCadastro;
	refresh();
	janelaCriarCadastro = criarJanela(16 , 40, (LINES-16)/2, (COLS-40)/2);
	mvwprintw(janelaCriarCadastro->janela, 1, (janelaCriarCadastro->colunas-14)/2, "Criar Cadastro");
	paineis[0] = new_panel(janelaCriarCadastro->janela);

	mvwprintw(janelaCriarCadastro->janela, 3, 3, "Nome:");
	mvwprintw(janelaCriarCadastro->janela, 5, 3, "Sobrenome(Ultimo):");
	mvwprintw(janelaCriarCadastro->janela, 7, 3, "Logradouro:");
	mvwprintw(janelaCriarCadastro->janela, 9, 3, "Numero:");
	mvwprintw(janelaCriarCadastro->janela, 11, 3, "Complemento:");
	mvwprintw(janelaCriarCadastro->janela, 13, 3, "CEP:");

	update_panels();
	doupdate();

	char temp[100];
	int cep;

	move(((LINES-16)/2)+3, ((COLS-40)/2)+9);
	scanw("%[^\n]s", temp);
	if((usuarios[numUsuarios].nomeUsuario = (char *) malloc(strlen(temp) * sizeof(char))) == NULL) exit(0);
	strcpy(usuarios[numUsuarios].nomeUsuario, temp);
	move(((LINES-16)/2)+5, ((COLS-40)/2)+22);
	scanw("%s", &temp);
	if((usuarios[numUsuarios].sobrenomeUsuario = (char *) malloc(strlen(temp) * sizeof(char))) == NULL) exit(0);
	strcpy(usuarios[numUsuarios].sobrenomeUsuario, temp);
	move(((LINES-16)/2)+7, ((COLS-40)/2)+15);
	scanw("%[^\n]s", temp);
	if((usuarios[numUsuarios].endereco.logradouro = (char *) malloc(strlen(temp) * sizeof(char))) == NULL) exit(0);
	strcpy(usuarios[numUsuarios].endereco.logradouro, temp);
	move(((LINES-16)/2)+9, ((COLS-40)/2)+11);
	scanw("%s", &temp);
	if((usuarios[numUsuarios].endereco.numero = (char *) malloc(strlen(temp) * sizeof(char))) == NULL) exit(0);
	strcpy(usuarios[numUsuarios].endereco.numero, temp);
	move(((LINES-16)/2)+11, ((COLS-40)/2)+17);
	scanw("%s", &temp);
	if((usuarios[numUsuarios].endereco.complemento = (char *) malloc(strlen(temp) * sizeof(char))) == NULL) exit(0);
	strcpy(usuarios[numUsuarios].endereco.complemento, temp);
	move(((LINES-16)/2)+13, ((COLS-40)/2)+8);
	scanw("%d", &cep);
	usuarios[numUsuarios].endereco.cep = cep;
	usuarios[numUsuarios].codigoUsuario = numUsuarios+1;
	strcpy(usuarios[numUsuarios].categoriaUsuario, "cliente");
	numUsuarios++;

	noecho();

	int ok = salvarUsuarioTexto("arquivos-de-entrada//usuarios.txt");
	if(ok == 1){
		hide_panel(paineis[0]);
		destruirJanela(janelaCriarCadastro);
		clear();

		JANELA *janelaExibeCodigo;
		refresh();
		janelaExibeCodigo = criarJanela(5 , 36, (LINES-5)/2, (COLS-21)/2);
		mvwprintw(janelaExibeCodigo->janela, 1, (janelaExibeCodigo->colunas-14)/2, "Seu codigo e: %d", numUsuarios);
		mvwprintw(janelaExibeCodigo->janela, 3, (janelaExibeCodigo->colunas-29)/2, "Pressione ENTER para continuar");
		paineis[0] = new_panel(janelaExibeCodigo->janela);
		update_panels();
		doupdate();

		int c=0, j;
		while((c = getch()) != 10){
			switch(c){
				case 27: //27 = Tecla ESC
					j = MenuSair();
					if(j == 0){
						update_panels();
						doupdate();
					}
			}
			wrefresh(janelaCriarCadastro->janela);
		}
		if(c == 10){
			hide_panel(paineis[0]);
			destruirJanela(janelaExibeCodigo);
		}
	}
}

int TelaContinuaCompra(){
	PANEL *paineis[2];

	mvprintw(0, (COLS-8)/2,"ECOMMERCE");
	mvprintw(LINES-2, COLS-27, "Pressione ESC para sair");

	JANELA *janelaContinuaCompra;
	refresh();
	janelaContinuaCompra = criarJanela(5, 50, (LINES-7)/2, (COLS-50)/2);
	mvprintw((LINES-5)/2, (COLS-38)/2, "Deseja continuar sua compra anterior?");
	paineis[0] = new_panel(janelaContinuaCompra->janela);

	JANELA *janelaMenuContinuaCompra;
	refresh();
	janelaMenuContinuaCompra = criarJanela(1,9, janelaContinuaCompra->inicioy+(janelaContinuaCompra->linhas/2), janelaContinuaCompra->iniciox+((janelaContinuaCompra->colunas-9)/2));

	char *opcoesMenuContinuaCompra[] = {"SIM", "NAO"};
	MENU *menuContinuaCompra;
	menuContinuaCompra = criarMenu(opcoesMenuContinuaCompra, 2);
	definirMenu(janelaMenuContinuaCompra->janela, menuContinuaCompra, 1, 2);

	post_menu(menuContinuaCompra);
	paineis[1] = new_panel(janelaMenuContinuaCompra->janela);

	update_panels();
	doupdate();

	noecho();

	int d = 0, j=0;

	while((d = getch()) != 10){
		switch(d){
			case KEY_LEFT:
				menu_driver(menuContinuaCompra, REQ_LEFT_ITEM);
				if(j != 0) j--;
				break;
			case KEY_RIGHT:
				menu_driver(menuContinuaCompra, REQ_RIGHT_ITEM);
				if(j != 1) j++;
				break;
			case 9: // 9 = Tecla TAB
				if(j < 1){
					menu_driver(menuContinuaCompra, REQ_NEXT_MATCH);
					j++;
				}else{
					menu_driver(menuContinuaCompra, REQ_PREV_MATCH);
					j--;
				}
				break;

				//COLOCAR ESC AQUI DEPOIS
		}
		wrefresh(janelaMenuContinuaCompra->janela);
	}
	if(d == 10){
		hide_panel(paineis[0]);
		hide_panel(paineis[1]);
		destruirJanela(janelaContinuaCompra);
		destruirJanela(janelaMenuContinuaCompra);
		update_panels();
		doupdate();
		clear();
		if(j == 0){
			return 1;
		}else{
			return 0;
		}
	}
}

void TelaPesquisar(){
	PANEL *paineis[2];

	JANELA *janelaMenuPesquisar;
	refresh();
	janelaMenuPesquisar = criarJanela(5, COLS, 1, 0);
	paineis[0] = new_panel(janelaMenuPesquisar->janela);

	JANELA *janelaMenuMenuPesquisar;
	refresh();
	janelaMenuMenuPesquisar = criarJanela(3, 47, 2, (COLS-49)/2);
	mvprintw(0, (COLS-21)/2, "Ecommerce - Pesquisar");
	mvprintw(LINES-2, COLS-27, "Pressione ESC para sair");

	char *opcoesMenuPesquisar[] = {"Eletronicos", "Vestuario"};
	MENU *menuPesquisar;
	menuPesquisar = criarMenu(opcoesMenuPesquisar, 2);
	definirMenu(janelaMenuMenuPesquisar->janela, menuPesquisar, 1, 2);

	post_menu(menuPesquisar);
	paineis[1] = new_panel(janelaMenuMenuPesquisar->janela);

	update_panels();
	doupdate();
}

void TelaCliente(int codigoUsuario){
	LerCarrinho(codigoUsuario);

	if(carrinhoEletronico[0].CodigoCliente != NULL && carrinhoVestuario[0].CodigoCliente != NULL){
		int continua = TelaContinuaCompra();
		if(continua == 0){
			ApagarCompra(codigoUsuario);
		}
	}
	
	PANEL *paineis[2];

	JANELA *janelaMenuPrincipal;
	refresh();
	janelaMenuPrincipal = criarJanela(5, COLS, 1, 0);
	paineis[0] = new_panel(janelaMenuPrincipal->janela);

	JANELA *janelaMenuMenuPrincipal;
	refresh();
	janelaMenuMenuPrincipal = criarJanela(3, 47, 2, (COLS-49)/2);
	mvprintw(0, (COLS-24)/2, "Ecommerce - Menu Cliente");
	mvprintw(LINES-2, COLS-27, "Pressione ESC para sair");

	char *opcoesMenuPrincipal[] = {"Trocar Usuario", "Sair", "Pesquisar", "Inserir", "Excluir", "Visualizar", "Suspender", "Fechar", "Cancelar"};
	MENU *menuPrincipal;
	menuPrincipal = criarMenu(opcoesMenuPrincipal, 9);
	definirMenu(janelaMenuMenuPrincipal->janela, menuPrincipal, 3, 3);

	post_menu(menuPrincipal);
	paineis[1] = new_panel(janelaMenuMenuPrincipal->janela);

	update_panels();
	doupdate();

	noecho();

	int c=0, i=0, j, encerra=0;

	while(encerra != 1){
		c = getch();
		switch(c){
			case KEY_LEFT:
				menu_driver(menuPrincipal, REQ_PREV_MATCH);
				if(i != 0 && i != 3 && i != 6) i--;
				break;
			case KEY_RIGHT:
				menu_driver(menuPrincipal, REQ_NEXT_MATCH);
				if(i != 2 && i != 5 && i != 8) i++;
				break;
			case KEY_UP:
				menu_driver(menuPrincipal, REQ_UP_ITEM);
				if(i >= 3){
					i -= 3;
				}
				break;
			case KEY_DOWN:
				menu_driver(menuPrincipal, REQ_DOWN_ITEM);
				if(i <= 5){
					i += 3;
				}
				break;
			case 27: //27 = Tecla ESC
				j = MenuSair();
				if(j == 0){
					update_panels();
					doupdate();
				}
			case 10:
				switch(i){
					case 0:{
						hide_panel(paineis[0]);
						hide_panel(paineis[1]);
						destruirJanela(janelaMenuPrincipal);
						destruirJanela(janelaMenuMenuPrincipal);
						update_panels();
						doupdate();
						encerra = 1;
						break;}
					case 1:{
						j = MenuSair();
						if(j == 0){
							update_panels();
							doupdate();
						}
						break;}
					case 2:
						TelaPesquisar();
						break;
					case 3:
						break;
				}

		}
		wrefresh(janelaMenuMenuPrincipal->janela);
	}
}

void TelaGerente(int codigoUsuario){
	PANEL *paineis[2];

	JANELA *janelaMenuPrincipal;
	refresh();
	janelaMenuPrincipal = criarJanela(3, COLS, 1, 0);
	paineis[0] = new_panel(janelaMenuPrincipal->janela);

	JANELA *janelaMenuMenuPrincipal;
	refresh();
	janelaMenuMenuPrincipal = criarJanela(1, 63, 2, (COLS-63)/2);
	mvprintw(0, (COLS-24)/2, "Ecommerce - Menu Gerente");

	char *opcoesMenuPrincipal[] = {"Trocar Usuario", "Sair", "Pesquisar", "Relatorios"};
	MENU *menuPrincipal;
	menuPrincipal = criarMenu(opcoesMenuPrincipal, 4);
	definirMenu(janelaMenuMenuPrincipal->janela, menuPrincipal, 1, 4);

	post_menu(menuPrincipal);
	paineis[1] = new_panel(janelaMenuMenuPrincipal->janela);

	update_panels();
	doupdate();

	noecho();

	int c=0, i=0, j;

	while((c = getch()) != 10){
		switch(c){
			case KEY_LEFT:
				menu_driver(menuPrincipal, REQ_PREV_MATCH);
				if(i != 0) i--;
				break;
			case KEY_RIGHT:
				menu_driver(menuPrincipal, REQ_NEXT_MATCH);
				if(i != 2) i++;
				break;
			case 27: //27 = Tecla ESC
				j = MenuSair();
				if(j == 0){
					update_panels();
					doupdate();
				}
		}
		wrefresh(janelaMenuMenuPrincipal->janela);
	}
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

	int c=0, i=0, j;

	while((c = getch()) != 10){
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
			case 27: //27 = Tecla ESC
				j = MenuSair();
				if(j == 0){
					update_panels();
					doupdate();
				}
		}
		wrefresh(janelaMenuCadastrado->janela);
	}
	if(c == 10){
		if(i == 0){
			hide_panel(paineis[0]);
			hide_panel(paineis[1]);
			destruirJanela(janelaMenuCadastrado);
			destruirJanela(janelaCadastrado);
			TelaLogin();
		}else{
			hide_panel(paineis[0]);
			hide_panel(paineis[1]);
			destruirJanela(janelaMenuCadastrado);
			destruirJanela(janelaCadastrado);
			TelaCriarCadastro();
		}
	}
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

void MenuAbrir(int NumeroCadastro){ //Não acabado
    int opcao;
    FILE *fp;
    printw("\nDeseja iniciar uma nova compra?\n");

    switch(opcao){
	    case 'S':
	    	break;
	    case 'N':
		break;
    }
}

//Funcao para ler os usuarios do arquivo em modo texto e gravar em um vetor de usuarios
void lerUsuariosTexto(char arq[30]){
	FILE *fp;
	char linha[256];//Valor 140 escolhido por ser a soma do tamanho maximo de todos os dados do usuario
	if((fp = fopen(arq, "r")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	while(fgets(linha,256,fp) != NULL) numUsuarios++;
	if((usuarios = (USUARIO *) malloc(numUsuarios * sizeof(USUARIO))) == NULL){
		printf("Erro ao alocar memoria\n");
		exit(1);
	}
	int i = 0;
	rewind(fp);
	while(fgets(linha,256,fp) != NULL){
		int j = 0;
		char *pch;
		pch = strtok(linha,",\n");
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
				case 7:;
					strcpy(usuarios[i].categoriaUsuario, pch);
					
			}
			pch = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
}

int salvarUsuarioTexto(char arq[30]){ //pronta
	FILE *fp;
	if((fp = fopen(arq, "a+")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	fprintf(fp, "%d,%s,%s,%s,%s,%s,%d,%s\n", usuarios[numUsuarios-1].codigoUsuario, usuarios[numUsuarios-1].nomeUsuario, usuarios[numUsuarios-1].sobrenomeUsuario, usuarios[numUsuarios-1].endereco.logradouro, usuarios[numUsuarios-1].endereco.numero, usuarios[numUsuarios-1].endereco.complemento, usuarios[numUsuarios-1].endereco.cep, usuarios[numUsuarios-1].categoriaUsuario);
	fclose(fp);
	return 1;
}

void LerCarrinho(int CodigoCliente){ //pronta
	int i = 0;
	for(i; i < 20; i++){
		carrinhoVestuario[i].CodigoCliente = NULL;
		carrinhoEletronico[i].CodigoCliente = NULL;
	}

	FILE *fp;
	CARRINHO temp;
	if((fp = fopen("carrinho_usuario.sav", "rb")) == NULL){
		if((fp=fopen("carrinho_usuario.sav", "wb"))==NULL){
			printw("\n O arquivo nao pode ser criado.");
		        exit(1);
		}
	}

	i = 0;
	int j = 0;
	while((fread(&temp, sizeof(CARRINHO), 1, fp))!=0){
		if(temp.aberto == 1){
			if(temp.CodigoCliente == CodigoCliente){
				if(strcmp(temp.categoria, "vestuario")){
					carrinhoEletronico[i] = temp;
					i++;
				}else{
					carrinhoVestuario[j] = temp;
					j++;
				}
			}
		}
	}
	fclose(fp);
}

void ApagarCompra(int CodigoCliente){
	FILE *fp;
	CARRINHO temp;

	if((fp = fopen("carrinho_usuario.sav", "r+b")) == NULL){
			printw("\n O arquivo nao pode ser criado.");
		    exit(1);
	}

	int i = 0, j = 0;
	while((fread(&temp, sizeof(CARRINHO), 1, fp))!=0){
		if(temp.CodigoCliente == CodigoCliente && temp.aberto == 1){
			temp.aberto = 0;
			fwrite(&temp, sizeof(CARRINHO), 1, fp);
		}
	}

	i =0;

	for(i; i < 20; i++){
		carrinhoVestuario[i].CodigoCliente = NULL;
		carrinhoEletronico[i].CodigoCliente = NULL;
	}

	fclose(fp);
}

/*
void LerEletro(){
    FILE *fp;
    char linha[256];
    if((fp=fopen("arquivos-de-entrada//eletro.txt", "r"))==NULL){
        printf("Erro ao abrir o arquivo %s\n", "arquivos-de-entrada//eletro.txt ");
        exit(1);
    }
    while(fgets(linha,256,fp) != NULL) numItensEletro++;
    if((eletronico = (ELETRO *) malloc(numItensEletro * sizeof(ELETRO))) == NULL){
		printf("Erro ao alocar memoria\n");
		exit(1);
    }
    int i=0;
    rewind(fp);
    while(fgets(linha,256,fp) != NULL){
        int j=0;
        char *pch;
        pch = strtok(linha,",");
        while(pch != NULL){
            switch (j){
            case 0:
                eletronico[i].codigo=atoi(pch);
                break;
            case 1:{
                int tamDescricao=strlen(pch);
                //eletronico[i].descricao = (char *) malloc(tamDescricao * sizeof(char));
                //strcpy(eletronico[i].descricao, pch);
                printf("%s\n", pch);
                break;}
            case 2:
                eletronico[i].dimensEletro.altura=atoi(pch);
                break;
            case 3:
                eletronico[i].dimensEletro.largura=atoi(pch);
                break;
            case 4:
                eletronico[i].dimensEletro.comprimento=atoi(pch);
                break;
            case 5:
                eletronico[i].preco=atoi(pch);
                break;
            case 6:
                eletronico[i].estoque=atoi(pch);
                break;
            case 7:{
                int tamCor=strlen(pch);
                eletronico[i].cor = (char*) malloc(tamCor * sizeof(char));
                strcpy(eletronico[i].cor, pch);
                break;}
            }
            pch = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
	exit(0);
}


void LerVestuario(){
    FILE *fp;
    char linha[256];
    if((fp=fopen("arquivos-de-entrada//vestuario.txt", "r"))==NULL){
        printf("Erro ao abrir o arquivo %s\n", "arquivos-de-entrada//vestuario.txt");
        exit(1);
    }
    while((fgets(linha,256,fp))!=NULL){
        numItensVest++;
    }

    if((vestuario=(VEST *)malloc(numItensVest*sizeof(VEST)))==NULL){
       printf("\nErro ao alocar memoria!");
       exit(1);
     }

    int i=0;
    rewind(fp);
    while((fgets(linha, 256, fp))!=NULL){
        char *pch;
        int j=0;
        pch = strtok(linha,",");
        while (pch!=NULL){
            switch(j){
            case 0:
                vestuario[i].codigo=atoi(pch);
                break;
            case 1:{
                int numDescricao=strlen(pch);
                vestuario[i].descricao=(char *)malloc(numDescricao*sizeof(char));
                strcpy(vestuario[i].descricao, pch);
                break;}
            case 2:
                strcpy(vestuario[i].tamanho, pch);
                break;
            case 3:
                vestuario[i].preco=atof(pch);
                break;
            case 4:
                vestuario[i].estoque=atoi(pch);
                break;
            case 5:{
                int numCor=strlen(pch);
                vestuario[i].cor=(char *)malloc(numCor*sizeof(char));
                strcpy(vestuario[i].cor, pch);
                break;}
            case 6:
                strcpy(vestuario[i].genero, pch);
                break;
            }
            pch = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    fclose(fp);
}
*/

/*void MenuVisualizar(){
    CARRINHO *temp;
    temp=LerCarrinho(int CodigoCliente);
        while(temp!=FEOF){
            printw("%d %s %d %d", temp.) /carrinhos bugados for the win, me travaram aqui de novo
        }

}*/

/*
void MenuAbrir(int NumeroCadastro, /*opção da interface, JANELA *janela){
    FILE *fp;
    i=0;
    if((fp=fopen("carrinho_usuario.sav", "r"))==NULL){
        printw("Erro ao abrir carrinho salvo.\n");
        destruirJanela(janela);//não sei se precisa do exit aqui já que eu coloquei destruir janela, dá uma testada nisso
    }
    fseek(fp,NumeroCadastro,SEEK_SET);
    for(i=NumeroCadastro;i<NumeroCadastro+1;i++)
    carrinhos[f]= //empaquei aqui por causa dos nossos carrinhos que estão bugados
*/

/*
void MenuCancelar(int CodigoCliente){
    free(carrinhos[f]); //decidir o que vai ser o índice f, não consegui adequar o código do cliente a ele
}
*/

/*
int MenuInserir(int NumeroCadastro, int quant, char cat){
    FILE *fp;
    if((fp=fopen("carrinho_usuario.sav", "r+b"))==NULL){
        wprintf("Erro ao abrir o arquivo %s\n", "carrinho_usuario.sav");
        exit(1);

    if(cat==Vestuario){
        while(carrinhoVestuario[i]!=NULL && i<20){
        i++;
        }
        if(i==20){return 1;}
            carrinhoVestuario[i].CodigoCliente= NumeroCadastro;
            carrinhoVestuario[i].CodigoProduto= item;
            carrinhoVestuario[i].quantidade=quant;
            carrinhoVestuario[i].aberto=1;
            carrinhoVestuario[i].categoria= cat;
            fseek(fp, sizeof(CARRINHO), SEEK_END);
            fwrite(carrinhoVestuario[i], sizeof(CARRINHO), 1, fp);
        }
        else{
            while(carrinhoEletronico[i]!=NULL && i<20){
            i++;
        }
            if(i==20){return 1;}
            carrinhoEletronico[i].CodigoCliente= NumeroCadastro;
            carrinhoEletronico[i].CodigoProduto= item;
            carrinhoEletronico[i].quantidade=quant;
            carrinhoEletronico[i].aberto=1;
            carrinhoEletronico[i].categoria= cat;
            fseek(fp, sizeof(CARRINHO), SEEK_END);
            fwrite(carrinhoEletronico[i], sizeof(CARRINHO), 1, fp);
        }
    }
    return 0;
}
*/

void MenuCancelar(int CodigoCliente){
    ApagarCompra(CodigoCliente);
    //mesma coisa da troca usuario
}

void MenuExcluir(int CodigoCliente, int CodigoProduto){
    FILE *fp;
	CARRINHO temp;

    if((fp = fopen("carrinho_usuario.sav", "r+b")) == NULL){
			printw("\n O arquivo nao pode ser criado.");
		    exit(1);
	}

	int i = 0, j = 0;
	while((fread(&temp, sizeof(CARRINHO), 1, fp))!=0){
		if(temp.CodigoProduto == CodigoProduto && temp.aberto == 1){
			temp.aberto = 0;
			fwrite(&temp, sizeof(CARRINHO), 1, fp);
		}
	}

	i =0;

	for(i; i < 20; i++){
		if(carrinhoVestuario[i].CodigoProduto==CodigoProduto){
		carrinhoVestuario[i].CodigoCliente = NULL;
		}
		if(carrinhoEletronico[i].CodigoProduto==CodigoProduto){
        carrinhoEletronico[i].CodigoCliente = NULL;
		}
	}

	fclose(fp);

}

/*
void MenuPesquisar(VEST ProdutosVestuario, ELETRO ProdutosEletro){
    char j;
    char e;
    char v;
    int i;
    int pesquisaInt;
    char pesquisaChar;
    float pesquisaFloat;
    printw("Escolha o tipo do produto de sua pesquisa:\n");
        printw("\t Eletro ou Vestuario?\n");
        scanw("%c", &j);
    switch(j){
    case 'E':
        printw("Escolha algum parâmetro para pesquisar seu eletroeletronico:\n");
        printw("\n\t O - cOdigo\n");
        printw("\t D - Descricao\n");
        printw("\t P - Preco\n");
        printw("\t C - Cor\n");
        scanw("%c", &e);
            switch(e){
            case 'O':
                printw("\t Digite o codigo pelo qual deseja procurar:\n");
                scanw("%d", &pesquisaInt);
                for(i=0;i<numItensEletro;i++){
                    if(ProdutosEletro[i].codigo==pesquisaInt){
                        printw("\n\t%d \n\t%s \n\t%d \n\t%d \n\t%d \n\t%f \n\t%d \n\t%s\n", ProdutosEletro[i].codigo, ProdutosEletro[i].descricao, ProdutosEletro[i].dimensEletro.altura, ProdutosEletro[i].dimensEletro.largura, ProdutosEletro[i].dimensEletro.comprimento, ProdutosEletro[i].preco, ProdutosEletro[i].cor);
                    }
                }
                break;
            case 'D':
                printw("\t Digite a descricao que deseja procurar:")
                scanw("%s", &pesquisaChar);
                for(i=0;i<numItensEletro;i++){
                    if(strcmp(pesquisaChar, ProdutosEletro.descricao)==0){
                        printw("\n\t%d \n\t%s \n\t%d \n\t%d \n\t%d \n\t%f \n\t%d \n\t%s\n", ProdutosEletro[i].codigo, ProdutosEletro[i].descricao, ProdutosEletro[i].dimensEletro.altura, ProdutosEletro[i].dimensEletro.largura, ProdutosEletro[i].dimensEletro.comprimento, ProdutosEletro[i].preco, ProdutosEletro[i].cor);
                    }
                break;
            case 'P':
                printw("\t Digite o preco:");
                scanw("%f", &pesquisaFloat);
                for(i=0;i<numItensEletro;i++){
                    if(ProdutosEletro[i].preco==pesquisaFloat){
                        printw("\n\t%d \n\t%s \n\t%d \n\t%d \n\t%d \n\t%f \n\t%d \n\t%s\n", ProdutosEletro[i].codigo, ProdutosEletro[i].descricao, ProdutosEletro[i].dimensEletro.altura, ProdutosEletro[i].dimensEletro.largura, ProdutosEletro[i].dimensEletro.comprimento, ProdutosEletro[i].preco, ProdutosEletro[i].cor);
                    }
                break;
            case 'C':
                printw("\n Informe a cor que deseja procurar:\n");
                scanw("%s", &pesquisaChar);
                for(i=0;i<numItensEletro;i++){
                    if(strcmp(pesquisaChar, ProdutosEletro.cor)==0){
                        printw("\n\t%d \n\t%s \n\t%d \n\t%d \n\t%d \n\t%f \n\t%d \n\t%s\n", ProdutosEletro[i].codigo, ProdutosEletro[i].descricao, ProdutosEletro[i].dimensEletro.altura, ProdutosEletro[i].dimensEletro.largura, ProdutosEletro[i].dimensEletro.comprimento, ProdutosEletro[i].preco, ProdutosEletro[i].cor);
                    }
                break;
            }
        break;
    case 'V':
        printw("Escolha algum parâmetro para pesquisar seu artigo de vestuario:\n");
        printw("\n\t O - cOdigo\n");
        printw("\t D - Descricao\n");
        printw("\t T - Tamanho\n");
        printw("\t P - Preco\n");
        printw("\t C - Cor\n");
        printw("\t G - Genero\n");
        scanw("%c", &v);
            switch(v){
            case 'O':
                printw("\t Digite o codigo pelo qual deseja procurar:\n");
                scanw("%d", &pesquisaInt);
                for(i=0;i<numItensVest;i++){
                    if(ProdutosVestuario[i].codigo==pesquisaInt){
                        printw("\n\t%d \n\t%s \n\t%s \n\t%f \n\t%d \n\t%s \n\t%s \n", ProdutosVestuario[i].codigo, ProdutosVestuario[i].descricao, ProdutosVestuario[i].tamanho, ProdutosVestuario[i].preco, ProdutosVestuario[i].cor, ProdutosVestuario[i].genero);
                    }
                break;
            case 'D':
                printw("\t Informe a descricao do produto:\n");
                scanw("%s", &pesquisaChar);
                for(i=0;i<numItensVest;i++){
                    if((strcmp(ProdutosVestuario[i], pesquisaChar))==0){
                        printw("\n\t%d \n\t%s \n\t%s \n\t%f \n\t%d \n\t%s \n\t%s \n", ProdutosVestuario[i].codigo, ProdutosVestuario[i].descricao, ProdutosVestuario[i].tamanho, ProdutosVestuario[i].preco, ProdutosVestuario[i].cor, ProdutosVestuario[i].genero);
                    }
                break;
            case 'T':
                printw("\t Digite o tamanho pelo qual deseja procurar:\n");
                scanw("%s", &pesquisaChar);
                for(i=0;i<numItensVest;i++){
                    if((strcmp(ProdutosVestuario[i].tamanho==pesquisaChar))==0){
                        printw("\n\t%d \n\t%s \n\t%s \n\t%f \n\t%d \n\t%s \n\t%s \n", ProdutosVestuario[i].codigo, ProdutosVestuario[i].descricao, ProdutosVestuario[i].tamanho, ProdutosVestuario[i].preco, ProdutosVestuario[i].cor, ProdutosVestuario[i].genero);
                    }
                break;
            case 'P':
                printw("\t Digite o preco pelo qual deseja procurar:\n");
                scanw("%f", &pesquisaFloat);
                for(i=0;i<numItensVest;i++){
                    if(ProdutosVestuario[i].preco==pesquisaFloat){
                        printw("\n\t%d \n\t%s \n\t%s \n\t%f \n\t%d \n\t%s \n\t%s \n", ProdutosVestuario[i].codigo, ProdutosVestuario[i].descricao, ProdutosVestuario[i].tamanho, ProdutosVestuario[i].preco, ProdutosVestuario[i].cor, ProdutosVestuario[i].genero);
                    }
                break;
            case 'C':
                printw("\t Digite a cor pela qual deseja procurar:\n");
                scanw("%s", &pesquisaChar);
                for(i=0;i<numItensVest;i++){
                    if((strcmp(ProdutosVestuario[i].cor, pesquisaChar))==0){
                        printw("\n\t%d \n\t%s \n\t%s \n\t%f \n\t%d \n\t%s \n\t%s \n", ProdutosVestuario[i].codigo, ProdutosVestuario[i].descricao, ProdutosVestuario[i].tamanho, ProdutosVestuario[i].preco, ProdutosVestuario[i].cor, ProdutosVestuario[i].genero);
                    }
                break;
            case 'G':
                printw("\t Digite o genero pelo qual deseja procurar (masculino, feminino ou unissex):\n");
                scanw("%s", &pesquisaChar);
                for(i=0;i<numItensVest;i++){
                    if(strcmp(ProdutosVestuario[i].genero, pesquisaChar)){
                        printw("\n\t%d \n\t%s \n\t%s \n\t%f \n\t%d \n\t%s \n\t%s \n", ProdutosVestuario[i].codigo, ProdutosVestuario[i].descricao, ProdutosVestuario[i].tamanho, ProdutosVestuario[i].preco, ProdutosVestuario[i].cor, ProdutosVestuario[i].genero);
                    }
                break;
            default:
                printw("\n\t Essa opçao nao e valida\n");
                break;
            }
        break;
    default:
        printw("Esta nao e uma opçao valida!");
        break;
    }
}
*/
