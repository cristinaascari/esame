#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int Elfi_con_problemi = 0; /*variabile condivisa tra gli elfi e Babbo Natale per sapere quanti elfi hanno bisogno di aiuto*/
int Renne_arrivate = 0; /*variabile condivisa tra le renne e Babbo Natale per sapere quante renne dsono arrivate dai tropici*/
bool Finito = false; /*variabile condivisa tra le renne e Babbo Natale per sapere se sono arrivate tutte e nove le renne, in queso modo Babbo Natale sa quando può terminare la sua esecuzione*/
sem_t mutex; /*semaforo per accedere alla variabile condivisa elfi_con_problemi e a renne_arrivate*/
sem_t Babbo_Natale; /*semaforo sul quale Babbo Natale si addormenta in attesa di essere risvegliato o dalle renne o dagli elfi*/
sem_t Renne; /*semaforo sul quale le renne si fermano per aspettare che tutte e nove siano arrivate*/
sem_t Elfi; /*semaforo sul quale si fermano gli elfi che attendono il ritorno dei 3 elfi partiti*/
sem_t Help; /*semaforo sul quale si fermano gli elfi in attesa del proprio turno di essere aiutati*/
void PrepareSleigh(){
	printf("*** Sono Babbo Natale, sono arrivate tutte e nove le renne e quindi inizio a preparare la slitta *** \n");
	for(int k = 0 ; k < 9 ; ++k){
		sem_post(&Renne);
	}
}

void HelpElves(){
	printf("*** Sono Babbo Natale, sono arrivati tre elfi che hanno bisogno di aiuto con la preparazione dei giocattoli **************************************** \n");
	for(int k = 0 ; k < 3 ; ++k){
		sem_post(&Help);
	}
}

void GetHelp(int i){
	printf("--- Sono l' elfo numero %d e sono stato aiutato ------------------------------------------------------------------------------ \n", i - 9);
}

void GetHitched(int i){
	printf("+++ Sono la renna numero %d e sono stata attaccata alla slitta +++ \n", i);
}

void RICHIEDI_AIUTO(int i){
		sem_wait(&Elfi);
		sem_wait(&mutex);
		Elfi_con_problemi  += 1;
		printf("sono l'elfo n.%d e ci sono %d elfi da aiutare-----------------------------------------------------------------------------------------\n", i, Elfi_con_problemi);
		if(Elfi_con_problemi == 3){
			printf("sono l'elfo n.%d e ho svegliato Babbo natale-----------------------------------------------------------------------------------------\n", i);
			sem_post(&Babbo_Natale);
		}else{
			sem_post(&Elfi);
		}
		sem_post(&mutex);
}

void RILASCIA_AIUTO(int i){
		sem_wait(&mutex);
		Elfi_con_problemi -= 1;
		if(Elfi_con_problemi == 0){
			sem_post(&Elfi);
		}
		sem_post(&mutex);
}

void *esegui_Elfo(void *id)
{
	int *pi = (int *)id;
	int *ptr;
	bool aiuto = false;
	ptr = (int *) malloc( sizeof(int));

	if (ptr == NULL)
	{
		perror("Problemi con l'allocazione di ptr\n");
		exit(-1);
	}
	printf("Thread%d partito: ho come identificatore %lu, sono un elfo \n", *pi, pthread_self());
	int n = (rand() % 3) + 1;
	if(n <= 2){
		aiuto = true;
		printf("--- Sono l'elfo n.%d e ho bisogno di aiuto ------------------------------------------------------------------------------------- \n", *pi -9);
	}

	if(aiuto == true){		
		RICHIEDI_AIUTO(*pi -9);
		printf("sono l'elfo n.%d e ho rilasciato il mutex e mi sto addormentando su Help-----------------------------------------------------------------------------------------\n", *pi -9);
		sem_wait(&Help);
		printf("sono l'elfo n.%d e mi hanno svegliato da Help-----------------------------------------------------------------------------------------\n", *pi -9);
		GetHelp(*pi);
		RILASCIA_AIUTO(*pi -9);		
	}
	*ptr = *pi;
	pthread_exit((void *) ptr);
}

void *esegui_Renna(void *id)
{
	int *pi = (int *)id;
	int *ptr;
	ptr = (int *) malloc( sizeof(int));
	if (ptr == NULL)
	{
		perror("Problemi con l'allocazione di ptr\n");
		exit(-1);
	}
	printf("Thread%d partito: ho come identificatore %lu, sono una renna \n", *pi, pthread_self());
	sleep(30);
	sem_wait(&mutex);
	Renne_arrivate += 1;
	if(Renne_arrivate == 9){
			sem_post(&Babbo_Natale);
			printf("+++ Sono l'ultima renna e ho preso Babbo Natale +++ \n");
		}
	sem_post(&mutex);
	sem_wait(&Renne);

	GetHitched(*pi);
	


	*ptr = *pi;
	pthread_exit((void *) ptr);
}

