#include <stdlib.h>
#include "swtimer.h"
#include "maplogic.h"
#include "button.h"
#include "display.h"
#include "hwtimer.h"
#include "microphone.h"
#include "dtmf.h"
#include "goertzel.h"
#include "stdio.h"
#include "sound.h"
#include "leds.h"
#include "dtmf.h"

// This is the state definition for the top-level state machine,
// implemented in ProcessStep. As you build the game, you will
// have to define additional states to implement the actual game
// logic.
typedef enum {idle, gaming} state_t;

tSWTimer glbl_OST3s;            //Used for Messages
tSWTimer glbl_OST2s;                              //Used for CPU time to place a 'X'
tSWTimer glbl_OST1s;
tSWTimer glbl_OST5s;            //Use for end game screen
tSWTimer glbl_OST200ms;         //Used to toggle lights.
tSWTimer glbl_OST10ms;

int unsigned sampled_value;
unsigned int glbl_Listening = 1;
unsigned int glbl_playerScore = 0;
unsigned int glbl_cpuScore = 0;
unsigned int glbl_flashLEDs = 0;
unsigned int glbl_animation = 0;

tSWTimer everySec;

// This structure is a game state. It keeps track of the current
// playing field, the score achieved by the human, and the score
// achieved by the computer. The game state gets passed around
// during the game to allow different software components access
// to the game state.

typedef struct {
    tcellstate map[9];
    int computerscore;
    int humanscore;
} gamestate_t;

 void T32_INT1_IRQHandler()
{
    //Samples sound from microphone 8kHz. Probably don't need an interrupt
     unsigned vx;
     static unsigned SamplesListened = 0;
     static int cycles = 0;
     static int ani_cycles = 0;

     //if (glbl_Listening) {
     if (glbl_Listening && !glbl_flashLEDs) {
         vx = GetSampleMicrophone();
         DTMFAddSample(vx);
         SamplesListened++;
         if (SamplesListened == 400) { //originally 400
             glbl_Listening = 0;
             SamplesListened = 0;
             //getDTMFPower();
         }
     }

     if (glbl_flashLEDs)
     {
        if (cycles == 1000)            //Call Toggle Player win
        {
            playerWinsLEDS();
            cycles = 0;
        }
        cycles++;
     }

     if (glbl_animation)
     {
         if(ani_cycles == 500)
         {
             //drawTrollAnimation();
             //drawMessageAnimation("CPU WINS!      ", EMPHASISCOLOR);
             drawMessageAnimation();
             ani_cycles = 0;
         }
         ani_cycles++;
     }
     Timer32_clearInterruptFlag(TIMER32_0_BASE);
}

// This function implements the functionality of Tic Tac Tone during
// the idle state, i.e. when it is waiting for the player to provide
// a 'start game' command.
//
// This function is called every time the top-level FSM visits the
// idle state. The function has five parameters. You are welcome to
// add more, but know that the reference solution was implemented using
// only these 5 parameters.
//    b1   = 1 when button S1 is pressed, 0 otherwise
//    b2   = 1 when button S2 is pressed, 0 otherwise
//    sec  = 1 when the second-interval software timer elapses
//    ms50 = 1 when the 50ms-interval software timer elapses
//    G    = gamestate, as defined above. Pass by reference to make
//           sure that changes to G within this function will be
//           propagated out of the function.
//
// Note that this function RETURNS a state_t. This can be used to influence
// the state transition in the top-level FSM that will call ProcessIdle(...).
// Currently, the ProcessIdle always returns idle (which means the top-level
// FSM will iterate in the idle state), but when the game is extended,
// the return state could change into something different (such as playing_circle
// or playing_cross, depending on whether S1 or S2 is pressed).

