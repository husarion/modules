@MODULE: MPU9250
@NAME: basic
@PORTS: stm32
@BOARDS: robocore,core2,core2mini
#include <hFramework.h>
#include <MPU9250.h>

using namespace hModules;

void hMain()
{
	sys.setLogDev(&Serial);

{% if board(robocore) %}
	MPU9250 mpu(hSens1.getBaseSens());
{% else %}
	MPU9250 mpu(hSens1);
{% endif %}

	mpu.init();

	for (;;)
	{
		mpu.waitForData();

		if (mpu.process())
		{
			int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
			mpu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

			printf("\n");
			printf("ax %6d ay %6d az %6d \r\n", ax, ay, az);
			printf("gx %6d gy %6d gz %6d \r\n", gx, gy, gz);
			printf("mx %6d my %6d mz %6d \r\n", mx, my, mz);
		}
	}
}
