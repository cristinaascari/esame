#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(){
	int len;
	printf("Da quanti elementi devono essere formati i vettori? ");
	scanf("%d",&len);
	int v1[len];
	int v2[len];
	int t1,t2;
       for(int i=0; i<len; ++i){
	       printf("Elemento %d del primo vettore:", i);
       		scanf("%d",&t1);
		v1[i]=t1;

       }
       for(int i=0; i<len; ++i){
               printf("Elemento %d del secondo vettore:", i);
                scanf("%d",&t2);
                v2[i]=t2;

       }

       int ris[len];
       for(int i=0; i<len; ++i){
	       ris[i]= v1[i]*v2[i];
       }

       printf("Il vettore risultante dal prodotto dei due vettori dati è:\n");
       for(int i=0; i<len; ++i){
	       if (i==len-1)
		       printf("%d.\n", ris[i]);
	       else
		       printf("%d, ", ris[i]);

       }
       return 0;
}

