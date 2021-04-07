/*
 * container.c
 *
 *  Created on: 6 avr. 2021
 *      Author: lucas
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "container.h"

/*------------------------Node Component Section--------------------------------------*/

typedef struct tt_node{
	struct tt_node*pPrev;
	struct tt_node*pNext;
	void*pElem;
}t_node;

t_node*NodeNew(t_node*pPrev, t_node*pNext,void*pElem){
	t_node*pNewNode=(t_node*)malloc(sizeof(t_node));

	pNewNode->pElem=pElem;
	pNewNode->pNext=pNext;
	pNewNode->pPrev=pPrev;

	if(pNext) pNext->pPrev=pNewNode;
	if(pPrev) pPrev->pNext=pNewNode;

	return pNewNode;
}

t_node*NodeDel(t_node*pNode,t_ptfV pDeleteFunc){

	t_node*pNext=pNode->pNext;

	if(pDeleteFunc) pDeleteFunc(pNode->pElem);
	else{
		free(pNode->pElem);
	}

	if(pNode->pPrev) pNode->pPrev->pNext=pNode->pNext;
	if(pNode->pNext) pNode->pNext->pPrev=pNode->pPrev;

	free(pNode);

	return pNext;	//return the next deleted node!
}

/*------------------------Container Component Section--------------------------------------*/
struct s_container{
	t_node*pHead;
	t_node*pTail;
	unsigned int uCard;
	t_ptfV pDeleteFunc;

};

t_container*ContainerNew(t_ptfV pDeleteFunc){
	t_container*pContainer=(t_container*)malloc(sizeof(t_container));

	pContainer->pDeleteFunc=pDeleteFunc;
	pContainer->pHead=pContainer->pTail=NULL;
	pContainer->uCard=0;

	return pContainer;
}

t_container*ContainerDel(t_container*pContainer){

	while(pContainer->pHead!=NULL){
		pContainer->pHead=NodeDel(pContainer->pHead, pContainer->pDeleteFunc);
		pContainer->uCard--;
	}

	assert(pContainer->uCard==0);

	free(pContainer);
	return NULL;
}

unsigned int ContainerCard(t_container*pContainer){return pContainer->uCard;}

void*ContainerPushback(t_container*pContainer,void*pElem){
	if(pContainer->uCard==0){
		//Empty container...............
		assert(pContainer->pHead==NULL);
		assert(pContainer->pTail==NULL);

		//The new element is both the first and the last one...
		//... and it has neither previous nor next!!!
		pContainer->pHead=pContainer->pTail=NodeNew(NULL, NULL, pElem);
	}
	else{
		//Container not empty........
		//Only pTail is modified by pushback
		pContainer->pTail=NodeNew(pContainer->pTail, NULL, pElem);
	}
	pContainer->uCard++;
	assert(pContainer->pTail->pElem==pElem);
	return pContainer->pTail->pElem;
}
void*ContainerGetback(t_container*pContainer){return pContainer->pTail->pElem;}

void*ContainerParse(t_container*pContainer, t_ptfVV pParseFunc, void*pParam){

	t_node*pScan=pContainer->pHead;

	while(pScan!=NULL){
		if(pParseFunc(pScan->pElem, pParam)!=NULL) return pScan->pElem;
		pScan=pScan->pNext;
	}
	return NULL;
}



