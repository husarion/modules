@CATEGORY: DistanceSensor
@NAME: basic
@PORTS: stm32
@BOARDS: core2,core2mini
#include <hFramework.h>
#include <DistanceSensor.h>

using namespace hModules;

void hMain()
{
	DistanceSensor sens(hSens1);

	for (;;)
	{
		int dist = sens.getDistance();
		Serial.printf("%d\r\n", dist);
		sys.delay(200);
	}
}

#####
@CATEGORY: DistanceSensor
@NAME: basic
@PORTS: stm32
@BOARDS: robocore
#include <hFramework.h>
#include <DistanceSensor.h>

using namespace hModules;

void hMain()
{
	DistanceSensor sens(hSens1.getBaseSens());

	for (;;)
	{
		int dist = sens.getDistance();
		Serial.printf("%d\r\n", dist);
		sys.delay(200);
	}
}
