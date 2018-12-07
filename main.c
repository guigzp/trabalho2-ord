#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(){
    int root;
    root = createTree();
  	int promo_rrn, promo_key, promo_offset, promoted;
  	btpage aux;
  	promoted = insert(root, 10, 4, &promo_rrn, &promo_key, &promo_offset); 
  	promoted = insert(root, 13,3, &promo_rrn, &promo_key, &promo_offset);
  	promoted = insert(root, 12, 1, &promo_rrn, &promo_key, &promo_offset);
  	promoted = insert(root, 25, 7, &promo_rrn, &promo_key, &promo_offset);
  	promoted = insert(root, 11, 5, &promo_rrn, &promo_key, &promo_offset);
  	root = createRoot(promo_key, promo_offset, root, promo_rrn, 0);
  	promoted = insert(root, 14, 10, &promo_rrn, &promo_key, &promo_offset);
  	promoted = insert(root, 26, 9, &promo_rrn, &promo_key, &promo_offset);
  	promoted = insert(root, 27, 15, &promo_rrn, &promo_key, &promo_offset);


  	printf("Pagina Raiz: %d\n\n", getRoot());
  	printf("Pagina 0\n");
  	btread(0, &aux);
  	printaPage(&aux);
  	printf("Pagina 1\n");
  	btread(1, &aux);
  	printaPage(&aux);
  	printf("Pagina 2\n");
  	btread(2, &aux);
  	printaPage(&aux);
  	printf("Pagina 3\n");
  	btread(3, &aux);
  	printaPage(&aux);
    btclose();
}
