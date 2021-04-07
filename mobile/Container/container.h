/*
 * container.h
 *
 *  Created on: 6 avr. 2021
 *      Author: lucas
 */

typedef void*(*t_ptfV)(void*);
typedef void*(*t_ptfVV)(void*,void*);

typedef struct s_container t_container;

t_container*ContainerNew(t_ptfV pDeleteFunc);
t_container*ContainerDel(t_container*pContainer);

unsigned int ContainerCard(t_container*pContainer);

void*ContainerPushback(t_container*pContainer,void*pElem);
void*ContainerGetback(t_container*pContainer);

void*ContainerParse(t_container*pContainer, t_ptfVV pParseFunc, void*pParam);
