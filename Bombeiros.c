#include <stdio.h>
#include <stdlib.h>

#define Q_BATALHOES 50
#define Q_INCIDENTES 1000

typedef struct {
int x;
int y;
} coord;

typedef struct {
coord local;
float raio;
int capac;
} batalhao;

typedef struct {
coord local;
} incidente;

batalhao lista_de_batalhoes[Q_BATALHOES];
incidente lista_de_incidentes[Q_INCIDENTES];

void questao01(char *arqbat, char *arqinci, int *qbat, int *qinci);
void questao02(char *arq, int qbat, int qinci);

int main() {
int qtde_bat, qtde_inci;
questao01("batalhoes.txt", "incidentes.txt", &qtde_bat, &qtde_inci);
questao02(“saida.txt”, qtde_bat, qtde_inci);
return 0;
}

void questao01(char qbat, char qinci, int *qte_bat, int *qinci){
	/*FILE *bat_file, *inci_file;
	bat_file=fopen(qbat, "r");
	inci_file=fopen(qinci, "r");
	 int i=0;
	 qte_bat=0;
	while(fscanf(bat_file,"%d %d %f %d", lista_de_batalhoes[i].coord.x, lista_de_batalhoes[i].coord.y, lista_de_batalhoes[i].raio, lista_de_batalhoes[i].capac)!=EOF){
		i++;
		qte_bat=*qte_bat+1;
		}*/
	FILE *arq;
	if((arq=fopen(arqbat,"r"))==NULL){
		printf("ERRO");
		EXIT(1);	
		}
		*qbat=0;
		while(!feof(arq)){
			fscanf(arq, "%d %d %f %d" &lista_de_batalhoes[*qbat].local.x, &lista_de_batalhoes[*qbat].local.y, &lista_de_batalhoes[*qbat].raio, &lista_de_batalhoes[*qbat].capac)
			(*qbat)++;
			}
			fclose(arq);
	}
int questao02(char *arq, qte_bat, qte_inci){
	FILE *saida;
	int i, j;
	float dist;
	if ((saida=fopen(arq,"w"))==NULL){
		printf("ERRO");
		EXIT(1);		
		} 
		for(i=0,i<qte_inci;i++){
			fprintf(saida, "%d: ", i);
			for(j=0;j<qte_bat;j++){
				dist=sqrt(pow(lista_de_incidentes[i].local.x-lista_de_batalhoes[j].local.x, 2)+pow(lista_de_incidentes[i].local.y-lista_de_batalhoes[i].local.y, 2));
				if(dist<=lista_de_batalhoes.raio){
					fprintf(saida,"%.2f",dist*0,5);
					}
					else {
						fprintf(saida, "-1");
						}
				}
				fprintf(saida,"\n");
		}
		fclose(saida);
	}
	


