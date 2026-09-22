#include <Bluepad32.h>
#ifndef PAD_H
#define PAD_H

void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void dumpGamepad(ControllerPtr ctl);
void dumpMouse(ControllerPtr ctl);
void dumpKeyboard(ControllerPtr ctl);
void dumpBalanceBoard(ControllerPtr ctl); 
void processGamepad(ControllerPtr ctl) ;
void processControllers();
void setup_pad();
int getLAxisY();
int getRAxisY();
int getLAxisX();
int getRAxisX();
int getControllerBattery();
#endif