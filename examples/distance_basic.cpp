@MODULE: DistanceSensor
@NAME: basic
@PORTS: stm32
@BOARDS: robocore,core2,core2mini
#include <hFramework.h>
#include <DistanceSensor.h>

using namespace hModules;

void hMain()
{
{% if board(robocore) %}
	DistanceSensor sens(hSens1.getBaseSens());
{% else %}
	DistanceSensor sens(hSens1);
{% endif %}

	for (;;)
	{
		int dist = sens.getDistance();
		Serial.printf("%d\r\n", dist);
		sys.delay(200);
	}
}
