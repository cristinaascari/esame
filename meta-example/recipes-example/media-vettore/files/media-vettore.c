#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(){
	int len;
	printf("Da quanti elementi deve essere formato il vettore? ");
	scanf("%d",&len);
	int v[len];
	int t;
       for(int i=0; i<len; ++i){
	       printf("Elemento %d:", i);
       		scanf("%d",&t);
		v[i]=t;

       }
 				
	int ris=0;
	for(int i=0; i<len; ++i){
		ris+= v[i];
	}
	ris=ris/len;
	printf("La media degli elementi del vettore è: %d\n", ris);
	return ris;
}

