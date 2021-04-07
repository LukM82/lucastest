/*
 * app.c
 *
 *  Created on: 6 avr. 2021
 *      Author: lucas
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <container.h>

#include "mobile.h"
#include "app.h"


#define mBitsSet(f,m)		((f)|=(m))
#define mBitsClr(f,m)		((f)&=(~(m)))
#define mBitsTgl(f,m)		((f)^=(m))
#define mBitsMsk(f,m)		((f)&(m))
#define mIsBitsSet(f,m)		(((f)&(m))==(m))
#define mIsBitsClr(f,m)		(((~(f))&(m))==(m))

#define mRGBA_r(RGBA)		((Uint8)RGBA)
#define mRGBA_g(RGBA)		((Uint8)(RGBA>>8))
#define mRGBA_b(RGBA)		((Uint8)(RGBA>>16))
#define mRGBA_a(RGBA)		((Uint8)(RGBA>>24))

#define mToSDLcolor(c,RGBA)	(*(uint32_t*)&c=RGBA)

#define mRGBA(r,g,b,a)		(((uint32_t)(a)<<24)|\
							 ((uint32_t)(b)<<16)|\
							 ((uint32_t)(g)<<8)|\
							 ((uint32_t)(r)<<0))

typedef enum e_status{
	ST_ALL_CLEARED			= 0x00000000,
	ST_ALL_SETTED			= 0xFFFFFFFF,
	ST_APP_INIT_FAILED		= 0x80000000,
	ST_SDL_INITIATED		= 0x00000001,
	//ST_TTF_INITIATED  		= 0x00000002,
	ST_APP_PAUSED			= 0x00000002,

}t_status;

#define mTglPause()		mBitsTgl(pApp->uStatus, ST_APP_PAUSED)
#define mIsPaused()		mIsBitsSet(pApp->uStatus,ST_APP_PAUSED)

struct s_app {
	t_status			uStatus;
	SDL_Window		*	pWindow;
	SDL_Renderer	*	pRenderer;
	SDL_Color			colorBkgnd,
						colorText;
	int					iWidth,
						iHeight;

	//TTF_Font		*	pFont;

	SDL_Rect			rSceneArea;
	//t_mobile		*	pMobiles[APP_MOBILE_NB];
	size_t				szMobileSize;

	SDL_TimerID			timerID;
	SDL_Texture		*	pTexture;
	SDL_Surface		*	pSurface;
	t_container		*	pMobiles;

};

/****************************PRIVATE FUNCTION PROTOTYPES************************/
void _AppDraw(t_app*pApp);
Uint32	_AppAnimatingFunc(Uint32 interval, t_app*pApp);
/*******************************************************************************/

