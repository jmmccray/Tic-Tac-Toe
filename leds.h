/*
 * leds.h
 *
 *  Created on: Apr 22, 2018
 *      Author: jmmcc
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "swtimer.h"
#include "hwtimer.h"

void InitLEDs();
void playerWinsLEDS();
void playerLosesLEDS();
void turnLEDSOff();

#endif /* LEDS_H_ */
