#include "display.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdlib.h>
#include <stdio.h>

Graphics_Context g_sContext;

int glbl_foreground = GRAPHICS_COLOR_CYAN;
int glbl_linecolor = GRAPHICS_COLOR_WHITE;
int glbl_background = GRAPHICS_COLOR_PURPLE;
int glbl_emphasis = GRAPHICS_COLOR_YELLOW;

void InitDisplay() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, FOREGROUNDCOLOR); //Original
    Graphics_setBackgroundColor(&g_sContext, BACKGROUNDCOLOR); //Original
    //Graphics_setForegroundColor(&g_sContext, glbl_foreground);
    //Graphics_setBackgroundColor(&g_sContext, glbl_background);
    Graphics_setFont(&g_sContext, &g_sFontCmtt16);
    Graphics_clearDisplay(&g_sContext);
}

void DrawTime(unsigned minutes, unsigned seconds) {
    char buf[5];
    buf[4] = (seconds % 10) + '0';
    buf[3] = (seconds / 10) % 10 + '0';
    buf[2] = ':';
    buf[1] = (minutes % 10) + '0';
    buf[0] = (minutes / 10) % 10 + '0';
    Graphics_drawStringCentered(&g_sContext, (int8_t *) buf, 5, 64, 120, OPAQUE_TEXT);
}

//void DrawMessage(char *s, uint32_t color, unsigned int msgCode) //OLD
void DrawMessage(char *s, uint32_t color)   //Original
{
    Graphics_setForegroundColor(&g_sContext, color); // Original
   // Graphics_setForegroundColor(&g_sContext, glbl_foreground);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) s, -1, 64, 8,
                                OPAQUE_TEXT);
}

void DrawScore(int computerscore, int humanscore, uint32_t color) {
    char buf[16] = "Score XX - XX ";
    buf[ 7] =  (char) ('0' + ( computerscore % 10));
    buf[ 6] =  (char) ('0' + ((computerscore/10) % 10));
    buf[12] =  (char) ('0' + ( humanscore % 10));
    buf[11] =  (char) ('0' + ((humanscore/10) % 10));
    Graphics_setForegroundColor(&g_sContext, color); //Original, color same as Emphasis
    //Graphics_setForegroundColor(&g_sContext, glbl_foreground);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) buf, -1, 64, 8, OPAQUE_TEXT);
}

void DrawBoard(tcellstate map[9]) {
    int8_t *symbolMap[3] = {" ", "X", "O"};

    ///Graphics_clearDisplay(&g_sContext);

    //Draw vertial and horizontal lines of Tic_Tac_Toe board
    //Graphics_setForegroundColor(&g_sContext, LINECOLOR); //Original
    Graphics_setForegroundColor(&g_sContext, glbl_linecolor);
    Graphics_drawLineH(&g_sContext, 19,        109,    49);
    Graphics_drawLineH(&g_sContext, 19,        109,    79);
    Graphics_drawLineV(&g_sContext, 49,        19,     109);
    Graphics_drawLineV(&g_sContext, 79,        19,     109);

    // Draw standard X O
    //Graphics_setForegroundColor(&g_sContext, FOREGROUNDCOLOR); //Original
    Graphics_setForegroundColor(&g_sContext, glbl_foreground);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);

    //Graphics_setBackgroundColor(&g_sContext, glbl_background); //Addition
}

//Highlights the winner row/col/diag
void DrawWinner(tcellstate map[9], int winner, uint32_t color) {
    int8_t *symbolMap[3] = {" ", "X", "O"};

    Graphics_setForegroundColor(&g_sContext, color); //Original
    //Graphics_setForegroundColor(&g_sContext, glbl_emphasis);

    switch (winner) {
    case 1: // row 1
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
        break;
    case 2: // row 2
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);
        break;
    case 3: // row 3
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
        break;
    case 4: // col 1
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        break;
    case 5: // col 2
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
        break;
    case 6: // col 3
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
        break;
    case 7: // diagonal
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
        break;
    case 8: // back diagonal
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        break;
    }

}

