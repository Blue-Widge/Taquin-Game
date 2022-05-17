// Inclusion des bibliothèques utiles
#include "taquin.h"
#include "AStar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>



// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * pSrc, Taquin * pDest)
{
	// TODO: copyTaquin*
	if (!pDest)
		return -1;

	createTaquin(pDest, pSrc->hauteur, pSrc->largeur);

	return pDest ? 1 : 0;
}

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des paramètres
int equalTaquin(Taquin * pTaquin1, Taquin * pTaquin2)
{
	// TODO: equalTaquin
	if (!pTaquin1 || !pTaquin2)
		return -1;

	if (pTaquin1->hauteur != pTaquin2->hauteur ||
		pTaquin1->largeur != pTaquin2->largeur ||
		pTaquin1->plateau != pTaquin2->plateau ||
		pTaquin1->x != pTaquin2->x			   ||
		pTaquin1->y != pTaquin2->y)
	{	return 0;	}

	return 1;
}

// Fonction qui crée un plateau de taquin 
// ATTENTION le plateau doit être NULL avant l'appel à cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de libérer la zone mémoire occupée par le plateau et cela peut donc aboutir à un crash si le plateau n'a pas été initialisé
int createTaquin(Taquin * pTaquin, unsigned char hauteur, unsigned char largeur)
{
	// Test pour vérifier que les données passées ne sont pas corrompues
	if(!pTaquin) return 0;
	
	// Tests pour vérifier les paramètres de taille de taquin
	if(hauteur<SIZE_MINI) hauteur = SIZE_MINI;
	if(hauteur>SIZE_MAXI) hauteur = SIZE_MAXI;
	if(largeur<SIZE_MINI) largeur = SIZE_MINI;
	if(largeur>SIZE_MAXI) largeur = SIZE_MAXI;
	
	{
		int i;

		// On vérifie que le plateau n'existe pas
		// S'il existe on libère la mémoire avant de recréer le plateau
		if(pTaquin->plateau)
		{
			// On libère le plateau ligne par ligne
			for(i=0; i < pTaquin->hauteur; i++) if(pTaquin->plateau[i]) free(pTaquin->plateau[i]);
			// On libère le tableau qui stockait les lignes
			free(pTaquin->plateau);
			pTaquin->plateau = NULL;
		}

		pTaquin->hauteur = hauteur;
		pTaquin->largeur = largeur;

		// on alloue la zone mémoire pour stocker les adresses des lignes du tableau
		pTaquin->plateau = (unsigned char**) malloc(sizeof(unsigned char*)*hauteur);
		
		// si on a pas réussi à allouer la zone mémoire on retourne 0
		if(!pTaquin->plateau) return 0;

		for(i=0; i < hauteur; i++) pTaquin->plateau[i] = NULL;

		for(i=0; i < hauteur; i++)
		{
			// On alloue la zone mémoire pour contenir la ligne i
			pTaquin->plateau[i] = (unsigned char*) malloc(sizeof(unsigned char)*largeur);
			// S'il y a eu un souci à l'allocation on libère tout ce qui a déjàà été alloué et on retourne 0
			if(!pTaquin->plateau[i])
			{
				freeTaquin(pTaquin);
				return 0;
			}
		}
		// On initialise le taquin
		if(!initTaquin(pTaquin)) return 0;
	}

	return 1;
}


// Fonction qui initialise le taquin
int initTaquin(Taquin * pTaquin)
{
	// TODO: initTaquin
	if (!pTaquin)
		return -1;

	int hauteur = pTaquin->hauteur;
	int largeur = pTaquin->largeur;
	for (int i = 0; i < hauteur; ++i)
		for (int j = 0; j < largeur; ++j)
			pTaquin->plateau[i][j] = i * largeur + j;
	return 1;
}

// Fonction qui mélange le taquin en effectuant entre minRandom et maxRandom coups aléatoires
int mixTaquin(Taquin * pTaquin, int minRandom, int maxRandom)
{
	// TODO: mixTaquin

	if (!pTaquin)
		return -1;

	int largeur = pTaquin->largeur;
	int hauteur = pTaquin->hauteur;

	deplacement possibleMove = AUCUN;

	int nbCoups = rand() % (maxRandom - minRandom + 1) + minRandom;

	while (nbCoups)
	{
		possibleMove = rand() % 5;
		if (possibleMove == AUCUN)
		{
			nbCoups--;
			continue;
		}
		if (possibleMove == GAUCHE)
		{
			if (pTaquin->x > 0)
			{
				moveTaquin(pTaquin, possibleMove);
				nbCoups--;
				continue;
			}
			possibleMove = (possibleMove + 1) % 5;
			continue;
		}
		if (possibleMove == DROITE)
		{
			if (pTaquin->x < largeur - 1)
			{
				moveTaquin(pTaquin, possibleMove);
				continue;
			}
			possibleMove = (possibleMove + 1) % 5;
			continue;
		}
		if (possibleMove == HAUT)
		{
			if (pTaquin->y > 0)
			{
				moveTaquin(pTaquin, possibleMove);
				continue;
			}
			possibleMove = (possibleMove + 1) % 5;
			continue;
		}
		if (possibleMove == BAS)
		{
			if (pTaquin->y < hauteur - 1)
			{
				moveTaquin(pTaquin, possibleMove);
				continue;
			}
			possibleMove = (possibleMove + 1) % 5;
			continue;
		}
	}

	return 1;
}

