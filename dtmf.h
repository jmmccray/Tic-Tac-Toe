#ifndef DTMF_H_
#define DTMF_H_

#include "goertzel.h"
#include "maplogic.h"

void     DTMFAddSample(unsigned x);
void     DTMFReset();
unsigned DTMFPower();
void     DTMFDisplay();

//***MY CODE***MY CODE***MY CODE***MY CODE
void getDTMFPower();
void displayDTMFPower();
void playerMoved(tcellstate map[9]);
void setPlayerMoved(int yes_no);
int getPlayerMoved();
void playerCellSound();

#endif /* DTMF_H_ */
