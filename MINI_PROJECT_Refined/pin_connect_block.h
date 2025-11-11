//pin_connect_block.h
#include "types.h"

#define OUTPUT 1
#define INPUT 0

void CfgPortPinFunc(ui32 portNo,ui32 pinNo,ui32 pinFunc);
void ReservePins(ui32 port,ui32 pins,ui32 from,ui32 dir);
