#include "signal.h"
#include "command.h"
#include <RCSwitch.h>

unsigned long int getComm(char row, bool function, char number) {
  return commands[row-1][!function][number-1];
}

void sendSignaltoAll(bool function, RCSwitch rc) {
  unsigned long command;
  for (short r=1; r<=4; r++) {
    for (short n=1; n<=4; n++) {
      command = getComm(r, function, n);
      if (command) rc.send(command, 24); else return; // assuming that there are no commands left
    }
  } 
}

void custom(RCSwitch rc) {
  // ToDo
}
