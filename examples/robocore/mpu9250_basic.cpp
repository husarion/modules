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
        if (mpu.process())
        {
            int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
            mpu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
            sys.delay(500);
            printf("\n");
            printf("ax %6d ay %6d az %6d \r\n", ax, ay, az);
            printf("gx %6d gy %6d gz %6d \r\n", gx, gy, gz);
            printf("mx %6d my %6d mz %6d \r\n", mx, my, mz);
        }
    }
}