void DrawUnsigned(unsigned line, char prefix[4], unsigned n) {
    char buf[13]    = "PPPP XXXXXXXX";
    char hexstr[16] = "0123456789ABCDEF";
    buf[0]  = prefix[0];
    buf[1]  = prefix[1];
    buf[2]  = prefix[2];
    buf[3]  = prefix[3];
    buf[4]  = ' ';
    buf[5]  = hexstr[(n >> 28) & 0xF];
    buf[6]  = hexstr[(n >> 24) & 0xF];
    buf[7]  = hexstr[(n >> 20) & 0xF];
    buf[8]  = hexstr[(n >> 16) & 0xF];
    buf[9]  = hexstr[(n >> 12) & 0xF];
    buf[10] = hexstr[(n >>  8) & 0xF];
    buf[11] = hexstr[(n >>  4) & 0xF];
    buf[12] = hexstr[(      n) & 0xF];
    Graphics_drawStringCentered(&g_sContext, (int8_t *) buf,
                                             13, 64,
                                             16 + (line - 1) * 16,
                                             OPAQUE_TEXT);
}

void DrawBoard2()
{
    //Graphics_setForegroundColor(&g_sContext, FOREGROUND);
    Graphics_setForegroundColor(&g_sContext, glbl_linecolor);
    Graphics_drawLineH(&g_sContext, 19,        109,    49);
    Graphics_drawLineH(&g_sContext, 19,        109,    79);
    Graphics_drawLineV(&g_sContext, 49,        19,     109);
    Graphics_drawLineV(&g_sContext, 79,        19,     109);
}

void changeBoardColor()
{
    //LINECOLOR = GRAPHICS_COLOR_YELLOW;
    static int color_state = 1;

    if(color_state > 3)
        color_state = 0;

    switch(color_state)
    {
    case 0:                                         //Default
        glbl_background = GRAPHICS_COLOR_PURPLE;
        glbl_foreground = GRAPHICS_COLOR_CYAN;
        glbl_linecolor = GRAPHICS_COLOR_WHITE;
        glbl_emphasis = GRAPHICS_COLOR_YELLOW;
        break;
    case 1:                                         //Hokie Theme
        glbl_foreground = GRAPHICS_COLOR_ORANGE;
        glbl_linecolor = GRAPHICS_COLOR_WHITE;
        glbl_background = GRAPHICS_COLOR_MAROON;
        glbl_emphasis = GRAPHICS_COLOR_YELLOW;
        break;
    case 2:                                         //Aqua/Winter Theme
        glbl_foreground = GRAPHICS_COLOR_STEEL_BLUE;
        glbl_linecolor = GRAPHICS_COLOR_TEAL;
        glbl_background = GRAPHICS_COLOR_CYAN;
        glbl_emphasis = GRAPHICS_COLOR_YELLOW;
        break;
    case 3:                                         //Earth/Forest Theme
        glbl_foreground = GRAPHICS_COLOR_SADDLE_BROWN;
        glbl_linecolor = GRAPHICS_COLOR_BLACK;
        glbl_background = GRAPHICS_COLOR_SPRING_GREEN;
        glbl_emphasis = GRAPHICS_COLOR_YELLOW;
        break;
    }

    color_state++;
}

void LCDClearDisplay(){
    Graphics_setBackgroundColor(&g_sContext, glbl_background);
    Graphics_clearDisplay(&g_sContext);
}

void LCDClearDisplayColor(int color) {
    Graphics_setBackgroundColor(&g_sContext, color);
    Graphics_clearDisplay(&g_sContext);
}

void resetPowerDisplay(){
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext, "POWER: 000000 ", -1, 0, 110, OPAQUE_TEXT);
}

void drawTrollAnimation(){
    extern tImage troll_meme8BPP_UNCOMP;
    static int xpos = -200;

    Graphics_Rectangle rect = {xpos, 0, xpos+126, 127};

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &rect);
    if(xpos == 200)
        xpos = -200;
    xpos = xpos + 5;
    Graphics_drawImage(&g_sContext, &troll_meme8BPP_UNCOMP, xpos, 0);
}

void drawTrollMeme(){
    extern tImage troll_meme8BPP_UNCOMP;
    Graphics_drawImage(&g_sContext, &troll_meme8BPP_UNCOMP, 0, 16);
}

