#include "dtmf.h"
#include "display.h"
#include "stdio.h"
#include "sound.h"

static Gtap t697  = {(int) (1.70774 * (1 << FRACBITS)), 0, 0};
static Gtap t770 = {(int) (1.645281 * (1 << FRACBITS)), 0, 0};
static Gtap t852 = {(int) (1.568687 * (1 << FRACBITS)), 0, 0};
static Gtap t1209 = {(int) (1.164104 * (1 << FRACBITS)), 0, 0};
static Gtap t1336 = {(int) (0.9963702 * (1 << FRACBITS)), 0, 0};
static Gtap t1477 = {(int) (0.7986184 * (1 << FRACBITS)), 0, 0};

Graphics_Context g_sContext;

int glbl_playerMoved = 0;  //Initialized as FALSE
int glbl_cellSound = 0;
typedef enum {cell0, cell1, cell2, cell3, cell4,
                   cell5, cell6, cell7, cell8, none} cellNoise;
cellNoise cellnum = none;

void DTMFAddSample(unsigned x) {
    unsigned vx = ScaleSample(x);

    SampleGoertzel( &t697, vx);
    SampleGoertzel( &t770, vx);
    SampleGoertzel( &t852, vx);
    SampleGoertzel(&t1209, vx);
    SampleGoertzel(&t1336, vx);
    SampleGoertzel(&t1477, vx);
}

void DTMFReset() {
    ResetGoertzel( &t697);
    ResetGoertzel( &t770);
    ResetGoertzel( &t852);
    ResetGoertzel(&t1209);
    ResetGoertzel(&t1336);
    ResetGoertzel(&t1477);
}

void DTMFDisplay() {
    unsigned p697  = PowerGoertzel(  &t697);
    unsigned p770  = PowerGoertzel(  &t770);
    unsigned p852  = PowerGoertzel(  &t852);
    unsigned p1209 = PowerGoertzel( &t1209);
    unsigned p1336 = PowerGoertzel( &t1336);
    unsigned p1477 = PowerGoertzel( &t1477);

    DrawUnsigned(1, " 697",  p697);
    DrawUnsigned(2, " 770",  p770);
    DrawUnsigned(3, " 852",  p852);
    DrawUnsigned(4, "1209", p1209);
    DrawUnsigned(5, "1336", p1336);
    DrawUnsigned(6, "1477", p1477);
}

void getDTMFPower() {

    //Before, I tried using int data, int did not return expected results
    unsigned p697  = PowerGoertzel(&t697);
    unsigned p770  = PowerGoertzel(&t770);
    unsigned p852  = PowerGoertzel(&t852);
    unsigned p1209 = PowerGoertzel(&t1209);
    unsigned p1336 = PowerGoertzel(&t1336);
    unsigned p1477 = PowerGoertzel(&t1477);

    //For debugging
    printf("t697: %u\n", p697);
    printf("t770: %u\n", p770);
    printf("t852: %u\n", p852);
    printf("t1209: %u\n", p1209);
    printf("t1336: %u\n", p1336);
    printf("t1477: %u\n", p1477);

}

void displayDTMFPower() {

    //Before, I tried using int data, int did not return expected results
    unsigned int p697  = PowerGoertzel(&t697);
    unsigned int p770  = PowerGoertzel(&t770);
    unsigned int p852  = PowerGoertzel(&t852);
    unsigned int p1209 = PowerGoertzel(&t1209);
    unsigned int p1336 = PowerGoertzel(&t1336);
    unsigned int p1477 = PowerGoertzel(&t1477);

    unsigned int sum = p697 + p770 + p852 + p1209 + p1336 + p1477;

    char buf[16] = {'P','O','W','E','R',':',' ','0','0','0','0','0','0',' ',' '}; //or "PWR: 000000"
    buf[12] = (char) ('0' + (sum%10));
    buf[11] = (char) ('0' + ((sum/10)%10));
    buf[10] = (char) ('0' + ((sum/100)%10));
    buf[9] = (char) ('0' + ((sum/1000)%10));
    buf[8] = (char) ('0' + ((sum/10000)%10));
    buf[7] = (char) ('0' + ((sum/100000)%10));

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext, (int8_t *) buf, -1, 0, 110, OPAQUE_TEXT);
    //printf("Power: %u\n", sum);     //For debugging
}

