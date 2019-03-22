#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "leds.h"
#include "display.h"

Graphics_Context g_sContext;

void InitLEDs(){

        GPIO_setAsOutputPin (GPIO_PORT_P2, GPIO_PIN6); //RED on BOOSTER BOARD
        GPIO_setAsOutputPin (GPIO_PORT_P2, GPIO_PIN4); //GREEN on BOOSTER BOARD
        GPIO_setAsOutputPin (GPIO_PORT_P5, GPIO_PIN6); //BLUE on BOOSTER BOARD
        GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
        GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);

        GPIO_setAsOutputPin    (GPIO_PORT_P2,  GPIO_PIN0);  // Right red LED on launchpad
        GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN0);

        GPIO_setAsOutputPin    (GPIO_PORT_P2,  GPIO_PIN1);  // Right green LED on launchpad
        GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN1);

        GPIO_setAsOutputPin    (GPIO_PORT_P2,  GPIO_PIN2);  // Right blue LED on launchpad
        GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN2);

        GPIO_setAsOutputPin    (GPIO_PORT_P1,  GPIO_PIN0);  // Left LED on launchpad
        GPIO_setOutputLowOnPin (GPIO_PORT_P1,  GPIO_PIN0);

}




void playerWinsLEDS(){
    typedef enum {black, red, green, yellow, blue, magenta, cyan, white} color_t;
    static color_t color = black;

    //Have circles fill the board and change color as well.

        switch (color)
        {
        case black: //OFF
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_BLACK);
            color = red;
            break;
        case red:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_RED);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
            color = yellow;
            break;
        case green:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_GREEN);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
            color = cyan;
            break;
        case yellow:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_YELLOW);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
            color = green;
            break;
        case blue:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_BLUE);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);
            color = magenta;
            break;
        case magenta:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_MAGENTA);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);
            color = white;
            break;
        case cyan:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_CYAN);
            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);
            color = blue;
            break;
        case white:
            DrawMessage("    YOU WIN!    ", GRAPHICS_COLOR_WHITE);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
            GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);
            color = red;
            break;
       }

    GPIO_toggleOutputOnPin    (GPIO_PORT_P2,  GPIO_PIN0);  // Right red LED on launchpad
    GPIO_toggleOutputOnPin    (GPIO_PORT_P1,  GPIO_PIN0);  // Left red LED on launchpad

}

void playerLosesLEDS(){


    GPIO_toggleOutputOnPin    (GPIO_PORT_P2,  GPIO_PIN0);  // Right red LED on launchpad
    GPIO_toggleOutputOnPin    (GPIO_PORT_P1,  GPIO_PIN0);  // Left red LED on launchpad
    GPIO_toggleOutputOnPin    (GPIO_PORT_P2, GPIO_PIN6);   //RED on BOOSTER BOARD

}

void turnLEDSOff(){

            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6); // Turns RED on BB Off

            GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4); // Turns GREEN on BB Off

            GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6); // Turns BLUE on BB Off

            GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN0);  // Turns RED LED on Launchpad Off

            GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN1); // Turns GREEN LED on Launchpad Off

            GPIO_setOutputLowOnPin (GPIO_PORT_P2,  GPIO_PIN2); // Turns GREEN LED on Launchpad Off

            GPIO_setOutputLowOnPin (GPIO_PORT_P1,  GPIO_PIN0);  // Turns left LED on Launchpad Off
}

