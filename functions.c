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
	btfd = fopen("btree.txt", "rb+");
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
	btfd = fopen("btree.txt", "wb");
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

void btread(int rrn, btpage *aux){
	int offset = (rrn * PAGESIZE) + 4;
	fseek(btfd, offset, SEEK_SET);
	fread(aux, PAGESIZE, 1, btfd);
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
    btpage page, newpage;
    int found, promoted, pos, p_b_rrn, p_b_key;

    if (rrn == NIL) {
        *promo_key = key; 
        *promo_r_child = NIL;
        return YES;	  
    }
    
    btread(rrn, &page);        
    found = search_node(key, &page, &pos);
    
    if (found) {
    	printf("Erro, chave duplicada: %d \n", key);
    	return 0;
    }
    promoted = insert(page.filhos[pos], key, &p_b_rrn, &p_b_key);
    if (!promoted)
    	return NO;
    if (page.qtd_chaves < MAXKEYS) {                  
    	ins_in_page(p_b_key, p_b_rrn, &page);
    	btwrite(rrn, &page);              
    	return NO;
    }
    else {
    	split(p_b_key, p_b_rrn, &page, promo_key, promo_r_child, &newpage);
    	btwrite(rrn, &page);
    	btwrite(*promo_r_child, &newpage);
    	return YES;
    }
}

split(int key, int r_child, btpage *p_oldpage, int *promo_key, int *promo_r_child, btpage *p_newpage)
{
    int i, mid, workkeys[MAXKEYS+1], workch[MAXKEYS+2];

    for (i=0; i < MAXKEYS; i++) {        
	workkeys[i] = p_oldpage->chaves[i]; 
	workch[i] = p_oldpage->filhos[i];
    }
    
    workch[i] = p_oldpage->filhos[i];    
	         
    for (i=MAXKEYS; key < workkeys[i-1] && i > 0; i--) {
        workkeys[i] = workkeys[i-1];
        workch[i+1] = workch[i];
    }
    workkeys[i] = key;
    workch[i+1] = r_child;

    *promo_r_child = getPage();            
    pageInit(p_newpage);                   

    for (i = 0; i < MINKEYS; i++) {        
    	p_oldpage->chaves[i] = workkeys[i];   
		p_oldpage->filhos[i] = workch[i];   
		p_newpage->chaves[i] = workkeys[i+1+MINKEYS];
        p_newpage->filhos[i] = workch[i+1+MINKEYS];
		p_oldpage->chaves[i+MINKEYS] = NIL;     
		p_oldpage->filhos[i+1+MINKEYS] = NIL;
	}
    p_oldpage->filhos[MINKEYS] = workch[MINKEYS];
    p_newpage->filhos[MINKEYS] = workch[i+1+MINKEYS];
    p_newpage->qtd_chaves = MAXKEYS - MINKEYS;
    p_oldpage->qtd_chaves = MINKEYS;
    *promo_key = workkeys[MINKEYS];    
}
