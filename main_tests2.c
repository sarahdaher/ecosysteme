#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "ecosys.h"

float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

int main(void) {
	int i;
 	Animal *liste_proie = NULL;
 	Animal *liste_predateur = NULL;
 	int energie=10;
 	
 	for(i=0; i<20; i++){
 		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie, &(liste_proie));
 		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie, &(liste_predateur));
 	}
 	assert(compte_animal_it(liste_proie) == 20);
 	assert(compte_animal_it(liste_predateur) == 20);
 	
 	afficher_ecosys(liste_proie, liste_predateur);
 	
	ecrire_ecosys("liste_proies.txt", liste_predateur, liste_proie);
	
	Animal *liste_proies= NULL;
 	Animal *liste_predateurs = NULL;
 	
	lire_ecosys("liste_proies.txt", &liste_predateurs, &liste_proies);
	ecrire_ecosys("liste_proie.txt", liste_predateurs, liste_proies);
	
 	enlever_animal(&liste_proie, liste_proie);
 	enlever_animal(&liste_proie, liste_proie->suivant);
 	enlever_animal(&liste_proie, liste_proie);
 	enlever_animal(&liste_predateur, liste_predateur);
 	enlever_animal(&liste_predateur, liste_predateur);
 	
 	afficher_ecosys(liste_proies, liste_predateurs);
 	
 	liberer_liste_animaux(liste_proie);
 	liberer_liste_animaux(liste_predateur);
 	liberer_liste_animaux(liste_proies);
 	liberer_liste_animaux(liste_predateurs);
 	
 	
 	
 	
 	return 0;
 }
