#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"
#define SIZE 100
/* PARTIE 1*/
/* Fourni: Part 1, exercice 3, question 2 */

//fonction qui crée un animal en position (x,y), d'energie energie, de directions aleatoires entre -1 0 et 1 

Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 3, question 3 */

//fonction qui ajoute l'animal passé en paramètre en tête de la liste chaînée liste

Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 5, question 1 */

//fonction qui crée un animal d'energie energie en position (x,y) puis l'ajoute en tête de la liste chaînee

void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
	assert(0<=x<= SIZE_X);
	assert(0<=y<= SIZE_Y);
	Animal* a = creer_animal(x, y, energie);
	*liste_animal = ajouter_en_tete_animal(*liste_animal,a);

}

/* A Faire. Part 1, exercice 5, question 5 */

//fonction qui enleve (et libère) de la liste chaînée l'animal passé en argument 

void enlever_animal(Animal **liste, Animal *animal) {
	Animal* prec = NULL, *tmp = *liste;
	while(tmp != NULL){
		if (tmp == animal){
			if(prec == NULL){
				*liste = tmp ->suivant;
				free(tmp);
				return;
			}
			else{
				prec->suivant = tmp ->suivant;
				free(tmp);
				return;
			}
		}
		else{
			prec = tmp;
			tmp = tmp ->suivant;
		}
	}
	return;
}
	
/* A Faire. Part 1, exercice 5, question 2 */

//fonction qui libère la liste d'animaux

Animal* liberer_liste_animaux(Animal *liste) {
   Animal * temp = liste;
   while(liste){
		temp = liste->suivant;
		free(liste);
		liste = temp;
	}
		

  return NULL;
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 4, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
  	assert(0 <= pa->x <= SIZE_X);
  	assert(0 <= pa->y <= SIZE_Y);
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

void ecrire_ecosys(const char* nom_fichier, Animal* liste_pred, Animal* liste_proies){
	FILE * ecosys = fopen(nom_fichier, "w");
	assert(ecosys);
	fprintf(ecosys, "<proies>\n");
	
	while(liste_proies){
		fprintf(ecosys, "x=%d y=%d dir=[%d, %d] e=%f \n", liste_proies->x, liste_proies->y,liste_proies->dir[0],liste_proies->dir[1],liste_proies-> energie);
		liste_proies = liste_proies->suivant;
	}
	fprintf(ecosys, "</proies>\n");
	fprintf(ecosys, "<preds>\n");
	
	while(liste_pred){
		fprintf(ecosys, "x=%d y=%d dir=[%d, %d] e=%f \n", liste_pred->x, liste_pred->y,liste_pred->dir[0],liste_pred->dir[1],liste_pred-> energie);
		liste_pred = liste_pred->suivant;
	}
	fprintf(ecosys, "</preds>\n");
	fclose(ecosys);
}

void lire_ecosys(const char* nom_fichier, Animal** liste_pred, Animal** liste_proies){
	FILE *f = fopen(nom_fichier, "r");
	assert(f);
	char buffer[SIZE];
	if((fgets(buffer, SIZE, f) == NULL) || (strcmp(buffer, "<proies>\n") != 0)){
		exit(-1);
	}
	while((fgets(buffer, SIZE, f) != NULL) && (strcmp(buffer, "</proies>\n") != 0)){
		int x,y,dir[2];
		float energie;
		int n = sscanf(buffer, "x=%d y=%d dir=[%d, %d] e=%f \n", &x,&y,&dir[0], &dir[1], &energie);
		assert(n==5);
		Animal* a = creer_animal(x,y,energie);
		a->dir[0] = dir[0];
		a->dir[1] = dir[1];
		*liste_proies = ajouter_en_tete_animal(*liste_proies, a);
	}
	
	if((fgets(buffer, SIZE, f) == NULL) || (strcmp(buffer, "<preds>\n") != 0)){
		exit(-1);
	}
	while((fgets(buffer, SIZE, f) != NULL) && (strcmp(buffer, "</preds>\n") != 0)){
		int x,y,dir[2];
		float energie;
		int n = sscanf(buffer, "x=%d y=%d dir=[%d, %d] e=%f \n", &x,&y,&dir[0], &dir[1], &energie);
		assert(n==5);
		Animal* a = creer_animal(x,y,energie);
		a->dir[0] = dir[0];
		a->dir[1] = dir[1];
		*liste_pred = ajouter_en_tete_animal(*liste_pred, a);
	}
	fclose(f);
	
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
	while(la){
		if((float) rand()/(float) RAND_MAX< p_ch_dir){
			la->dir[0] = (rand()%3)-1;
			la->dir[1] = (rand()%3)-1;
		}
		la->x = (la->x + la->dir[0] + SIZE_X)%SIZE_X;
		la->y = (la->y + la->dir[0] + SIZE_Y)%SIZE_Y;
		la =la->suivant;
	}
		
}


void reproduce(Animal **liste_animal, float p_reproduce) {
  assert(liste_animal);
  Animal *a = *liste_animal;
  while (a) {
    if ((float)rand() / (float)RAND_MAX <= p_reproduce) {
      ajouter_animal(a->x, a->y, a->energie / 2, liste_animal);
      a->energie /=  2;
    }
    a = a->suivant;
  }
}

/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
	if(*liste_proie){
    	Animal *lp = *liste_proie;
    	bouger_animaux(lp);
    	while(lp){
    		((lp)-> energie) --;
    		Animal* temp = (lp)->suivant;
    		if ((lp-> energie)<0){
    			enlever_animal(liste_proie, lp);
    		}
    		else if( monde[lp->x][lp->y] >0){
    			(lp->energie)+=monde[lp->x][lp->y];
    			monde[lp->x][lp->y] =  temps_repousse_herbe;
    		}
    		lp = temp;
    	}
    reproduce(liste_proie, p_reproduce_proie);
    }

}


/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    Animal *liste = l;
    while(liste){
    	if(liste->x ==x && liste->y == y){
    		return liste;
    	}
    	liste = liste->suivant;
    }

  return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
	if(*liste_predateur){
    	Animal *lp = *liste_predateur;
    	bouger_animaux(lp);
    	while(lp){
    		((lp)-> energie) --;
    		Animal* temp = (lp)->suivant;
    		if ((lp-> energie)<0){
    			enlever_animal(liste_predateur, lp);
    		}
    		else{
    			Animal* amanger=animal_en_XY(*liste_proie, lp->x, lp->y);
    			if(amanger){
    				lp->energie+= amanger->energie;
    				enlever_animal(liste_proie, amanger);
    			}
    		}
    		lp = temp;
   	 	}
    	reproduce(liste_predateur, p_reproduce_predateur);
    }

}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
 	for(int i = 0; i<SIZE_X; i++){
 		for(int j = 0; j<SIZE_Y; j++){
 			monde[i][j] ++;
 		}
 	}

}

