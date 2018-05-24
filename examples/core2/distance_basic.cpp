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