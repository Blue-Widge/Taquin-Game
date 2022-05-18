#include "AStar.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "SDL.h"
#include "SDL_image.h"



// fonction pour créer (allouer) un noeud de liste et l'initialiser avec le taquin passé en paramètre
ptrListAStar createNodeList(Taquin * pTaquin, int gValue, int fValue, deplacement d, ptrListAStar pPrevPlay)
{
	ptrListAStar list = (ptrListAStar) calloc(1, sizeof(ListAStar));
	if (!list)
		return NULL;

	list->m_parcouru = gValue;
	list->m_distanceLeft = fValue;
	list->m_lastMove = d;
	list->m_taquin = pTaquin;
	list->m_lastStep = pPrevPlay;
	return NULL;
}

// Insertion dans la liste de façon triée ou non
// si on passe le paramètre tri à 0, on insère en tête de liste
int insertList(ptrListAStar * ppHead, ptrListAStar pNode, int tri)
{
	if (!ppHead || !pNode)
		return -1;

	if (!(*ppHead))
		(*ppHead) = pNode;

	if (!tri)
	{
		pNode->m_nextlist = (*ppHead);
		(*ppHead) = pNode;
		return 1;
	}

	ptrListAStar temp = (*ppHead), temp2;
	while (temp->m_nextlist && temp->m_nextlist->m_distanceLeft < pNode->m_distanceLeft)
		temp = temp->m_nextlist;
	temp2 = temp->m_nextlist;
	temp->m_nextlist = pNode;
	pNode->m_nextlist = temp2;
	return 1;
}

// Fonction pour prélever le noeud en tête de liste
// Retourne le noeud prélevé
ptrListAStar popList(ptrListAStar * ppHead)
{
	if (!ppHead || !(*ppHead))
		return NULL;

	ptrListAStar temp = (*ppHead);
	(*ppHead) = temp->m_nextlist;
	return temp;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin passé en paramètre (pointeur sur le pointeur dans la liste)
ptrListAStar * isInList(ptrListAStar * ppHead, Taquin * pTaquin)
{
	ptrListAStar temp = (*ppHead);
	while (temp && temp->m_taquin != pTaquin)
		temp = temp->m_nextlist;
	return &temp;
}

// fonction pour afficher une liste
// si on met displayFGH à 0 les valeur de F, G et H ne sont pas affichées
int displayList(ptrListAStar pHead, int displayFGH)
{
	ptrListAStar temp = pHead;
	while (temp)
	{
		displayTaquin(temp->m_taquin, 3);
		(displayFGH == 1) ? printf("F : %d; G : %d; H : %d;\n\n", pHead->m_distanceLeft + pHead->m_parcouru, pHead->m_distanceLeft, pHead->m_parcouru) : printf("\n\n");
	}
	return 1;
}

// Fonction pour résoudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin spécifié par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les déplacements à effectuer pour résoudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est différent de 0 on affiche dans la console toutes les étapes de la résolution
// pWindow
int solveTaquin(Taquin *pTaquin, deplacement ** pTabDeplacement, unsigned long *pNbDeplacements, unsigned long * pNbTaquinsGeneres, unsigned long * pTimeElapsed, int stepByStep, SDL_Surface * pWindow)
{
	ptrListAStar openList = createNodeList(pTaquin, 0, h(pTaquin), AUCUN, NULL);
	ptrListAStar closedList = NULL;
	while (openList)
	{
		ptrListAStar minNode = openList->m_taquin;
		popList(&minNode);
		for (int i = 1; i < 5; ++i)
		{
			ptrListAStar newNode;
			copyTaquin(&minNode, &newNode);
			(*pNbTaquinsGeneres)++;
			moveTaquin(&newNode, i);
			if (equalTaquin(&minNode, &newNode))
			{
				popList(&newNode);
				(*pNbTaquinsGeneres)--;
			}
			insertList(&minNode, newNode, 1);
			if (endTaquin(newNode->m_taquin))
			{
				//completed
				if (stepByStep)
				{

					return 1;
				}

				return 1;
			}
			newNode->m_distanceLeft = h(&newNode);
			newNode->m_parcouru = minNode->m_parcouru + 1;
			ptrListAStar search = isInList(&openList, &newNode);
			if (search && (search->m_distanceLeft + search->m_parcouru) < (newNode->m_parcouru + newNode->m_distanceLeft))
			{
				popList(&newNode);
				continue;
			}
			search = isInList(&closedList, &newNode);
			if (search && (search->m_distanceLeft + search->m_parcouru) < (newNode->m_parcouru + newNode->m_distanceLeft))
			{
				insertList(&openList, newNode, 0);
			}
			insertList(&openList, minNode, 0);
		}
	}
	return 0;
}

// fonction d'évaluation pour la résolution avec AStar
int h(Taquin * pTaquin)
{
	int distance = 0;
	int hauteur = pTaquin->hauteur;
	int largeur = pTaquin->largeur;

	for (int i = 0; i < hauteur; ++i)
	{
		for (int j = 0; j < largeur; ++j)
		{
			distance += abs((pTaquin->plateau[i][j] / largeur + pTaquin->plateau[i][j] % largeur) - i - j);
		}
	}
	return distance;
}