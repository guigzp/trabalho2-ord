#include "header.h"

FILE* btfd;

int contPage = 0;

getLine (char *str, FILE *arq){
	int i = 0;
	fgets(str, 20, arq);
	while (str[i] != '\n' && str[i] != '\0') i++;
	str[i] = '\0';	// adiciona o fim de string
}

importarArquivo(char nome_arquivo[50]){
	char buffer[100], string[20];
	int root;
    root = createTree();
  	int promo_rrn, promo_key, promo_offset, promoted;
	int quantidade, i, j, id, offset = 4;
	FILE* arq = fopen(nome_arquivo, "r");
	if(arq == NULL){
		printf("Falha na abertura do arquivo!\n");
	}else{
		getLine(string, arq);
		quantidade = atoi(string);
		for(i = 0; i < quantidade; i++){
			for(j = 0; j < 4; j++){
				getLine(string, arq);
				if(j == 0){
					id = atoi(string);
				}
				strcat(buffer, string);
			}
			promoted = insert(root, id, offset, &promo_rrn, &promo_key, &promo_offset);
			if(promoted){
				root = createRoot(promo_key, promo_offset, root, promo_rrn, 0);
			}
			offset = offset + strlen(buffer);		// calcula o offset de cada registro
			buffer[0] = '\0';						// limpa o buffer
		}
		printf("Arquivo importado com sucesso!\n");
		fclose(arq);
	}
}

void printaPaginas(){
	int quantidade = getPage(), root = getRoot(), i;
	btpage aux;
	for(i = 0; i < quantidade; i++){
		if(i == root){
			printf("-----PAGINA RAIZ-----\n");
		}
		printf("RRN %d\n", i);
  		btread(i, &aux);
  		printaPage(&aux);
	}
}


void printaMenu(){
	system("cls");
	printf("Menu Cadastro de Cães com Árvore B\n");
	printf("Opções: \n");
	printf("1) Importar um arquivo e construir a Árvore B\n");
	printf("2) Listar a Árvore B\n");
	printf("3) Buscar um cão por ID-I\n");
	printf("4) Inserir um cão\n");
	printf("0) Sair\n");
	printf("\nDigite sua opção: ");
}

void menu(){
	int opcao = 1, aux, pos, offset;
	btpage page;
	char nome_arquivo[50];
	while(opcao > 0 && opcao < 5){
		
		printaMenu();
		scanf("%d", &opcao);
		
		switch(opcao){
			case 1:
				system("cls");
				printf("Digite o nome do arquivo a ser importado: ");
				scanf("%s", nome_arquivo);
				printf("%s\n", nome_arquivo);
				importarArquivo(nome_arquivo);
				system("PAUSE");
				break;

			case 2:
				system("cls");
				printaPaginas();
				system("PAUSE");
				break;

			case 3:
				system("cls");
				printf("Digite o ID do cão a ser procurado: ");
				scanf("%d", &aux);
				btread(getRoot(), &page);
				offset = procura(aux, &page, pos);
				if(offset == -1){
					printf("Cão não cadastrado!\n");
				}else{
					lerRegistroArquivo(offset, nome_arquivo);
				}
				system("PAUSE");
				break;

			case 4:
				system("cls");
				system("PAUSE");
				break;
			default:
				printf("Encerrando o programa!\n");
		}
	}
}

int procura(int key, btpage *page, int pos){
	if(search_node(key, page, &pos)){
		return page->offsets[pos];
	}else if(page->filhos[pos] == -1){
		return -1;
	}else{
		btread(page->filhos[pos], page);
		return procura(key, page, pos);
	}
}


void lerRegistroArquivo(int offset, char nome_arquivo[50]){
	FILE *arq = fopen(nome_arquivo, "r");
	fseek(arq, offset, SEEK_SET);
	char string[20];
	getLine(string, arq);
	printf("\nID-I Cão: %s", string);
	getLine(string, arq);
	printf("\nID-Raça: %s", string);
	getLine(string, arq);
	printf("\nNome: %s", string);
	getLine(string, arq);
	printf("\nSexo: %s\n", string);	
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
	return createRoot(NIL, NIL, NIL, NIL, 1);
}

