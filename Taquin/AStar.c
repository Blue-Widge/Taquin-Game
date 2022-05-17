#include "AStar.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "SDL.h"
#include "SDL_image.h"



// fonction pour cr�er (allouer) un noeud de liste et l'initialiser avec le taquin pass� en param�tre
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

// Insertion dans la liste de fa�on tri�e ou non
// si on passe le param�tre tri � 0, on ins�re en t�te de liste
int insertList(ptrListAStar * ppHead, ptrListAStar pNode, int tri)
{
	if (!ppHead || !(*ppHead) || !pNode)
		return -1;

	if (!tri)
	{
		pNode->m_nextlist = (*ppHead);
		(*ppHead) = pNode;
		return 1;
	}

	ptrListAStar temp = ppHead;
	while (temp->m_nextlist)
		temp = temp->m_nextlist;
	temp->m_nextlist = pNode;
	return 1;
}

// Fonction pour pr�lever le noeud en t�te de liste
// Retourne le noeud pr�lev�
ptrListAStar popList(ptrListAStar * ppHead)
{
	ptrListAStar temp = (*ppHead);
	(*ppHead) = temp->m_nextlist;
	return temp;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin pass� en param�tre (pointeur sur le pointeur dans la liste)
ptrListAStar * isInList(ptrListAStar * ppHead, Taquin * pTaquin)
{
	ptrListAStar temp = (*ppHead);
	while (temp && temp->m_taquin != pTaquin)
		temp = temp->m_nextlist;
	return temp;
}

// fonction pour afficher une liste
// si on met displayFGH � 0 les valeur de F, G et H ne sont pas affich�es
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

// Fonction pour r�soudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin sp�cifi� par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les d�placements � effectuer pour r�soudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est diff�rent de 0 on affiche dans la console toutes les �tapes de la r�solution
// pWindow
int solveTaquin(Taquin *pTaquin, deplacement ** pTabDeplacement, unsigned long *pNbDeplacements, unsigned long * pNbTaquinsGeneres, unsigned long * pTimeElapsed, int stepByStep, SDL_Surface * pWindow)
{

	return 1;
}

// fonction d'�valuation pour la r�solution avec AStar
int h(Taquin * pTaquin)
{
	return 0;
}