void LCDDrawChar(unsigned row, unsigned col, int8_t c) {
    Graphics_drawString(&g_sContext,
                        &c,
                        1,
                        8 * (col % 16),
                        16 * (row % 8),
                        OPAQUE_TEXT);
}


void drawMessageAnimation(){
    //static int i = 0;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_PURPLE); // Original
    //Graphics_setForegroundColor(&g_sContext, glbl_foreground);

    static int index0 = 0;    static int index1 = 1;    static int index2 = 2;
    static int index3 = 3;    static int index4 = 4;    static int index5 = 5;
    static int index6 = 6;    static int index7 = 7;    static int index8 = 8;
    static int index9 = 9;    static int index10 = 10;  static int index11 = 11;
    static int index12 = 12;  static int index13 = 13;  static int index14 = 14;
    static int index15 = 15;

    index0++; index1++; index2++; index3++; index4++;
    index5++; index6++; index7++; index8++; index9++;
    index10++; index11++; index12++; index13++; index14++;
    index15++;

    if(index0 == 16)
        index0 = 0;
    else if(index1 == 16)
        index1 = 0;
    else if(index2 == 16)
        index2 = 0;
    else if(index3 == 16)
        index3 = 0;
    else if(index4 == 16)
        index4 = 0;
    else if(index5 == 16)
        index5 = 0;
    else if(index6 == 16)
        index6 = 0;
    else if(index7 == 16)
        index7 = 0;
    else if(index8 == 16)
        index8 = 0;
    else if(index9 == 16)
        index9 = 0;
    else if(index10 == 16)
        index10 = 0;
    else if(index11 == 16)
        index11 = 0;
    else if(index12 == 16)
        index12 = 0;
    else if(index13 == 16)
        index13 = 0;
    else if(index14 == 16)
        index14 = 0;
    else if(index15 == 16)
        index15 = 0;

    //string[index0];

    LCDDrawChar(0, index15, ' ');
    LCDDrawChar(0, index0, 'C');
    LCDDrawChar(0, index1, 'P');
    LCDDrawChar(0, index2, 'U');
    LCDDrawChar(0, index3, ' ');
    LCDDrawChar(0, index4, 'W');
    LCDDrawChar(0, index5, 'I');
    LCDDrawChar(0, index6, 'N');
    LCDDrawChar(0, index7, 'S');
    LCDDrawChar(0, index8, '!');
    LCDDrawChar(0, index9, ' ');
    LCDDrawChar(0, index10, ' ');
    LCDDrawChar(0, index11, ' ');
    LCDDrawChar(0, index12, ' ');
    LCDDrawChar(0, index13, ' ');
    LCDDrawChar(0, index14, ' ');


    /*static char string[16] = "CPU WINS!!!    ";
    static char buf[16] = "BLAHBLAHBLAHBLAH";
    //static char temp1[16];
    //static char temp2[16];

    //temp2 = string;

    buf[0] = string[15];
    buf[1] = string[0];
    buf[2] = string[1];
    buf[3] = string[2];
    buf[4] = string[3];
    buf[5] = string[4];
    buf[6] = string[5];
    buf[7] = string[6];
    buf[8] = string[7];
    buf[9] = string[8];
    buf[10] = string[9];
    buf[11] = string[10];
    buf[12] = string[11];
    buf[13] = string[12];
    buf[14] = string[13];
    buf[15] = string[14];

    strcpy(string, buf);*/

    //strcpy(buf[0], string[15]);
    //strcpy(buf[1], string[0]);

/*  char temp0 = string[15];
    string[15] = string[14];
    string[14] = string[13];
    string[13] = string[12];
    string[12] = string[11];
    string[11] = string[10];
    string[10] = string[9];
    string[9] = string[8];
    string[8] = string[7];
    string[7] = string[6];
    string[6] = string[5];
    string[5] = string[4];
    string[4] = string[3];
    string[3] = string[2];
    string[2] = string[1];
    string[1] = string[0];
    string[0] = temp0;      */

    /*Graphics_drawString(&g_sContext, (int8_t *) buf, -1, 0, 0, OPAQUE_TEXT);
    printf("String Value: %s\n", string);
    printf("   Buf Value: %s\n", buf);*/

}










