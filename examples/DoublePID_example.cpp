@CATEGORY: hDoublePID
@NAME: basic
@PORTS: stm32
@BOARDS: core2,core2mini

#include "hFramework.h"
#include "hDoublePIDRegulator.h"
#include <stdio.h>

using namespace hFramework;

hDoublePIDRegulator dPidReg;

void taskmot()
{
    while (true)
    {
        hMot1.rotRel(90, 1000, 0);
        sys.delay(300);
        hMot1.rotRel(-90, 1000, 0);
        sys.delay(300);
    }
}

void hMain()
{
    sys.setLogDev(&Serial);

    dPidReg.setScale(1);
    dPidReg.setCoeffs(10, 0, 0, 10, 0, 0);

    dPidReg.dtMs = 5;
    dPidReg.stableRange = 10;
    dPidReg.stableTimes = 3;

    hMot1.attachPositionRegulator(dPidReg);

    sys.taskCreate(taskmot);

    for (;;)
    {
        hLED1.toggle();
        sys.delay(500);
    }
}
