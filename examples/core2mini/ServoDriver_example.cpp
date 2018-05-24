#include <hFramework.h>
#include "ServoDriver.h"

using namespace hModules;

void hMain()
{
	hSens3.selectSerial(); // switch hSens interface to the UART mode
	ServoDriver servoDriver_ext(hSens3.serial, 0); // select "0" address (default)
	servoDriver_ext.init();
	servoDriver_ext.enablePower();
	servoDriver_ext.setPowerMedium(); //change servo supply voltage to 6V

	while (1) { 
		for (int i = 0; i < 12; i++) { // set 1us PWM for all servos
			servoDriver_ext.getServo(i+1).setWidth(1000);
		}
		sys.delay(1000);
		for (int i = 0; i < 12; i++) { // set 2us PWM for all servos
			servoDriver_ext.getServo(i+1).setWidth(2000);
		}
		sys.delay(1000);
		LED1.toggle();
	}
}
