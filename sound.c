#include "sound.h"
#include "hwtimer.h"
#include "swtimer.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// This configuration sets a 50% duty cycle configuration
// for CCR4. You will have to figure out what Timer_A module
// drives the buzzer, i.e. what pin TAx.4 is driving the
// buzzer. The Timer_A module then is x.


//void InitSWTimer(tSWTimer *T, uint32_t  hwtimer,    uint32_t  period);

Timer_A_PWMConfig pwmConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        (int) (SYSTEMCLOCK / 523.25),         // C5
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        (int) ((SYSTEMCLOCK/2) / 523.25)
};


void InitSound() {
    // This function switches the IC pin connected to
    // the buzzer from GPIO functionality to Timer_A functionality
    // so that we can drive it with PWM.

    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P2,
            GPIO_PIN7,
            GPIO_PRIMARY_MODULE_FUNCTION);
}

void PlaySound(tnote n, unsigned ms) {

   //Orignally: 17 different notes

   //Added
   //note_c0, note_db0, note_d0, note_b0,
   //note_c1, note_db1, note_d1, note_a1,
   //note_b1, note_c2,  note_d3,

    switch (n)
    {
    case note_silent: // 0Hz
        pwmConfig.timerPeriod = 0;
        pwmConfig.dutyCycle = 0;
        break;
    case note_c0: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 16.35);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 16.35);
                break;
    case note_db0: // 261.63
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 17.32);
                        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 17.32);
                        break;
    case note_d0: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 18.35);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 18.35);
                break;
    case note_b0: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 30.87);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 30.87);
                break;
    case note_c1: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 32.70);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 32.70);
                break;
    case note_db1: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 34.65);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 34.65);
                break;
    case note_d1: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 36.71);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 36.71);
                break;
    case note_a1: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 55.00);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 55.00);
                break;
    case note_b1: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 61.74);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 61.74);
                break;
    case note_c2: // 261.63
                pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 65.41);
                pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 65.41);
                break;
    case note_d3: // 261.63
            pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 146.83);
            pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 146.83);
            break;

    ///Original Notes!!
    case note_c4: // 261.63
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 261.63);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 261.63);
        break;
    case note_d4: // 293.66
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 293.66);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 293.66);
        break;
    case note_e4: // 329.63
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 329.63);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 329.63);
        break;
    case note_f4: // 349.23
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 349.23);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 349.23);
        break;
    case note_g4: // 392.00
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 392.00);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 392.00);
        break;
    case note_a4: // 440.00
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 440.00);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 440.00);
        break;
    case note_b4: // 493.88
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 493.88);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 493.88);
        break;
    case note_c5: // 523.25
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 523.25);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 523.25);
        break;
    case note_d5: // 587.33
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 587.33);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 587.33);
        break;
    case note_e5: // 659.25
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 659.25);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 659.25);
        break;
    case note_f5: // 698.46
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 698.46);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 698.46);
        break;
    case note_fs5: // 739.99
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 739.99);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 739.99);
        break;
    case note_g5: // 783.99
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 783.99);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 783.99);
        break;
    case note_a5: // 880.00
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 880.00);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 880.00);
        break;
    case note_b5: // 987.77
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 987.77);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 987.77);
        break;
    case note_c6: // 1046.50
        pwmConfig.timerPeriod = (int) (SYSTEMCLOCK / 1046.50);
        pwmConfig.dutyCycle = (int) ((SYSTEMCLOCK / 2) / 1046.50);
        break;
    }

    tSWTimer OST;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    InitSWTimer(&OST, TIMER32_1_BASE, ms*(SYSTEMCLOCK/1000)); //correct base?
    StartSWTimer(&OST);
    while(!SWTimerOneShotExpired(&OST)){}
    pwmConfig.dutyCycle = 0;
    pwmConfig.timerPeriod =  0;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

}

void loseSound(){
    PlaySound(note_d5, 700);
    PlaySound(note_d4, 700);
    PlaySound(note_g4, 700);
    PlaySound(note_c4, 1200);
}
void winSound(){
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);
    PlaySound(note_b5, 50);
    PlaySound(note_c6, 50);

}

void tieSound(){
    PlaySound(note_d5, 2500);
}
