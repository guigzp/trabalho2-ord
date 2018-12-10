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
extern FILE* infd;

int btopen();
void btclose();
void putRoot(int root);
int getRoot();
void btread(int rrn, btpage *aux);
void lerRegistroArquivo(int offset, char nome_arquivo[50]);


