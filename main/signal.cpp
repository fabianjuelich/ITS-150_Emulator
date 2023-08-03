#include "signal.h"
#include "command.h"
#include <RCSwitch.h>

unsigned long int getComm(char row, bool function, char number) {
  return commands[row-1][!function][number-1];
}

void sendSignaltoAll(bool function, RCSwitch rf) {
  unsigned long com;
  for (short r=1; r<=4; r++) {
    for (short n=1; n<=4; n++) {
      com = getComm(r, function, n);
      if (com) rf.send(com, 24); else return;
    }
  } 
}

void custom() {
  // ToDo
}