void *esegui_Babbo_Natale(void *id)
{
	int *pi = (int *)id;
	int *ptr;
	ptr = (int *) malloc( sizeof(int));
	if (ptr == NULL)
	{
		perror("Problemi con l'allocazione di ptr\n");
		exit(-1);
	}
	printf("Thread%d partito: ho come identificatore %lu, sono Babbo Natale \n", *pi, pthread_self());
	while(!Finito){
		printf("sono Babbo Natale e mi sono addormentato-------------------------------------------------------------------------------------------------\n");
		sem_wait(&Babbo_Natale);
		printf("sono Babbo Natale e qualcuno mi ha risvegliato---------------------------------------------------------------------------------------------\n");
		sem_wait(&mutex);
		if(Renne_arrivate == 9){
			PrepareSleigh();
			Finito = true;
		}else if(Elfi_con_problemi == 3){
			HelpElves();
		}
		sem_post(&mutex);
	}
	*ptr = *pi;
	pthread_exit((void *) ptr);
}

int main (int argc, char **argv)
{
	pthread_t *thread;
	int *taskids;
	int i;
	int *p;
	int NUM_THREADS;
	char error[250];
	/*Controllo sul numero di parametri */
	/*Deve essere passato esattamente un parametro*/
	if (argc != 2 ) 
	{
		printf(error,"Errore nel numero dei parametri %d\n", argc-1);
		perror(error);
       		exit(1);
	}
	/*Calcoliamo il numero passato che sara' il numero di elfi da creare quindi deve essere un numero strettamente positivo*/
	NUM_THREADS = atoi(argv[1]);
	if (NUM_THREADS <= 0) 
	{
		sprintf(error,"Errore: Il primo parametro non e' un numero strettamente maggiore di 0\n");
		perror(error);
        	exit(2);
   	}
	/*Il numero di Pthread da creare deve comprendere anche Babbo Natale e le 9 renne*/
	NUM_THREADS += 10;
	/*Allochiamo la memoria necessaria ai Pthread*/
	thread=(pthread_t *) malloc(NUM_THREADS * sizeof(pthread_t));
	if (thread == NULL)
	{
        	perror("Problemi con l'allocazione dell'array thread\n");
        	exit(3);
   	}
	taskids = (int *) malloc(NUM_THREADS * sizeof(int));
	if (taskids == NULL)
   	{
        	perror("Problemi con l'allocazione dell'array taskids\n");
        	exit(4);
    	}
	/*Creiamo i semafori per accedere alle variabili condivise*/
	sem_init(&mutex, 0, 1);
	sem_init(&Babbo_Natale, 0, 0);
	sem_init(&Renne, 0, 0);
	sem_init(&Elfi, 0, 1);
	sem_init(&Help, 0, 0);
	/*Creiamo i Pthread*/
	for (i=0; i < NUM_THREADS; i++)
   	{
		if(i==0){
			taskids[i] = i;
   			sprintf("Sto per creare il thread %d-esimo\n", taskids[i]);
        		if (pthread_create(&thread[i], NULL, esegui_Babbo_Natale , (void *) (&taskids[i])) != 0)
        		{
        	        	sprintf(error,"SONO IL MAIN E CI SONO STATI PROBLEMI DELLA CREAZIONE DEL thread %d-esimo\n", taskids[i]);
        	        	perror(error);
				exit(5);
        		}
			printf("SONO IL MAIN e ho creato il Pthread %i-esimo con id=%lu(Babbo Natale)\n", i, thread[i]);
		}else if((1<=i) && (i<=9)){
			taskids[i] = i;
   			printf("Sto per creare il thread %d-esimo\n", taskids[i]);
        		if (pthread_create(&thread[i], NULL, esegui_Renna , (void *) (&taskids[i])) != 0)
        		{
        	        	sprintf(error,"SONO IL MAIN E CI SONO STATI PROBLEMI DELLA CREAZIONE DEL thread %d-esimo\n", taskids[i]);
        	        	perror(error);
				exit(6);
        		}
			printf("SONO IL MAIN e ho creato il Pthread %i-esimo con id=%lu \n", i, thread[i]);
		}else{
 			taskids[i] = i;
   			printf("Sto per creare il thread %d-esimo\n", taskids[i]);
        		if (pthread_create(&thread[i], NULL, esegui_Elfo, (void *) (&taskids[i])) != 0)
        		{
        		        sprintf(error,"SONO IL MAIN E CI SONO STATI PROBLEMI DELLA CREAZIONE DEL thread %d-esimo\n", taskids[i]);
        		        perror(error);
				exit(7);
        		}
			printf("SONO IL MAIN e ho creato il Pthread %i-esimo con id=%lu\n", i, thread[i]);
		}
    	}
	/*Il main aspetta le renne e Babbo Natale*/
   	for (i=0; i < 10; i++)
   	{
		int ris;
   		pthread_join(thread[i], (void**) & p);
		ris= *p;
		printf("Pthread %d-esimo restituisce %d\n", i, ris);
   	} 
   	exit(0);
}
