/*
 * mobile.h
 *
 *  Created on: 6 avr. 2021
 *      Author: lucas
 */
typedef struct s_mobile t_mobile;

t_mobile* MobileNew(SDL_Rect*pFrame,SDL_Point*pSpeed,int iColor);
t_mobile* MobileDel(t_mobile*pMobile);
void*MobileDraw(t_mobile*pMobile,SDL_Renderer*pRenderer);
t_mobile*MobileMove(t_mobile*pMobile, SDL_Rect*pSceneArea);

size_t MobileGetStructSize(void);
