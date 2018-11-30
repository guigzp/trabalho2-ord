#include "header.h"

FILE* btfd;

int contPage = 0;

getLine (char *str, FILE *arq){
	int i = 0;
	fgets(str, 20, arq);
	while (str[i] != '\n' && str[i] != '\0') i++;
	str[i] = '\0';	// adiciona o fim de string
}

int btopen(){
	btfd = fopen("btree.txt", "r+");
	return (btfd == NULL);
}

void btclose(){
	fclose(btfd);
}

int getRoot(){
	int root;
	rewind(btfd);
	fread(&root, sizeof(root), 1, btfd);
	return root;
}

void putRoot(int root){
	rewind(btfd);
	fwrite(&root, sizeof(root), 1, btfd);
}

pageInit(btpage* pagina){
	int i;
	pagina->qtd_chaves = 0;
	for(i = 0; i < MAXKEYS; i++){
		pagina->chaves[i] = NIL;
		pagina->filhos[i] = NIL;
		pagina->offsets[i] = NIL;
	}
	pagina->filhos[MAXKEYS] = NIL;
}

int createTree(){
	btfd = fopen("btree.txt", "w");
	fclose(btfd);
	btopen();
	return createRoot(NIL, NIL, NIL, NIL);
}

int createRoot(int key, int offset, int left, int right){
	btpage page;
	int rrn = 0;
	pageInit(&page);
	page.chaves[0] = key;
	page.filhos[0] = left;
	page.filhos[1] = right;
	page.offsets[0] = offset;
	page.qtd_chaves = 0;
	btwrite(rrn, &page);
	putRoot(rrn);
	return rrn;	
}

btwrite(int rrn, btpage* page){
	int offset = (rrn * PAGESIZE) + 4;
	fseek(btfd, offset, SEEK_SET);
	fwrite(page, PAGESIZE, 1, btfd);
}

btpage* btread(int rrn){
	btpage *aux;
	aux = (btpage*)calloc(1, PAGESIZE);
	int offset = (rrn * PAGESIZE) + 4;
	fseek(btfd, offset, SEEK_SET);
	fread(aux, PAGESIZE, 1, btfd);
	return aux;
}

int search_node(int key, btpage *p_page, int *pos){
	int i;
	for (i = 0;  i < p_page->qtd_chaves  &&   key > p_page->chaves[i] ; i++ );
    *pos = i;
    if (  (*pos < p_page->qtd_chaves)  &&  (key == p_page->chaves[*pos] ) ) {
    	return YES;
	}else{
		return NO;
	}
}

ins_in_page(int key, int r_child, btpage *p_page){
    int i;
    for (i = p_page->qtd_chaves; key < p_page->chaves[i-1] && i > 0; i--) {
        p_page->chaves[i] = p_page->chaves[i-1];
        p_page->filhos[i+1] = p_page->filhos[i];
    }
    p_page->qtd_chaves++;
    p_page->chaves[i] = key;
    p_page->filhos[i+1] = r_child;
}

int getPage(){
	int offset;
	fseek(btfd, -4, SEEK_END);
	offset = ftell(btfd);
	return (offset/PAGESIZE);
}

printaPage (btpage *page){
	printf("Quantidade de Chaves: %d\n", page->qtd_chaves);
	printf("Chaves:\t %d | %d | %d | %d\n", page->chaves[0], page->chaves[1], page->chaves[2], page->chaves[3]);
	printf("Offsets: %d | %d | %d | %d\n", page->offsets[0], page->offsets[1], page->offsets[2], page->offsets[3]);
	printf("Filhos:\t %d | %d | %d | %d | %d\n\n", page->filhos[0], page->filhos[1], page->filhos[2], page->filhos[3], page->filhos[4]);
}

insert (int rrn, int key, int *promo_r_child, int *promo_key)
{
    btpage *page,         /* current page                      */
           newpage;      /* new page created if split occurs  */
    int found, promoted; /* boolean values                    */
    int  pos,
           p_b_rrn;      /* rrn promoted from below           */
    int   p_b_key;      /* key promoted from below           */

    if (rrn == NIL) {           /* past bottom of tree... "promote" */
        *promo_key = key;       /* original key so that it will be  */
        *promo_r_child = NIL;   /* inserted at leaf level           */
        return (YES);	  
    }
    page = btread(rrn);          
    found = search_node(key, page, &pos);
    if (found) {
    	printf("Error: attempt to insert duplicate key: %c \n\007", key);
    	return (0);
    }
    promoted = insert(page->filhos[pos], key, &p_b_rrn, &p_b_key);
    if (!promoted)
    	return (NO);  /* no promotion */
    if (page->qtd_chaves < MAXKEYS) {                  
    	ins_in_page(p_b_key, p_b_rrn, page);   /* OK to insert key and  */
    	btwrite(rrn, page);                    /* pointer in this page. */
    	return (NO);  /* no promotion */
    }
    else {
    	//split(p_b_key, p_b_rrn, &page, promo_key, promo_r_child, &newpage);
    	btwrite(rrn, page);
    	btwrite(*promo_r_child, &newpage);
    	return (YES);   /* promotion */
    }
}
