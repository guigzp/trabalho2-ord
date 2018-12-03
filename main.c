#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(){
    int root;
    root = createTree();
  	int promo_rrn, promo_key, promoted;
  	btpage aux;
//  	btread(0,aux);
//  	printaPage(aux);
  	promoted = insert(root, 10, &promo_rrn, &promo_key); 
//  	btread(0, &aux);
//  	printaPage(&aux);
  	promoted = insert(root, 13, &promo_rrn, &promo_key);
//  	btread(0, &aux);
//  	printaPage(aux);
  	promoted = insert(root, 12, &promo_rrn, &promo_key);
//  	btread(0, &aux);
//  	printaPage(aux);
  	promoted = insert(root, 25, &promo_rrn, &promo_key);
//  	btread(0, &aux);
//  	printaPage(aux);
  	promoted = insert(root, 11, &promo_rrn, &promo_key);
  	root = createRoot(promo_key, -1, root, promo_rrn, 0);
  	printf("Pagina Raiz: %d\n\n", getRoot());
  	printf("Pagina 0\n");
  	btread(0, &aux);
  	printaPage(aux);
  	printf("Pagina 1\n");
  	btread(1, &aux);
  	printaPage(aux);
  	printf("Pagina 2\n");
  	btread(2, &aux);
  	printaPage(aux);
    btclose();
}
