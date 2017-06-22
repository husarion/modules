@CATEGORY: MatrixButtons
@NAME: basic
@PORTS: stm32
@BOARDS: core2,core2mini,robocore
#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "MatrixButtons.h"
#include <iostream>
#include <cstdio>
#include <vector>

void a1_printf() { Serial.printf("a1\n"); }
void a2_printf() { Serial.printf("a2\n"); }
void a3_printf() { Serial.printf("a3\n"); }
void a4_printf() { Serial.printf("a4\n"); }
void b1_printf() { Serial.printf("b1\n"); }
void b2_printf() { Serial.printf("b2\n"); }
void b3_printf() { Serial.printf("b3\n"); }
void b4_printf() { Serial.printf("b4\n"); }
void c1_printf() { Serial.printf("c1\n"); }
void c2_printf() { Serial.printf("c2\n"); }
void c3_printf() { Serial.printf("c3\n"); }
void c4_printf() { Serial.printf("c4\n"); }
void d1_printf() { Serial.printf("d1\n"); }
void d2_printf() { Serial.printf("d2\n"); }
void d3_printf() { Serial.printf("d3\n"); }
void d4_printf() { Serial.printf("d4\n"); }

void hMain()
{
    MatrixButtons a(4, 4);
a.addPinOnHeight(hSens6.getPin1());
a.addPinOnHeight(hSens6.getPin2());
a.addPinOnHeight(hSens6.getPin3());
a.addPinOnHeight(hSens6.getPin4());
a.addPinOnWidth(hSens5.getPin1());
a.addPinOnWidth(hSens5.getPin2());
a.addPinOnWidth(hSens5.getPin3());
a.addPinOnWidth(hSens5.getPin4());

a.init();

a.addButon(0, 0, a1_printf);
a.addButon(0, 1, a2_printf);
a.addButon(0, 2, a3_printf);
a.addButon(0, 3, a4_printf);

a.addButon(1, 0, b1_printf);
a.addButon(1, 1, b2_printf);
a.addButon(1, 2, b3_printf);
a.addButon(1, 3, b4_printf);

a.addButon(2, 0, c1_printf);
a.addButon(2, 1, c2_printf);
a.addButon(2, 2, c3_printf);
a.addButon(2, 3, c4_printf);

a.addButon(3, 0, d1_printf);
a.addButon(3, 1, d2_printf);
a.addButon(3, 2, d3_printf);
a.addButon(3, 3, d4_printf);

    for (;;)
    {
	sys.delay(50);
	LED1.toggle();
    	a.update();
    }
}
