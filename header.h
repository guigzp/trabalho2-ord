#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define MAXKEYS 4
#define MINKEYS MAXKEYS/2
#define NIL (-1)
#define NO 0
#define YES 1

typedef struct{
	int qtd_chaves;
	int chaves[MAXKEYS];
	int filhos[MAXKEYS +1];
	int offsets[MAXKEYS];
}btpage;

#define PAGESIZE sizeof(btpage)

extern int root;
extern FILE* btfd;

int btopen();
void btclose();
void putRoot(int root);
int getRoot();
void putRoot(int root);
void pageInit(btpage* pagina);
int createTree();
int createRoot(int key, int offset, int left, int right, int primeira);
void ins_in_page(int key, int offset, int r_child, btpage *p_page);
int search_node(int key, btpage *p_page, int *pos);
void printaPage (btpage *page);
int insert (int rrn, int key, int offset, int *promo_r_child, int *promo_key, int *promo_offset);
void split(int key, int offset, int *promo_offset, int r_child, btpage *p_oldpage, int *promo_key, int *promo_r_child, btpage *p_newpage);
int getPage();
void btwrite(int rrn, btpage* page);
void btread(int rrn, btpage *aux);
void lerRegistroArquivo(int offset);
void getLine (char *str, FILE *arq);
void getLinePipe(char *str, FILE *arq);
void importarArquivo(char nome_arquivo[50]);
void printaPaginas();
void printaMenu();
void menu();
void insereCao();
int procura(int key, btpage *page, int pos);