state_t ProcessIdle(int b1, int b2,
                    int sec, int ms50,
                    gamestate_t *G) {

    // These are the states of a _local_ FSM.
    // The state labels are prefixed with 'processidle' to
    // make sure they are distinct from the labels used for the
    // top-level FSM.
    //
    // The local FSM functionality plays a game of tic-tac-toe
    // against itself, using randomized moves. However, the
    // rules of tic-tac-toe are followed, including the game
    // map drawing and coloring over the reference solution.

    typedef enum {processidle_idle,
                  processidle_playingcircle,
                  processidle_playingcross,
                  processidle_winning,
                  processidle_cpuwin,
                  processidle_playerwin,
                  BTN1Presswait,
                  BTN2Presswait} processidle_state_t;

    static processidle_state_t localstate = processidle_idle;

    static state_t mainState = idle;

    static tcellstate piece = circle;

    unsigned w;
    unsigned w2;

    // We will run this local state machine only once per second,
    // therefore, we only run it when sec is 1. sec is
    // a software-timer generated in the main function.
    //
    // To add more functionality, you can extend this function. For example,
    // to display a label every three seconds, you can add a counter that is
    // incremented for every sec, modulo-3. When the counter is two, it means
    // that the three-second timer mark is reached.
    //
    // A longer counter period (eg modulo-12 iso modulo-3) can be used to
    // display rotating messages.

    if (sec) {
        switch (localstate)
        {
        case processidle_idle:
            if (b1)
            {
                //changeBoardColor(); //Addition
                ClearMap(G->map);
                DrawBoard(G->map);
                localstate = processidle_playingcross;
                mainState = gaming;
                //printf("Idle --> PlayCross\n"); //For debugging
                StartSWTimer(&glbl_OST2s);
                DrawMessage(" Thinking...  ", FOREGROUNDCOLOR);
            }
            else if (b2)
            {
                //changeBoardColor(); //Addition
                ClearMap(G->map);
                DrawBoard(G->map);
                localstate = processidle_playingcircle;
                mainState = gaming;
                //printf("Idle --> PlayCircle\n"); //For debugging
                glbl_Listening = 1;
                Interrupt_enableInterrupt(INT_T32_INT1);
            }
            else
            {
                RandomAdd(G->map, piece);
                if (piece == cross)
                    piece = circle;
                else if (piece == circle)
                    piece = cross;
                DrawBoard(G->map);
                if ((w2 = CrossWins(G->map)) || (w2 = CircleWins(G->map)))
                {
                    DrawWinner(G->map, w2, EMPHASISCOLOR);
                    ClearMap(G->map);
                }
                if (isBoardFilled(G->map))
                    ClearMap(G->map);
            }
            resetPowerDisplay();
            break;
            case processidle_playingcircle:                 //PLAYER is CIRCLE. Add functionality for user to choose game piece.

            // This is circle who is playing. A circle is added in a random (but valid) location.
            // Next, we check if the game ends, which happens when circle or cross would win, or when there's a tie.
            Interrupt_enableInterrupt(INT_T32_INT1); //DO i even need an interrupt?

            DrawMessage(" Listening...  ", FOREGROUNDCOLOR);

            playerMoved(G->map);

             if (!glbl_Listening)
            {
                 displayDTMFPower();
                if(getPlayerMoved()==1)
                {
                    // Next, it's cross' turn
                    localstate = processidle_playingcross;
                    //printf("PlayCircle --> PlayCross\n");

                    DrawBoard(G->map);

                    // If we win or tie, go to winning state instead
                    if (CircleWins(G->map) || Tie(G->map))
                    {
                        localstate = processidle_winning;
                        ////printf("PlayCircle --> Winning \n"); //For debugging
                        //changeBoardColor(); //Addition
                        resetPowerDisplay();
                        DTMFReset();
                    }

                    playerCellSound(); //why warning
                    Interrupt_disableInterrupt(INT_T32_INT1);
                    StartSWTimer(&glbl_OST2s);
                }
                else if(getPlayerMoved()==2)
                {
                Interrupt_disableInterrupt(INT_T32_INT1);
                StartSWTimer(&glbl_OST1s);
                    while (!SWTimerOneShotExpired(&glbl_OST1s))
                    {            // Is it okay to stall here? I think so LOL
                        DrawMessage(" Illegal Move! ", FOREGROUNDCOLOR);
                    }
                DrawMessage(" Listening...  ", FOREGROUNDCOLOR);
                }
                setPlayerMoved(0);
                DTMFReset();
                glbl_Listening = 1;
            }
             if (b1)
            {
                //Fill with Crosses:
                fillBoardCross(G->map);
                DrawBoard(G->map);
                localstate = BTN1Presswait;
                //mainState = idle;
                Interrupt_disableInterrupt(INT_T32_INT1);
                //printf("Reset Called\n");
                glbl_Listening = 1;
                DTMFReset();
            }
              break;
            case processidle_playingcross:              //CPU is CROSS
              // This is cross who is playing. A cross is added in a random (but valid) location. Next,
              // we check if the game ends, which happens when circle or cross would win, or when there's a tie.
              // Interrupt_disableInterrupt(INT_T32_INT1); //Need to move this. MOve ot playCircle state. When sample is finished, then disable
                DrawMessage(" Thinking...  ", FOREGROUNDCOLOR);

             /*if (b1 == true && mainState == gaming)
            {
                localstate = processidle_idle;
                mainState = idle;
                //printf("PlayCross --> Idle\n");
            }*/
             if (SWTimerOneShotExpired(&glbl_OST2s))
            {
                // Decide what position to play
                RandomAdd(G->map, cross);
                //cpuIntelMove(tcellstate map[9]);

                // Next, it's circles' turn
                localstate = processidle_playingcircle;
                //printf("PlayCross --> PlayCirlce\n");

                // If we win or tie, go to winning state instead
                if (CrossWins(G->map) || Tie(G->map))
                {
                    localstate = processidle_winning;
                    //printf("PlayCircle -> Winning\n");
                }
                // update game board status on display
                DrawBoard(G->map);
                InitSWTimer(&glbl_OST2s, TIMER32_1_BASE, SYSTEMCLOCK * 2); //Turns CPU decision clock "OFF"
                cpuCellSound();
                glbl_Listening = 1;
                //Interrupt_enableInterrupt(INT_T32_INT1); //DO i even need an interrupt?
            }
                break;
            case processidle_winning:
              // This state is entered when there is a winner,
              // or it's a tie. In this state, we redraw the
              // winning combination in the emphasis color.
              // After that, we go for the next round.

              //If I decide to include "RESET" functionality in the "processidel_crossplay" state decomment below code
              // localstate = process_idle;

              if (w = CircleWins(G->map))
              {
                glbl_playerScore++;
                DrawWinner(G->map, w, EMPHASISCOLOR); //YELLOW 0x00FFFF00
                localstate = processidle_playerwin;
                winSound();
                //If player wins, display a fancy color flashing of "You WIN!!!"
              }
              else if (w = CrossWins(G->map))
              {
                glbl_cpuScore++;
                DrawWinner(G->map, w, EMPHASISCOLOR); //YELLOW 0x00FFFF00
                localstate = processidle_cpuwin;
                //localstate = processidle_playerwin;
                /*if(bug){*/
                LCDClearDisplayColor(LINECOLOR);
                drawTrollMeme(); //Addition
                loseSound();
              }
              else
              {
              DrawMessage(" TIE GAME!!!   ", FOREGROUNDCOLOR);
              tieSound();
              //could make color of piece flash a certain color
              localstate = processidle_idle;
              mainState = idle;
              }

              StartSWTimer(&glbl_OST5s);
              break;
            case processidle_cpuwin:
            //DrawMessage("CPU WON!!!    ", LINECOLOR);
            Interrupt_enableInterrupt(INT_T32_INT1);
            glbl_animation = 1;

            if(SWTimerOneShotExpired(&glbl_OST200ms))
            {
                playerLosesLEDS();
                StartSWTimer(&glbl_OST200ms);
            }

            if (SWTimerOneShotExpired(&glbl_OST5s))
            {
                //changeBoardColor(); //Addition
                Interrupt_disableInterrupt(INT_T32_INT1);
                glbl_animation = 0;
                DTMFReset();
                turnLEDSOff();
                mainState = idle;
                localstate = processidle_idle;
                LCDClearDisplay();
                ClearMap(G->map);
                DrawBoard(G->map);
            }
                break;
            case processidle_playerwin:
                glbl_flashLEDs = 1;
                Interrupt_enableInterrupt(INT_T32_INT1);

            if (SWTimerOneShotExpired(&glbl_OST200ms))
            {
                playerWinsLEDS();
                StartSWTimer(&glbl_OST200ms);
            }
            if (SWTimerOneShotExpired(&glbl_OST5s))
            {
                //changeBoardColor(); //Addition
                glbl_flashLEDs = 0;
                glbl_animation = 0;
                LCDClearDisplay();
                Interrupt_disableInterrupt(INT_T32_INT1);
                InitTimer(); //Latest addition
                DTMFReset();
                turnLEDSOff();
                mainState = idle;
                localstate = processidle_idle;
                ClearMap(G->map);
                DrawBoard(G->map);
                InitSWTimer(&everySec, TIMER32_1_BASE, SYSTEMCLOCK);
                StartSWTimer(&everySec);
            }
                break;
            case BTN1Presswait:
                if(!b1)
                {
                    localstate = processidle_winning;
                    //printf("BTN1 == 0\n");
                    //printf("B1Wait --> Idle\n");
                }
              break;
            case BTN2Presswait:
                if(!b2)
                {
                    localstate = processidle_idle;
                    //printf("BTN2 == 0\n");
                    //printf("B2Wait --> Idle\n");
                }
        }
    }


    return mainState;
    //return idle; //original code
}

