#include "goertzel.h"

#define PTHRESHOLD 1000
#define FRACBITS 8
#define FIXMUL(A, B) ((A * B) >> FRACBITS)

int ScaleSample(unsigned s) {
    // scale microphone sample to fix <32, FRACBITS>
    // the microphone signal is assumed to lie
    // between 0x1000 (min) and 0x2FFF (max) with midpoint 0x1FFF
    //
    // we map to <32, 10>
    return ((int) s - 0x1FFF) >> (12 - FRACBITS);
}

void SampleGoertzel(Gtap *t, unsigned x) {
    int s0 = x + FIXMUL(t->coef, t->s1) - t->s2;
    t->s2 = t->s1;
    t->s1 = s0;
}

void ResetGoertzel(Gtap *t) {
    t->s1 = 0;
    t->s2 = 0;
}

int PowerGoertzel(Gtap *t) {
    int power = FIXMUL(t->s1, t->s1) + FIXMUL(t->s2, t->s2) - FIXMUL(FIXMUL(t->coef, t->s1), t->s2);
    return power;
}
