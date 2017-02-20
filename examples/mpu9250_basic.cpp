@CATEGORY: MPU9250
@NAME: basic
@PORTS: stm32
@BOARDS: core2,core2mini
#include <hFramework.h>
#include <MPU9250.h>

using namespace hModules;

void hMain()
{
	sys.setLogDev(&Serial);

	MPU9250 mpu(hSens1);

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

#####
@CATEGORY: MPU9250
@NAME: basic
@PORTS: stm32
@BOARDS: robocore
#include <hFramework.h>
#include <MPU9250.h>

using namespace hModules;

void hMain()
{
	sys.setLogDev(&Serial);

	MPU9250 mpu(hSens1.getBaseSens());

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
