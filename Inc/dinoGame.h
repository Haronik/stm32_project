#include "main.h"
#include "ssd1306.h"
#include "string.h"

enum type
{
	CACTUS,
	BIRD,
};

typedef enum
{
	NORMAL,
	SIT,
	JUMP,
	FALL,
} status;

typedef struct slide
{
	struct slide *next;
	const uint8_t *pic;
	uint8_t height;
	uint8_t width;
}slide;

struct dino
{
	int16_t pos_x;
	uint8_t pos_y;
	status state;
};

typedef struct
{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t ticks;
	slide *slides;
} object;

typedef struct Objects
{
	struct Objects *next;
	object entity;
} objects;


/*    MODELS        */
extern const uint8_t dino[];
extern const uint8_t dino1[];
extern const uint8_t dino2[];
extern const uint8_t bird1[];
extern const uint8_t bird2[];
extern const uint8_t cactus1[];
extern const uint8_t cactus2[];
extern const uint8_t cactus3[];
extern const uint8_t dinosit1[];
extern const uint8_t dinosit2[];
extern const uint8_t road[];
extern const uint8_t endGame[];
extern const uint8_t dinoTest[];
/*                     */



void drawRoad();
void drawDino(uint8_t run_sc, uint8_t jump_sc, uint8_t fall_sc);
void drawPoints();
void draw();
void initGame();
void SetJumpState();
void createEntity(enum type type);
void processEntities(uint8_t speed);
void drawEntity(object *entity, uint8_t scale);
void end();
void startGame();
uint8_t getState();


