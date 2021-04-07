/*
 ============================================================================
 Name        : mobile.c
 Author      : lucas
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#include <stdint-gcc.h>
#include "app.h"

#define mRGBA(r,g,b,a)		(((uint32_t)a<<24)|\
							 ((uint32_t)b<<16)|\
							 ((uint32_t)g<<8)|\
							 ((uint32_t)r<<0))

int main(void){

	int res;

	printf("==================ENTER APP========================\n");

//	t_app*pApp = (int*)malloc(1100 * 900 * sizeof (int));

	t_app*pApp = AppNew("Mine Sweeper", 800, 500, mRGBA(0,0,100,255));

	if((res=AppRun(pApp))==EXIT_FAILURE){
		fprintf(stderr,"App Init Failed - Aborting App!\n");
	}

	pApp=AppDel(pApp);

	printf("==================EXIT APP=========================\n");

	return res;
}
