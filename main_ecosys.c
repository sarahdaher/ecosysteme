#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000


/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.01;
float p_reproduce_proie= 0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;


int main(void) {
	srand(time(NULL));
	//on teste bouger animaux
	Animal* a = creer_animal(20,50, 10);
	a->dir[0] = 1;
	a->dir[1] = 1;
	bouger_animaux(a);
	printf("x = %d, y = %d, dir=[%d,%d]\n", a->x, a->y, a->dir[0], a->dir[1]); //on a bien la toricité du monde 
	
	Animal *liste_proie = NULL;
 	Animal *liste_predateur = NULL;
 	int energie=10;
 	int monde[SIZE_X][SIZE_Y];
 	
 	//on crée le fichier pour tracer les courbes
 	FILE * f = fopen("Evol_Pop.txt", "w");
	assert(f);
	
	
 	for(int i = 0; i<SIZE_X; i++){
 		for(int j = 0; j<SIZE_Y; j++){
 			monde[i][j] = 0;
 		}
 	}
 	
 	
 	
 	for(int i=0; i<20; i++){
 		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie, &(liste_proie));
 		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 20, &(liste_predateur));
 	}
 	
 	assert(compte_animal_it(liste_proie) == 20);
 	assert(compte_animal_it(liste_predateur) == 20);
 	afficher_ecosys(liste_proie, liste_predateur);
 	
 	//test reproduce avec proba = 1
 	reproduce(&liste_proie, 1);
 	reproduce(&liste_predateur, 1);
 	afficher_ecosys(liste_proie, liste_predateur);
 	
 	int i = 0; //cpt
 	
 	
 	while((liste_proie)&&(i<200)&&(liste_predateur)){
 		rafraichir_monde(monde);
		rafraichir_proies(&liste_proie,monde);
		rafraichir_predateurs(&liste_predateur, &liste_proie);
		afficher_ecosys(liste_proie,liste_predateur);
		printf("i :%d\n", i);
		fprintf(f, "%d %d %d\n", i, compte_animal_it(liste_proie), compte_animal_it(liste_predateur));
		i++;
    usleep(1000000);
	}
  
	fclose(f);
    liberer_liste_animaux(liste_proie);
 	liberer_liste_animaux(liste_predateur);
	free(a);
	
  return 0;
}