// Fonction qui permet de bouger une pièce du taquin (en bougeant la case vide)
int moveTaquin(Taquin * pTaquin, deplacement d)
{
	// TODO: moveTaquin
	if (!pTaquin)
		return -1;

	if (!d)
		return 1;
	int x = pTaquin->x;
	int y = pTaquin->y;

	if (d == GAUCHE)
	{
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x - 1][y];
		pTaquin->plateau[x - 1][y] ^= pTaquin->plateau[x][y];
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x - 1][y];
		pTaquin->x = x - 1;
		return 1;
	}
	if (d == HAUT)
	{
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x][y - 1];
		pTaquin->plateau[x][y - 1] ^= pTaquin->plateau[x][y];
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x][y - 1];
		pTaquin->y = y - 1;
		return 1;
	}
	if (d == DROITE)
	{
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x + 1][y];
		pTaquin->plateau[x + 1][y] ^= pTaquin->plateau[x][y];
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x + 1][y];
		pTaquin->x = x + 1;
		return 1;
	}
	if (d == BAS)
	{
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x][y + 1];
		pTaquin->plateau[x][y + 1] ^= pTaquin->plateau[x][y];
		pTaquin->plateau[x][y] ^= pTaquin->plateau[x][y + 1];
		pTaquin->y = y - 1;
		return 1;
	}

	return 0;
}

// Fonction qui renvoie le déplacement à effectuer pour annuler le déplacement donné en paramètre
deplacement cancelMove(deplacement d)
{
	// TODO: cancelMove
	return (d == AUCUN) ? AUCUN : (d == HAUT) ? BAS : (d == BAS) ? HAUT : (d == DROITE) ? GAUCHE : DROITE;
}

// Fonction qui renvoie 1 si le taquin es résolu, 0 sinon
int endTaquin(Taquin * pTaquin)
{
	// TODO: endTaquin
	if (!pTaquin)
		return -1;
	int hauteur = pTaquin->hauteur;
	int largeur = pTaquin->largeur;
	unsigned char** plateau = pTaquin->plateau;
	for (int i = 0; i < hauteur; ++i)
	{
		for (int j = 0; j < largeur; ++j)
		{
			if (plateau[i][j] != (i * largeur + j))
				return 0;
		}
	}
	return 1;
}

// fonction d'affichage du taquin
int displayTaquin(Taquin * pTaquin, int offset)
{
	// TODO: displayTaquin
	if (!pTaquin || !(pTaquin->plateau))
		return -1;
	int hauteur = pTaquin->hauteur;
	int largeur = pTaquin->largeur;
	unsigned char** plateau = pTaquin->plateau;
	for (int i = 0; i < hauteur; ++i)
	{
		for (int j = 0; j < largeur; ++j)
		{
			printf("%hd ", plateau[i][j], offset);
		}
		printf("\n");
	}
	return 1;
}

// Fonction pour libérer les zones mémoires occupées par un taquin
int freeTaquin(Taquin * pTaquin)
{
	// TODO: freeTaquin
	if (!pTaquin)
		return -1;
	int hauteur = pTaquin->hauteur;
	for (int i = 0; i < hauteur; ++i)
	{
		free(pTaquin->plateau[i]);
		pTaquin->plateau = NULL;
	}
	free(pTaquin);
	pTaquin = NULL;
	return 1;
}
// Boucle de jeu 
int gameLoop(int hauteur,int largeur,int minRandom, int maxRandom)
{
	int end = 0;
	Taquin t;
	t.plateau = NULL;
	createTaquin(&t, hauteur, largeur);
	t.x = t.y = 0;

	srand((unsigned)time(NULL));
	mixTaquin(&t, minRandom, maxRandom);

	displayTaquin(&t, 3);
	// BOUCLE DE JEU ! A DEVELOPPER
	while(!_kbhit())
	{
		//printf(".");
	}
	return 1;
}

