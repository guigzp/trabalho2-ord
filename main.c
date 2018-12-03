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
  	btread(0, &aux);
  	printaPage(&aux);
//  	promoted = insert(root, 13, &promo_rrn, &promo_key);
//  	aux = btread(0);
//  	printaPage(aux);
//  	promoted = insert(root, 12, &promo_rrn, &promo_key);
//  	aux = btread(0);
//  	printaPage(aux);
//  	promoted = insert(root, 25, &promo_rrn, &promo_key);
//  	aux = btread(0);
//  	printaPage(aux);
//  	promoted = insert(root, 11, &promo_rrn, &promo_key);
//  	root = createRoot(promo_key, -1, root, promo_rrn);
//  	aux = btread(0);
//  	printaPage(aux);
//  	aux = btread(1);
//  	printaPage(aux);
    btclose();
}
