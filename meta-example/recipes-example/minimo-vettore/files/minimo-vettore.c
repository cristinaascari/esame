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
 				
	int min=0;
	int j=0;
	for(int i=0; i<len; ++i){
		if(i==0){
			min=v[i];
			j=0;
		}
		else {
			if(min>v[i]){
				min=v[i];
				j=i;
			}
		}

	}
	printf("Il valore minore del vettore è: %d e si trova in posizione %d\n", min, j);
	return min;
}