t_app* AppNew(
		char*titleStr,		//Window title
		int iWidth, 		//Window width
		int iHeight, 		//Window height
		int colorBkgnd		//window background color RGBA format
){
	srand((unsigned int) time(NULL));

	t_app*pApp=(t_app*)malloc(sizeof (t_app));

	pApp->uStatus=ST_ALL_CLEARED;

	pApp->pWindow=NULL;
	pApp->pRenderer=NULL;


/*iWidth & iHeight parameters are ignored!!!
 *
 */
	pApp->iWidth=APP_WINDOW_WIDTH;
	pApp->iHeight=APP_WINDOW_HEIGHT;
/*******************************************/

	if(SDL_Init(SDL_INIT_VIDEO)!=0){
		fprintf(stderr,"SDL_Init() failed; %s\n", SDL_GetError());
		mBitsSet(pApp->uStatus, ST_APP_INIT_FAILED);
		return pApp;
	}
	mBitsSet(pApp->uStatus, ST_SDL_INITIATED);

	pApp->pWindow = SDL_CreateWindow(
			titleStr,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			pApp->iWidth,
			pApp->iHeight,
			SDL_WINDOW_SHOWN
		);

	if(pApp->pWindow==NULL){
		fprintf(stderr,"SDL_CreatWindow() failed: %s\n", SDL_GetError());
		mBitsSet(pApp->uStatus, ST_APP_INIT_FAILED);
		return pApp;
	}

	pApp->pRenderer=SDL_CreateRenderer(pApp->pWindow,-1, SDL_RENDERER_ACCELERATED);
	if(pApp->pRenderer==NULL){
		fprintf(stderr,"SDL_CreateRenderer ACCELERATED failed: %s\n", SDL_GetError());
		pApp->pRenderer=SDL_CreateRenderer(pApp->pWindow,-1, SDL_RENDERER_SOFTWARE);
		if(pApp->pRenderer==NULL){
			fprintf(stderr,"SDL_CreateRenderer SOFTWARE failed: %s\n", SDL_GetError());
			//SDL_DestroyWindow(pApp->pWindow);
			mBitsSet(pApp->uStatus, ST_APP_INIT_FAILED);
			return pApp;
		}

	}


	pApp->rSceneArea = (SDL_Rect){
		.y= APP_SCENE_PADDING_TOP,
		.x=	APP_SCENE_PADDING_HZR,
		.w= pApp->iWidth-(APP_SCENE_PADDING_HZR*2),
		.h=	pApp->iHeight-(APP_SCENE_PADDING_TOP+APP_SCENE_PADDING_BOT)
		};

	//pApp->szMobileSize=MobileGetStructSize();

	//pApp->pMobiles=(t_mobile*)malloc(APP_MOBILE_NB*pApp->szMobileSize);

//	for(int k=0; k<APP_MOBILE_NB; k++){
	pApp->pMobiles=ContainerNew((t_ptfV)MobileDel);

	for(int k=0;k<APP_MOBILE_NB;k++){
		ContainerPushback(
			pApp->pMobiles,
			MobileNew(
				&(SDL_Rect){//Frame						//Literal fashion...
					.x=	APP_SCENE_PADDING_HZR+rand()%(APP_WINDOW_WIDTH-APP_MOBILE_SIZE),
					.y= APP_SCENE_PADDING_TOP+rand()%(APP_WINDOW_HEIGHT-APP_MOBILE_SIZE),
					.w= APP_MOBILE_SIZE,
					.h=	APP_MOBILE_SIZE,
				},
				&(SDL_Point){//Speed					//Literal fashion...
					.x=rand()%11-5,
					.y=rand()%11-5
				},
				mRGBA(//Color
					rand()%156+100,	//=> [100;255]
					rand()%156+100,	//=> [100;255]
					rand()%156+100,	//=> [100;255]
					255))
		);
	}

//	pApp->pSurface =TTF_RenderText_Blended(pApp->pFont

	/* Set timer callback running-----------------------------------------------------------------*/
	pApp->timerID=SDL_AddTimer(APP_MOBILE_ANIMATING_RATE, (SDL_TimerCallback)_AppAnimatingFunc, pApp);

	return pApp;

}

t_app* AppDel(t_app*pApp){

//	SDL_FreeSurface(pApp->Surface);

	/* Set timer callback running-----------------------------------------------------------------*/
	SDL_RemoveTimer(pApp->timerID);

	pApp->pMobiles=ContainerDel(pApp->pMobiles);



//	free(pApp->pMobiles);
//	for(int k=0; k<APP_MOBILE_NB; k++){
//		MobileDel(pApp->pMobiles[k]);
//	}

	if(pApp->pRenderer){
		SDL_DestroyRenderer(pApp->pRenderer);
	}

	if(pApp->pWindow){
		SDL_DestroyWindow(pApp->pWindow);
	}

	if(mIsBitsSet(pApp->uStatus, ST_SDL_INITIATED)){
		SDL_Quit();
	}
	free(pApp);

	return NULL;
}


int AppRun(t_app*pApp){
	int quit = 0;
	SDL_Event event;

	_AppDraw(pApp);

	while(!quit){
		while(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_QUIT:	//Window quit events
				quit=1;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:

					break;
				case SDLK_p:
					mTglPause();
					break;
				case SDLK_ESCAPE:
					event.type=SDL_QUIT;	//Make a Quit event
					SDL_PushEvent(&event);	//Post the event
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}
	return 0;

}


Uint32	_AppAnimatingFunc(Uint32 interval, t_app*pApp){
	if(!mIsPaused){
//	for(int k=0; k<APP_MOBILE_NB; k++){
//		MobileMove(pApp->pMobiles[k], &pApp->rSceneArea);
//	}
		ContainerParse(pApp->pMobiles, (t_ptfVV)MobileMove , &pApp->rSceneArea);
		_AppDraw(pApp);
	}
	return interval;
}

void _AppDraw(t_app*pApp){

	SDL_SetRenderDrawColor(
			pApp->pRenderer,
			pApp->colorBkgnd.r,
			pApp->colorBkgnd.g,
			pApp->colorBkgnd.b,
			pApp->colorBkgnd.a
	);

	SDL_RenderClear(pApp->pRenderer);

	SDL_SetRenderDrawColor(pApp->pRenderer, 0, 0, 255, 255);
	SDL_RenderDrawRect(pApp->pRenderer, &pApp->rSceneArea);

	ContainerParse(pApp->pMobiles,(t_ptfVV)MobileDraw, pApp->pRenderer);

	//for(int k=0; k<APP_MOBILE_NB; k++)MobileDraw(pApp->pMobiles[k],pApp->pRenderer);

	SDL_RenderPresent(pApp->pRenderer);

}

