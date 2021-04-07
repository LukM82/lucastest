/*
 * app.h
 *
 *  Created on: 6 avr. 2021
 *      Author: lucas
 */
#define APP_WINDOW_WIDTH		(500)

#define APP_WINDOW_HEIGHT		(500)

#define APP_SCENE_PADDING_TOP	(50)
#define APP_SCENE_PADDING_BOT	(15)
#define APP_SCENE_PADDING_HZR	(20)

#define APP_WINDOW_FONT_SIZE	(30)

#define APP_MOBILE_SIZE		(20)
#define APP_MOBILE_HEIGHT	(200)
#define APP_MOBILE_WIDTH	(100)
#define APP_MOBILE_NB		(10)

#define APP_MOBILE_ANIMATING_RATE	(10)

typedef struct s_app t_app;

t_app* AppNew(
		char*titleStr,		//Window title
		int iWidth, 		//Window width
		int iHeight, 		//Window height
		int colorBkgnd		//window background color RGBA format
);

t_app* AppDel(t_app*pApp);
int AppRun(t_app*pApp);
