#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(){
    int root;
    root = createTree();
  	int promo_rrn, promo_key, promoted;
  	btpage *aux = btread(0);
  	printaPage(aux);
  	promoted = insert(root, 10, &promo_rrn, &promo_key); 
  	aux = btread(0);
  	printaPage(aux);
  	promoted = insert(root, 13, &promo_rrn, &promo_key);
  	aux = btread(0);
  	printaPage(aux);
  	promoted = insert(root, 12, &promo_rrn, &promo_key);
  	aux = btread(0);
  	printaPage(aux);
  	promoted = insert(root, 25, &promo_rrn, &promo_key);
  	promoted = insert(root, 52, &promo_rrn, &promo_key);
  	promoted = insert(root, 30, &promo_rrn, &promo_key);
  	aux = btread(0);
  	printaPage(aux);
  	aux = btread(1);
  	printaPage(aux);
    btclose();
}
