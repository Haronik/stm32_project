#include <dinoGame_defines.h>
#include "dinoGame.h"
#include "stdlib.h"

/*   Normal state  */
static slide norm_slide1;
static slide norm_slide2;
static slide *curSlideNormal;

/*   Sit state   */
static slide sit_slide1;
static slide sit_slide2;
static slide *curSlideSit;

/*   Jump state   */
static slide jump_slide;

/*****  BIRD   *****/
static slide bird_slide1;
static slide bird_slide2;
static slide *curSlideBird;

/*     CACTUS     */
static slide cactus1_slide;  // three different cactuses
static slide cactus2_slide;
static slide cactus3_slide;

/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\ */

uint8_t gameState = 1;
uint16_t points = 0;
struct dino player;

object objList[20];           // list for different barriers like cactus or bird
uint8_t slist[20] = {0};


/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\ */

void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoaK(int n, char *s)
 {
     int i, sign;

     if ((sign = n) < 0)
         n = -n;
     i = 0;
     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

void initSlides()
{
	// Normal state
	norm_slide1.pic = dino1;
	norm_slide1.width = 20;
	norm_slide1.height = 23;
	norm_slide1.next = &norm_slide2;

	norm_slide2.pic = dino2;
	norm_slide2.width = 20;
	norm_slide2.height = 23;
	norm_slide2.next = &norm_slide1;

	curSlideNormal = &norm_slide1;

	// Sit state
	sit_slide1.pic = dinosit1;
	sit_slide1.width = 28;
	sit_slide1.height = 13;
	sit_slide1.next = &sit_slide2;


	sit_slide2.pic = dinosit2;
	sit_slide2.width = 27;
	sit_slide2.height = 13;
	sit_slide2.next = &sit_slide1;

	curSlideSit = &sit_slide1;

	// Jump state
	jump_slide.pic =dino;
	jump_slide.width = 20;
	jump_slide.height = 23;
	jump_slide.next = NULL;

	// Bird
	bird_slide1.pic = bird1;
	bird_slide1.width = 21;
	bird_slide1.height = 15;
	bird_slide1.next = &bird_slide2;

	bird_slide2.pic = bird2;
	bird_slide2.width = 21;
	bird_slide2.height = 13;
	bird_slide2.next = &bird_slide1;

	curSlideBird = &bird_slide1;
	// Cactus
	cactus1_slide.pic = cactus1;
	cactus1_slide.width = 6;
	cactus1_slide.height = 11;
	cactus1_slide.next = NULL;

	cactus2_slide.pic = cactus2;
	cactus2_slide.width = 6;
	cactus2_slide.height = 11;
	cactus2_slide.next = NULL;

	cactus3_slide.pic = cactus3;
	cactus3_slide.width = 12;
	cactus3_slide.height = 22;
	cactus3_slide.next = NULL;
}


void initDino()
{
	player.pos_x = 5;
	player.pos_y = ssd1306_GetHeight()-curSlideNormal->height;
	player.state = NORMAL;
}

void initGame()
{
	initSlides();
	initDino();
}

void drawRoad()
{
	static uint8_t pos = 0;

	ssd1306_DrawBitmap(0,64-6,128,5, &road[pos]);

	pos+=2;
	if (pos >= 128)
		pos = 1;
}


void createEntity(enum type type)
{
	uint8_t iter = 0;
	while(slist[iter])
	{
		iter++;
	}
	slist[iter] = 1;
	switch (type)
	{
	case CACTUS:
		objList[iter].ticks=0;
		objList[iter].slides = &cactus3_slide;
		objList[iter].pos_x = ssd1306_GetWidth()-cactus3_slide.width-1;
		objList[iter].pos_y = cactus3_slide.height;
		break;
	case BIRD:
		objList[iter].ticks=0;
		objList[iter].slides = &bird_slide1;
		objList[iter].pos_x = ssd1306_GetWidth()-bird_slide1.width;
		objList[iter].pos_y = ssd1306_GetHeight()-bird_slide1.height-3;
		break;
	}


}

void drawEntities()
{
	uint8_t iter = 0;
	while(iter < 20)
	{
		if (slist[iter])
		{
			drawEntity(&objList[iter], 1);
		}
		iter++;
	}
}


void drawEntity(object *entity, uint8_t scale)
{
	if (entity != NULL)
	{
		if ( entity->ticks >= scale)
		{
			entity->ticks = 0;
			if (entity->slides->next != NULL)
				entity->slides = entity->slides->next;
		}
		else {
			entity->ticks++;
		}
		ssd1306_DrawBitmap(entity->pos_x,ssd1306_GetHeight()- entity->pos_y,
						entity->slides->width, entity->slides->height, entity->slides->pic);
	}

}


/* have problems with malloc : always rerurns NULL */
/*
void createEntity(enum type type)
{
	objects *last;
	if (listObj == NULL)
	{
		listObj = malloc(sizeof(objects));
		last = listObj;
	}
	else {
		last = listObj;
		while(last->next)
			last = last->next;
	}

	if (last)
	{
		return;
	}
	switch(type)
	{
	case CACTUS:
		//last->entity.pos_x = ssd1306_GetWidth()-cactus1_slide.width;
		last->entity.pos_x = 0;
		//last->entity.pos_y = ssd1306_GetHeight()-cactus1_slide.height-3;
		//last->entity.slides = &cactus1_slide;
		//last->entity.ticks = 0;
		break;

	case BIRD:
		break;
	}
}
*/


void drawDino(uint8_t run_sc, uint8_t jump_sc, uint8_t fall_sc)
{
	const uint8_t jumpSize = 30;
	static uint8_t tmp = 0;
	tmp++;

	switch (player.state)
	{
	case NORMAL:
		if (tmp >= run_sc)
		{
			curSlideNormal = curSlideNormal->next;
			tmp = 0;
		}
		ssd1306_DrawBitmap(player.pos_x,player.pos_y,curSlideNormal->width, curSlideNormal->height, curSlideNormal->pic);

		break;
	case SIT:
		// I don't have 2 buttons ((
		break;
	case JUMP:
		if (tmp >= jump_sc)
		{
			if (player.pos_y <= ssd1306_GetHeight() - jumpSize-curSlideNormal->height)
				player.state = FALL;
			else
				player.pos_y-= JUMP_SPEED;
		}
		ssd1306_DrawBitmap(player.pos_x,player.pos_y,jump_slide.width, jump_slide.height, jump_slide.pic);

		break;
	case FALL:
		if (tmp >= fall_sc)
		{
			player.pos_y+= FALL_SPEED;
			if (player.pos_y >= ssd1306_GetHeight()-curSlideNormal->height)
			{
				player.pos_y = ssd1306_GetHeight()-curSlideNormal->height;        // if FALL_SPEED != JUMP_SPEED
				player.state = NORMAL;
			}
		}
		ssd1306_DrawBitmap(player.pos_x,player.pos_y,jump_slide.width, jump_slide.height, jump_slide.pic);
		break;
	}

}

void normalizeField()
{
	static uint8_t t= 0;
	static uint8_t typek;

	if (t > SPAWN_SPEED_BASE - points/XSPEED_SPAWN)
	{
		typek++;
		if (typek <= 2)
		{
			createEntity(CACTUS);
		}
		else
		{
			createEntity(BIRD);
			typek = 0;
		}
		t= 0;
	}
	t++;
}

// draws score
void drawPoints()
{
	char buff[8] = {0};
	itoaK(points, buff);
	ssd1306_SetCursor(ssd1306_GetWidth()-30, 0);
	ssd1306_WriteString(buff, Font_7x10);
}

void draw()
{
	normalizeField(); //  spawns barriers
	ssd1306_Clear();
	drawEntities();
	processEntities(BASE_SPEED+points/XSPEED_BY_TIME);  // speed depends on points(time playing)
	drawPoints();

	drawRoad();
	drawDino(2,1,1);
	ssd1306_UpdateScreen();

}


void processEntities(uint8_t speed) // 10 is fast already
{
	uint8_t iter = 0;
	while(iter < 20)
	{
		if (slist[iter])
		{
			objList[iter].pos_x-=speed;

			if (objList[iter].pos_x <= speed)
				slist[iter] = 0;

			if (ssd1306_TestForPixels(player.pos_x, player.pos_y, 20, 23, dinoTest))
				end();

		}
		iter++;
	}
}

void end()
{
	gameState = 0;
	ssd1306_DrawBitmap(10,10, 78, 9, endGame);
}

void SetJumpState()
{
	player.state = JUMP;

}

uint8_t getState()    // bad usage
{
	if (!gameState)
		return 2;
	if (player.state == NORMAL)
		return 1;
	return 0;
}
void startGame()
{
	points = 0;
	for (uint8_t i = 0; i < 20; i++)
		slist[i] = 0;
	gameState = 1;
}
