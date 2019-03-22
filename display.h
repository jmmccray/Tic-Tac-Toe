#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "maplogic.h"
#include <ti/grlib/grlib.h>

/* palette */
#define BACKGROUNDCOLOR GRAPHICS_COLOR_PURPLE
#define LINECOLOR       GRAPHICS_COLOR_WHITE
#define FOREGROUNDCOLOR GRAPHICS_COLOR_CYAN
#define EMPHASISCOLOR   GRAPHICS_COLOR_YELLOW
#define ORANGE          GRAPHICS_COLOR_ORANGE
#define SILVER          GRAPHICS_COLOR_SILVER
#define PINK            GRAPHICS_COLOR_HOT_PINK
#define GREENYELLOW     GRAPHICS_COLOR_GREEN_YELLOW
#define PEACH           GRAPHICS_COLOR_PEACH_PUFF
#define RED             GRAPHICS_COLOR_CRIMSON;
#define ORANGERED       GRAPHICS_COLOR_ORANGE_RED
//enum message {ttt, score, player, cpu, thinking, illegal};
//enum Message {ttt, score, player, cpu, thinking, illegal} msg;

void InitDisplay();
void DrawTime   (unsigned minutes, unsigned seconds);
void DrawMessage(char *s, uint32_t color);
void DrawScore  (int computerscore, int humanscore, uint32_t color);
void DrawBoard  (tcellstate map[9]);
void DrawWinner (tcellstate map[9],int winner, uint32_t color);

//***MY CODE***MY CODE***MY CODE***MY CODE***MY CODE***
void DrawUnsigned(unsigned line, char prefix[4], unsigned n);
void DrawBoard2();
void changeBoardColor();
void LCDClearDisplay();
void LCDClearDisplayColor(int color);
void resetPowerDisplay();
void drawTrollMeme();
void drawTrollAnimation();
void drawMessageAnimation();
void LCDDrawChar(unsigned row, unsigned col, int8_t c);
//void drawMessageAnimation(char *string, uint32_t color);




#endif /* DISPLAY_H_ */