// This is the top-level FSM, which is called from within
// the cyclic executive. You will have to extend this FSM
// with the game logic. The FSM takes four inputs:
//
//    b1   = 1 when button S1 is pressed, 0 otherwise
//    b2   = 1 when button S2 is pressed, 0 otherwise
//    sec  = 1 when the second-interval software timer elapses
//    ms50 = 1 when the 50ms-interval software timer elapses

void ProcessStep(int b1, int b2, int sec, int ms50) {
    static state_t S = idle;
    static gamestate_t G;
    static int msgCode = 1;

    switch (S) {
    case idle:

        S = ProcessIdle(b1, b2, sec, ms50, &G); //maybe extend to take in a "state_t" parameter

         if(SWTimerOneShotExpired(&glbl_OST3s))
        {
             if(msgCode == 4) //Could put this into a HAL function
                msgCode = 0;
             switch (msgCode)
            {
            case 0:
                DrawMessage("  *TicTacTone*  ", ORANGERED);
                break;
            case 1:
                DrawScore(glbl_cpuScore,  glbl_playerScore, LINECOLOR);
                break;
            case 2:
                DrawMessage("  S1: I Start   ", GREENYELLOW);
                break;
            case 3:
                DrawMessage("  S2: CPU Start ", PINK);
                break;
            }
            msgCode++;
            StartSWTimer(&glbl_OST3s);
        }
        //Add functionality where if joystick is pressed, 2 play game can be initiated.

        break;
    case gaming:

        S = ProcessIdle(b1, b2, sec, ms50, &G);

        break;
    }
}

