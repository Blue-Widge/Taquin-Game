#include "AStar.h"
#include <windows.h>
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
	list->m_poidsMoves = fValue;
	list->m_lastMove = d;
	list->m_taquin = pTaquin;
	list->m_lastStep = pPrevPlay;
	return list;
}

// Insertion dans la liste de façon triée ou non
// si on passe le paramètre tri à 0, on insère en tête de liste
int insertList(ptrListAStar * ppHead, ptrListAStar pNode, int tri)
{
	if (!ppHead || !pNode)
		return 0;

	if (!(*ppHead))
	{
		(*ppHead) = pNode;
		return 1;
	}

	if (!tri)
	{
		pNode->m_nextlist = (*ppHead);
		(*ppHead) = pNode;
		return 1;
	}
	ptrListAStar temp = (*ppHead);

	while (temp->m_nextlist && temp->m_nextlist->m_poidsMoves <= pNode->m_poidsMoves)
		temp = temp->m_nextlist;

	pNode->m_nextlist = temp->m_nextlist;
	temp->m_nextlist = pNode;
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
	temp->m_nextlist = NULL;
	return temp;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin passé en paramètre (pointeur sur le pointeur dans la liste)
ptrListAStar* isInList(ptrListAStar * ppHead, Taquin * pTaquin)
{
	if (!ppHead || !(*ppHead))
		return NULL;

	ptrListAStar* cursor = ppHead;

	while ((*cursor) && !equalTaquin((*cursor)->m_taquin, pTaquin))
		cursor = &((*cursor)->m_nextlist);
	return (*cursor) ? cursor : NULL;
}

// fonction pour afficher une liste
// si on met displayFGH à 0 les valeur de F, G et H ne sont pas affichées
int displayList(ptrListAStar pHead, int displayFGH)
{
	ptrListAStar temp = pHead;
	while (temp)
	{
		displayTaquin(temp->m_taquin, 3);
		(displayFGH) * printf("F : %d; G : %d; H : %d;\n\n", pHead->m_poidsMoves + pHead->m_parcouru, pHead->m_poidsMoves, pHead->m_parcouru);
		temp = temp->m_nextlist;
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
	ptrListAStar current = NULL;
	ptrListAStar childrenNode = NULL;
	Taquin* childrenTaquin = NULL;
	unsigned long startTime = SDL_GetTicks();
	while (openList)
	{
		current = popList(&openList);
		for (int i = 1; i < 5; ++i)
		{
			childrenTaquin = (Taquin*) calloc(1, sizeof(Taquin));
			copyTaquin(current->m_taquin, childrenTaquin);
			(*pNbTaquinsGeneres)++;

			if (!moveTaquin(childrenTaquin, i))
			{
				freeTaquin(childrenTaquin);
				free(childrenTaquin);
				continue;
			}

			childrenNode = createNodeList(childrenTaquin, current->m_parcouru + 1,
				current->m_parcouru + 1 + h(childrenTaquin), i, current);

			if (endTaquin(childrenTaquin))
			{
				//completed

				ptrListAStar cursor = childrenNode;
				while (cursor->m_lastStep)
				{
					(*pNbDeplacements)++;
					cursor = cursor->m_lastStep;
				}

				(*pTabDeplacement) = (deplacement*) calloc (*pNbDeplacements, sizeof(deplacement));
				cursor = childrenNode;
				for (int i = (*pNbDeplacements) - 1; i > -1 ; --i)
				{
					(*pTabDeplacement)[i] = cursor->m_lastMove;
					cursor = cursor->m_lastStep;
				}
				(*pTimeElapsed) = SDL_GetTicks() - startTime;
				freeList(childrenNode, pTaquin);
				freeList(current, pTaquin);
				freeList(openList, pTaquin);
				freeList(closedList, pTaquin);
				return 1;
			}
			if(isInList(&closedList, childrenTaquin) || (isInList(&openList, childrenTaquin)))
			{
				freeList(childrenNode, pTaquin);
				continue;
			}
			insertList(&openList, childrenNode, 1);
		}
		insertList(&closedList, current, 0);
	}
	printf("Couldn't find any solutions...\n");
	freeList(childrenNode, pTaquin);
	freeList(current, pTaquin);
	freeList(openList, pTaquin);
	freeList(closedList, pTaquin);
	return 0;
}

// fonction d'évaluation pour la résolution avec AStar
int h(Taquin * pTaquin)
{
	int distance = -1;
	int hauteur = pTaquin->hauteur;
	int largeur = pTaquin->largeur;

	for (int i = 0; i < hauteur; ++i)
	{
		for (int j = 0; j < largeur; ++j)
		{
			distance += abs((pTaquin->plateau[i][j] / largeur) - i) + abs((pTaquin->plateau[i][j] % largeur) - j);
		}
	}
	if (hauteur == 2)
		return distance;

	int bottom = hauteur - 1;
	for (int i = 0; i < largeur; ++i)
	{
		if (pTaquin->plateau[bottom][i] != bottom / hauteur + i)
			return distance;
	}
	distance -= 20;
	if (hauteur == 3)
		return distance;

	--bottom;

	for (int i = 0; i < largeur; ++i)
	{
		if (pTaquin->plateau[bottom][i] != bottom / hauteur + i)
			return distance;
	}
	return distance - 20;
}

void freeList(ptrListAStar p_list, Taquin* p_originel)
{
	if (!p_list)
		return;

	ptrListAStar cursor = NULL;
	while (cursor = popList(&p_list))
	{
		
		if (cursor->m_taquin != p_originel)
		{
			freeTaquin(cursor->m_taquin);
			free(cursor->m_taquin);
			cursor->m_taquin = NULL;
		}
		free(cursor);
	}
	p_list = NULL;
}