int createRoot(int key, int offset, int left, int right, int primeira){
	btpage page;
	int rrn = getPage();
	pageInit(&page);
	page.chaves[0] = key;
	page.filhos[0] = left;
	page.filhos[1] = right;
	page.offsets[0] = offset;
	page.qtd_chaves = 1;
	if(primeira){
		page.qtd_chaves = 0;
	}
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

ins_in_page(int key, int offset, int r_child, btpage *p_page){
    int i;
    for (i = p_page->qtd_chaves; key < p_page->chaves[i-1] && i > 0; i--) {
        p_page->chaves[i] = p_page->chaves[i-1];
        p_page->filhos[i+1] = p_page->filhos[i];
        p_page->offsets[i] = p_page->offsets[i-1];
    }
    p_page->qtd_chaves++;
    p_page->chaves[i] = key;
    p_page->filhos[i+1] = r_child;
    p_page->offsets[i] = offset;
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

insert (int rrn, int key, int offset, int *promo_r_child, int *promo_key, int *promo_offset)
{
    btpage page, newpage;
    int found, promoted, pos, p_b_rrn, p_b_key, p_b_offset;

    if (rrn == NIL) {
        *promo_key = key; 
        *promo_r_child = NIL;
        *promo_offset = offset;
        return YES;	  
    }
    
    btread(rrn, &page);        
    found = search_node(key, &page, &pos);
    
    if (found) {
    	printf("Erro, chave duplicada: %d \n", key);
    	return 0;
    }
    promoted = insert(page.filhos[pos], key, offset, &p_b_rrn, &p_b_key, &p_b_offset);
    
	if (!promoted)
    	return NO;
    if (page.qtd_chaves < MAXKEYS) {            
    	ins_in_page(p_b_key, p_b_offset, p_b_rrn, &page);
    	btwrite(rrn, &page);              
    	return NO;
    }
    else {
    	split(p_b_key, p_b_offset, promo_offset, p_b_rrn, &page, promo_key, promo_r_child, &newpage);
    	btwrite(rrn, &page);
    	btwrite(*promo_r_child, &newpage);
    	return YES;
    }
}

split(int key, int offset, int *promo_offset, int r_child, btpage *p_oldpage, int *promo_key, int *promo_r_child, btpage *p_newpage)
{
    int i, aux_chaves[MAXKEYS+1], aux_filhos[MAXKEYS+2], aux_offsets[MAXKEYS+1];
    for (i=0; i < MAXKEYS; i++) {        
	aux_chaves[i] = p_oldpage->chaves[i]; 
	aux_offsets[i] = p_oldpage->offsets[i];
	aux_filhos[i] = p_oldpage->filhos[i];
    }
    
    aux_filhos[i] = p_oldpage->filhos[i];    
	         
    for (i=MAXKEYS; key < aux_chaves[i-1] && i > 0; i--) {
        aux_chaves[i] = aux_chaves[i-1];
        aux_offsets[i] = aux_offsets[i-1];
        aux_filhos[i+1] = aux_filhos[i];
    }
    aux_chaves[i] = key;
    aux_offsets[i] = offset;
    aux_filhos[i+1] = r_child;

    *promo_r_child = getPage();          
    pageInit(p_newpage);                   

    for (i = 0; i < MINKEYS; i++) {        
    	p_oldpage->chaves[i] = aux_chaves[i];   
		p_oldpage->filhos[i] = aux_filhos[i];
		p_oldpage->offsets[i] = aux_offsets[i];
		p_newpage->chaves[i] = aux_chaves[i+1+MINKEYS];
        p_newpage->filhos[i] = aux_filhos[i+1+MINKEYS];
        p_newpage->offsets[i] = aux_offsets[i+1+MINKEYS];
		p_oldpage->chaves[i+MINKEYS] = NIL;     
		p_oldpage->filhos[i+1+MINKEYS] = NIL;
		p_oldpage->offsets[i+MINKEYS] = NIL;  
	}
    p_oldpage->filhos[MINKEYS] = aux_filhos[MINKEYS];
    p_newpage->filhos[MINKEYS] = aux_filhos[i+1+MINKEYS];
    p_newpage->qtd_chaves = MAXKEYS - MINKEYS;
    p_oldpage->qtd_chaves = MINKEYS;
    *promo_key = aux_chaves[MINKEYS];
    *promo_offset = aux_offsets[MINKEYS];
}