int main(void) {

    // Device Initialization
    InitTimer();
    InitDisplay();
    InitButtonS1();
    InitButtonS2();

    // My Code
    InitMicrophone();
    InitSound();
    InitLEDs();

    Interrupt_enableMaster(); //This function allows the processor to respond to interrupts.
    Interrupt_disableInterrupt(INT_T32_INT1);
    //Interrupt_enableInterrupt(INT_T32_INT2);
    // Software Timer - per second
    // Note that software timers MUST be tied to TIMER32_1_BASE;
    // TIMER32_1_BASE is configured in continuous mode
    // (TIMER32_0_BASE can then be used for periodic interrupts
    //  which will be needed to take samples from microphone)
    //tSWTimer everySec;
    InitSWTimer(&everySec, TIMER32_1_BASE, SYSTEMCLOCK); // Originally just SYSTEMCLOCK
    StartSWTimer(&everySec);

    // Software Timer - per 50ms = 20Hz
    tSWTimer every50ms;
    InitSWTimer(&every50ms, TIMER32_1_BASE, SYSTEMCLOCK/20);
    StartSWTimer(&every50ms);

    //****My Code****
    InitSWTimer(&glbl_OST3s, TIMER32_1_BASE, SYSTEMCLOCK*3);// 3000 ms = 3s SW Timer
    StartSWTimer(&glbl_OST3s);

    InitSWTimer(&glbl_OST2s, TIMER32_1_BASE, SYSTEMCLOCK*2);        // 2000 ms = 2s SW Timer
    InitSWTimer(&glbl_OST1s, TIMER32_1_BASE, SYSTEMCLOCK);          // 1000 ms = 1s SW Timer. Could've just used "everySec"
    InitSWTimer(&glbl_OST5s, TIMER32_1_BASE, SYSTEMCLOCK*5);        // 5000 ms - 5s SW Timer. Used for Game Over Screen
    InitSWTimer(&glbl_OST200ms, TIMER32_1_BASE, SYSTEMCLOCK/200);    // 200 ms. Used to blink Game Over Screen's LEDs.
    InitSWTimer(&glbl_OST10ms, TIMER32_1_BASE, SYSTEMCLOCK/1000);   //

    DrawMessage("  *TicTacTone*  ", ORANGERED);
    DrawBoard2();
    resetPowerDisplay();

//    loseSound(); //For debugging
//    winSound(); //For debuggin

    // The cyclic executive is simple: read the buttons and software
    // timers, and call the top-level FSM in ProcessStep.
    while (1) {
        int b1    = ButtonS1Pressed();
        int b2    = ButtonS2Pressed();
        int sec   = SWTimerExpired(&everySec);
        int ms50  = SWTimerExpired(&every50ms);

        ProcessStep(b1, b2, sec, ms50);

    }

}
