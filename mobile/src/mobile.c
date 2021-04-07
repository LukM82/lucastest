/*
 * mobile.c
 *
 *  Created on: 6 avr. 2021
 *      Author: lucas
 */

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "mobile.h"

struct s_mobile{
	SDL_Rect	frame;
	SDL_Point	speed;
	SDL_Color	color;
};

t_mobile* MobileNew(SDL_Rect*pFrame,SDL_Point*pSpeed,int iColor){
	t_mobile*pMobile=(t_mobile*)malloc(sizeof (t_mobile));
	pMobile->frame=*pFrame; 	//frame reçoit le contenu pointé par le pointeur *pFrame
	pMobile->speed=*pSpeed;		//speed reçoit le contenu pointé par le pointeur *pSpeed
	pMobile->color=*(SDL_Color*)(&iColor);
	return pMobile;
}

t_mobile* MobileDel(t_mobile*pMobile){
		free(pMobile);
		return NULL;
}

void*MobileDraw(t_mobile*pMobile,SDL_Renderer*pRenderer){

	SDL_SetRenderDrawColor(pRenderer, pMobile->color.r, pMobile->color.g, pMobile->color.b, pMobile->color.a);
	SDL_RenderFillRect(pRenderer, &pMobile->frame);
	return NULL;
}
t_mobile*MobileMove(t_mobile*pMobile,SDL_Rect*pSceneArea){

	pMobile->frame.x+=pMobile->speed.x;
	pMobile->frame.y+=pMobile->speed.y;

	if(pMobile->frame.x<=pSceneArea->x){
		pMobile->frame.x=pSceneArea->x+1;
		pMobile->speed.x*=-1;
	}

	if(pMobile->frame.x>(pSceneArea->w+pSceneArea->x-pMobile->frame.w)){
		pMobile->frame.x=pSceneArea->w+pSceneArea->x-pMobile->frame.w-1;
		pMobile->speed.x*=-1;
	}

	if(pMobile->frame.y<=pSceneArea->y){
		pMobile->frame.y=pSceneArea->y+1;
		pMobile->speed.y*=-1;
	}

	if(pMobile->frame.y>(pSceneArea->h+pSceneArea->y-pMobile->frame.h)){
		pMobile->frame.y=pSceneArea->h+pSceneArea->y-pMobile->frame.h-1;
		pMobile->speed.y*=-1;
	}

//	if((pMobile->frame.x + pMobile->speed.x + pMobile->frame.w) > (pSceneArea->w + pSceneArea->x)) pMobile->speed.x=-pMobile->speed.x;
//	if((pMobile->frame.x + pMobile->speed.x) < (pSceneArea->x)) pMobile->speed.x=-pMobile->speed.x;
//	if((pMobile->frame.y + pMobile->speed.y + pMobile->frame.h) > (pSceneArea->h + pSceneArea->y)) pMobile->speed.y=-pMobile->speed.y;
//	if((pMobile->frame.y + pMobile->speed.y) < (pSceneArea->y)) pMobile->speed.y=-pMobile->speed.y;

	return NULL;
}

size_t MobileGetStructSize(void){return sizeof(t_mobile);}
