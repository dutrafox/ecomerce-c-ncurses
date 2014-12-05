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
	char categoria[8];
	struct Endereco endereco;
} USUARIO;

//Estrutura Carrinho Eletroeletronicos
typedef struct Carrinho{
    	int CodigoCliente;
    	int CodigoProduto;
    	int quantidade;
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

//EM PRODUCAO
int carrinhoVest(int /*numero do cadastro*/, int /*outro parametro*/ );
int carrinhoEletro(/*algum parametro que eu ainda nao pensei*/);
void MenuAbrir(int NumeroCadastro);//abre um log j� existente
void MenuGerente(int opcao);//interface
void MenuCliente(int opcao);//interface
void MenuPesquisar(int opcao, char /*parametro da pesquisa*/);//pesquisa item por tipo e nome
void MenuRelatorios(/*parametros a definir*/);//emite rel�torio de v�rias coisas
void MenuExcluir(); //faltam parametros ainda para excluir produtos do carrinho
void MenuVisualizar(); //listar o carrinho do cliente
void MenuSuspender();//apenas salvar o carrinho com fwrite
void MenuFechar(); //salvar e fechar o carrinho
void MenuCancelar(int CodigoCliente); //cancelar 1 item do carrinho
void MenuInserir();//inserir com o codigo, um item no carrinho


//Variavel globais
USUARIO *usuarios;
CARRINHO carrinhos[20];
int numUsuarios=0;
ELETRO *eletronico;
int numItensEletro=0;
VEST *vestuario;
int numItensVest=0;

//Funcao main
int main(){
	lerUsuariosTexto("arquivos-de-entrada//usuarios.txt");

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

	move(((LINES-6)/2)+3, ((COLS-40)/2)+25);

	int codigoUsuario;
	scanw("%d", &codigoUsuario);

	if(codigoUsuario <= numUsuarios){
		mvprintw(0,0, "xalalallala");
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
	strcpy(usuarios[numUsuarios].categoria, "cliente");
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

void MenuAbrir(int NumeroCadastro){ //N�o acabado
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

int salvarUsuarioTexto(char arq[30]){ //pronta
	FILE *fp;
	if((fp = fopen(arq, "a+")) == NULL){
		printf("Erro ao abrir o arquivo %s\n", arq);
		exit(1);
	}
	fprintf(fp, "%d,%s,%s,%s,%s,%s,%d,%s\n", usuarios[numUsuarios-1].codigoUsuario, usuarios[numUsuarios-1].nomeUsuario, usuarios[numUsuarios-1].sobrenomeUsuario, usuarios[numUsuarios-1].endereco.logradouro, usuarios[numUsuarios-1].endereco.numero, usuarios[numUsuarios-1].endereco.complemento, usuarios[numUsuarios-1].endereco.cep, usuarios[numUsuarios-1].categoria);
	fclose(fp);
	return 1;
}

void LerCarrinho(int CodigoCliente){ //pronta
	FILE *fp;
	CARRINHO temp;
	if((fp = fopen("carrinho_usuario.sav", "rb")) == NULL){
		if((fp=fopen("carrinho_usuario.sav", "wb"))==NULL){
			printw("\n O arquivo nao pode ser criado.");
		        exit(1);
		}
	}
	int i = 0;
	while((fread(&temp, sizeof(CARRINHO), 1, fp))!=0){
		if(temp.CodigoCliente == CodigoCliente){
			carrinhos[i] = temp;
		}
	}
	fclose(fp);
}

void LerEletro(){
    FILE *fp;
    char linha[100]; //coloquei 100 porque acho que nunca vai ultrapassar disso
    if((fp=fopen("eletro.txt", "r"))==NULL){
        wprintf("Erro ao abrir o arquivo %s\n", "eletro.txt");
        exit(1);
    }
    while(fgets(linha,100,fp) != NULL){
        numItensEletro++;
    }
    if((eletronico=(ELETRO *)malloc(numItensEletro*sizeof(ELETRO)))==NULL){
		printf("Erro ao alocar memoria\n");
		exit(1);
        }
    int i=0;
    rewind(fp);
    while(fgets(linha,100,fp) != NULL){
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
                eletronico[i].descricao=(char*)malloc(tamDescricao* sizeof(char));
                strcpy(eletronico[i].descricao, pch);
                break;}
            case 2:
                eletronico[i].dimensEletro.altura=atoi(pch);
                break;
            case 3:
                eletronico[i].dimensEletro.largura=atoi(pch);
                break;
            case 4:
                eletronico[i].dimensEletro.altura=atoi(pch);
                break;
            case 5:
                eletronico[i].dimensEletro.comprimento=atoi(pch);
                break;
            case 6:
                eletronico[i].preco=atof(pch);
                break;
            case 7:
                eletronico[i].estoque=atoi(pch);
                break;
            case 8:{
                int tamCor=strlen(pch);
                eletronico[i].cor=(char*)malloc(tamCor*sizeof(char));
                strcpy(eletronico[i].cor, pch);
                break;}
            }
           pch = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
}

void LerVestuario(){
    FILE *fp;
    char linha[70];
    if((fp=fopen("vestuario.txt", "r"))==NULL){
        wprintf("Erro ao abrir o arquivo %s\n", "vestuario.txt");
        exit(1);
    }
    while((fgets(linha,70,fp))!=NULL){
        numItensVest++;
    }
    if((vestuario=(VEST *)malloc(numItensVest*sizeof(VEST)))==NULL){
       wprintf("\nErro ao alocar memoria!");
       exit(1);
       }
    int i=0;
    rewind(fp);
    while((fgets(linha, 70 ,fp))!=NULL){
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
            j++;
        }
        i++;
    }
    fclose(fp);
}

/*void MenuVisualizar(){
    CARRINHO *temp;
    temp=LerCarrinho(int CodigoCliente);
        while(temp!=FEOF){
            printw("%d %s %d %d", temp.) /carrinhos bugados for the win, me travaram aqui de novo
        }

}*/

/*
void MenuAbrir(int NumeroCadastro, /*op��o da interface, JANELA *janela){
    FILE *fp;
    i=0;
    if((fp=fopen("carrinho_usuario.sav", "r"))==NULL){
        printw("Erro ao abrir carrinho salvo.\n");
        destruirJanela(janela);//n�o sei se precisa do exit aqui j� que eu coloquei destruir janela, d� uma testada nisso
    }
    fseek(fp,NumeroCadastro,SEEK_SET);
    for(i=NumeroCadastro;i<NumeroCadastro+1;i++)
    carrinhos[f]= //empaquei aqui por causa dos nossos carrinhos que est�o bugados
*/

void MenuCancelar(int CodigoCliente){
    free(carrinhos[f]); //decidir o que vai ser o �ndice f, n�o consegui adequar o c�digo do cliente a ele
}

void MenuInserir(int NumeroCadastro){
    int item;
    int quant;
    printw("Informe o c�digo do produto:\n");
    scanw("%d", &item);
    printw("Informe a quantidade do produto:\n");
    scanw("%d", &quant);
    //a gente precisa consertar os carrinhos ainda pra terminar essa fun��o
}