void playerMoved(tcellstate map[9]){

    //should I set pXXXX to global variables?
    unsigned p697  = PowerGoertzel(&t697);
    unsigned p770  = PowerGoertzel(&t770);
    unsigned p852  = PowerGoertzel(&t852);
    unsigned p1209 = PowerGoertzel(&t1209);
    unsigned p1336 = PowerGoertzel(&t1336);
    unsigned p1477 = PowerGoertzel(&t1477);



    //For Debugging
    /*printf("t697: %u\n", p697);
    printf("t770: %u\n", p770);
    printf("t852: %u\n", p852);
    printf("t1209: %u\n", p1209);
    printf("t1336: %u\n", p1336);
    printf("t1477: %u\n", p1477);*/

    //Lowest, power "spike" 100-20000,
    //Maybe change thresholds to 100-1200
    if(p697 > 200 && p1209 > 200 ){         //map[0]
        if(map[0] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = true;
        map[0] = circle;
        printf("Map[0]\n");
        cellnum = cell0;
        }
    }
    else if (p697 > 200 && p1336 > 200){   //map[1]
        if (map[1] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[1] = circle;
        printf("Map[1]\n");
        cellnum = cell1;
        }
    }
    else if (p697 > 200 && p1477 > 200){   //map[2]
        if (map[2] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[2] = circle;
        printf("Map[2]\n");
        cellnum = cell2;
        }
    }
    else if (p770 > 200 && p1209 > 200){   //map[3]
        if (map[3] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[3] = circle;
        printf("Map[3]\n");
        cellnum = cell3;
        }
    }
    else if (p770 > 200 && p1336 > 200){   //map[4]
        if (map[4] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[4] = circle;
        printf("Map[4]\n");
        cellnum = cell4;
        }
    }
    else if (p770 > 200 && p1477 > 200 && map[5] == empty){   //map[5]
        if (map[5] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[5] = circle;
        printf("Map[5]\n");
        cellnum = cell5;
        }
    }
    else if (p852 > 200 && p1209 > 200){   //map[6]
        if (map[6] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[6] = circle;
        printf("Map[6]\n");
        cellnum = cell6;
        }
    }
    else if (p852 > 200 && p1336 > 200){    //map[7]
        if (map[7] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[7] = circle;
        printf("Map[7]\n");
        cellnum = cell7;
        }
    }
    else if (p852 > 200 && p1477 > 200 ){    //map[8]
        if (map[8] != empty)
        {
            glbl_playerMoved = 2;
        }
        else
        {
        glbl_playerMoved = 1;
        map[8] = circle;
        printf("Map[8]\n");
        cellnum = cell8;
        }
    }
    else
    {
        cellnum = none;
        glbl_playerMoved = 0;
    }

}

void setPlayerMoved(int yes_no){
    glbl_playerMoved = yes_no;
}

int getPlayerMoved()
{
    return glbl_playerMoved;
}

void playerCellSound(){
/*typedef enum {note_silent, note_c4, note_d4, note_e4, note_f4,  note_g4,
                  note_a4, note_b4, note_c5, note_d5, note_e5, note_f5,
                  note_fs5, note_g5, note_a5, note_b5, note_c6} tnote;*/
       switch(cellnum)
       {
       case cell0:
           PlaySound(note_c4, 500);
           PlaySound(note_c6, 500);
           cellnum = none;
           break;
       case cell1:
           PlaySound(note_d4, 500);
           PlaySound(note_b5, 500);
           cellnum = none;
           break;
       case cell2:
           PlaySound(note_e4, 500);
           PlaySound(note_b5, 500);
           cellnum = none;
           break;
       case cell3:
           PlaySound(note_f4, 500);
           PlaySound(note_a5, 500);
           cellnum = none;
           break;
       case cell4:
           PlaySound(note_g4, 500);
           PlaySound(note_g5, 500);
           cellnum = none;
           break;
       case cell5:
           PlaySound(note_a4, 500);
           PlaySound(note_fs5, 500);
           cellnum = none;
           break;
       case cell6:
           PlaySound(note_b4, 500);
           PlaySound(note_f5, 500);
           cellnum = none;
           break;
       case cell7:
           PlaySound(note_c5, 500);
           PlaySound(note_e5, 500);
           cellnum = none;
           break;
       case cell8:
           PlaySound(note_g5, 500);
           PlaySound(note_e4, 500);
           cellnum = none;
           break;
       case none:
           cellnum = none;
           break;
       }

}






