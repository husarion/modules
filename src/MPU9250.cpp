#include "MPU9250.h"

#include <stdio.h>
#include <string.h>
#define DEBUG_CATEGORY MPU
#include "debug.h"

#include "registers.h"

const uint8_t I2C_ADDR = 0x69;

// #define MPU6050_SMPRT_DIV    25
// #define MPU6050_CONFIG       0x1a
// #define MPU6050_GYRO_CONFIG  0x1b
// #define MPU6050_ACCEL_CONFIG 0x1c

// #define REG_I2CMASTERSTATUS_I2CSLV4DONE 0x40

// #define REG_INTSTATUS_RAWDATARDYINT     0x01

// #define REG_GYROCFG_SCALE250            0b00
// #define REG_GYROCFG_SCALE500            0b01
// #define REG_GYROCFG_SCALE1000           0b10
// #define REG_GYROCFG_SCALE2000           0b11
// #define REG_GYROCFG_SCALE_BIT           3
// #define REG_GYROCFG_SCALE_MASK          (0b11 << REG_GYROCFG_SCALE_BIT)

// #define MPU6050_INT_ENABLE   0x38

// #define MPU6050_INT_ENABLE_WOM_EN         0x40
// #define MPU6050_INT_ENABLE_FIFO_OFLOW_EN  0x10
// #define MPU6050_INT_ENABLE_FSYNC_INT_EN   0x08
// #define MPU6050_INT_ENABLE_RAW_RDY_EN     0x01

#define E_I2C(x) \
	if (!(x)) { \
		error = Error::I2CProtocol; \
		SYSLOG("error i2c"); \
		return false; }

#define E(x) \
	if (!(x)) { \
		return false; }

using namespace hModules;

void mpuDelayMs(int delayMs)
{
	sys.delay(delayMs);
}
void mpuDelayUs(int delayUs)
{
	sys.delayUs(delayUs);
}
uint8_t mpuReadCommand(uint8_t cmd, uint8_t* data, uint16_t len, void* userdata)
{
	// printf("read %d %d\r\n", cmd, len);
	MPU9250 *mpu = (MPU9250*)userdata;
	int r = mpu->getSensor().getI2C().rw(I2C_ADDR, &cmd, 1, data, len);
	// sys.delay(5);
	return r;
}
uint8_t mpuSendCommand(uint8_t cmd, const uint8_t* data, uint16_t len, void* userdata)
{
	MPU9250 *mpu = (MPU9250*)userdata;
	uint8_t tx[1 + len];
	tx[0] = cmd;
	memcpy(tx + 1, data, len);
	return mpu->getSensor().getI2C().write(I2C_ADDR, tx, 1 + len);
}

MPU9250::MPU9250(ISensor_i2c& sens)
	: sens(sens)
{
	userdata = this;
}

MPU9250::~MPU9250()
{
}

bool MPU9250::initDMP()
{
	dmp = true;
	sens.selectI2C();
	sens.getI2C().setDataRate(100000);

	int devStatus = dmpInitialize();

	if (devStatus == 0)
	{
		setDMPEnabled(true);
		return true;
	}
	else
	{
		return false;
	}
}
bool MPU9250::init()
{
	dmp = false;
	sens.selectI2C();
	sens.getI2C().setDataRate(100000);

	reset();
	setI2CMasterModeEnabled(true);
	resetI2CMaster();
	setDLPFMode(3);
	setRate(5 - 1);

	E(initMagnetometer());

	return true;
}
bool MPU9250::setGyroScale(GyroScale scale)
{
	uint8_t val;
	switch (scale)
	{
	case GyroScale::DPS_250:  val = MPU9250_GYRO_FS_250;  break;
	case GyroScale::DPS_500:  val = MPU9250_GYRO_FS_500;  break;
	case GyroScale::DPS_1000: val = MPU9250_GYRO_FS_1000; break;
	default:
	case GyroScale::DPS_2000: val = MPU9250_GYRO_FS_2000; break;
	}
	setFullScaleGyroRange(val);
	return true;
}
bool MPU9250::enableInterrupt()
{
	if (!dmp)
	{
		setIntDataReadyEnabled(true);
	}
	sens.getPin1().setIn();
	sens.getPin1().interruptOn_EdgeRising();

	return true;
}
bool MPU9250::disableInterrupt()
{
	if (!dmp)
	{
		setIntDataReadyEnabled(false);
	}
	sens.getPin1().interruptOff();
	return true;
}
void MPU9250::waitForData()
{
	sens.getPin1().interruptWait();
}

bool MPU9250::process()
{
	if (dmp)
	{
		int packetSize = dmpGetFIFOPacketSize();
		bool rd = 0;
		int fifoCount = getFIFOCount();
		while (fifoCount >= packetSize)
		{
			getFIFOBytes(fifoBuffer, packetSize);
			fifoCount -= packetSize;
			rd = 1;
		}
		resetFIFO();

		if (rd)
		{
			dmpGetQuaternionFloat((float*)&quat, fifoBuffer);
			return true;
		}
		return false;
	}
	else
	{
		return true;
	}
}
// bool MPU9250::process()
// {
// uint8_t status;
// E_I2C(readReg(MPU9250_INT_STATUS, status));

// if (!(status & MPU9250_RAW_DATA_RDY_INT_MASK))
// {
// SYSLOG("no data");
// return false;
// }

// E_I2C(readBlock(MPU9250_ACCEL_XOUT_H, &reading, 6 + 2 + 6 + 1 + 6 + 1));
// reading.ax = SWAP16(reading.ax);
// reading.ay = SWAP16(reading.ay);
// reading.az = SWAP16(reading.az);
// reading.temp = SWAP16(reading.temp);
// reading.gx = SWAP16(reading.gx);
// reading.gy = SWAP16(reading.gy);
// reading.gz = SWAP16(reading.gz);

// reading.mx = reading.mx;
// reading.my = reading.my;
// reading.mz = reading.mz;

// return true;
// }

// bool MPU9250::setSelfTestEnabled(bool gyro, bool accel, bool magnet)
// {
// E_I2C(writeReg(MPU9250_CONFIG, 0x02));
// E_I2C(writeReg(MPU9250_SMPLRT_DIV, 0));
// E_I2C(writeReg(MPU9250_ACCEL_CONFIG2, 0x02));

// E_I2C(writeBit(MPU9250_GYRO_CONFIG, MPU9250_XGYRO_CTEN_MASK, gyro));
// E_I2C(writeBit(MPU9250_GYRO_CONFIG, MPU9250_YGYRO_CTEN_MASK, gyro));
// E_I2C(writeBit(MPU9250_GYRO_CONFIG, MPU9250_ZGYRO_CTEN_MASK, gyro));

// E_I2C(writeBit(MPU9250_ACCEL_CONFIG, MPU9250_AX_ST_EN_MASK, accel));
// E_I2C(writeBit(MPU9250_ACCEL_CONFIG, MPU9250_AY_ST_EN_MASK, accel));
// E_I2C(writeBit(MPU9250_ACCEL_CONFIG, MPU9250_AZ_ST_EN_MASK, accel));

// E(writeSlaveReg(MPU9250_MAG_ADDRESS, MPU9250_MAG_ASTC, magnet ? 0x40 : 0x00));
// E(writeSlaveReg(MPU9250_MAG_ADDRESS, MPU9250_MAG_CNTL, magnet ? 0x18 : 0x16));

// return true;
// }
// bool MPU9250::getSelfTestRegisters(uint8_t& gx, uint8_t& gy, uint8_t& gz, uint8_t& ax, uint8_t& ay, uint8_t& az)
// {
// E_I2C(readReg(MPU9250_SELF_TEST_X_GYRO, gx));
// E_I2C(readReg(MPU9250_SELF_TEST_Y_GYRO, gy));
// E_I2C(readReg(MPU9250_SELF_TEST_Z_GYRO, gz));

// E_I2C(readReg(MPU9250_SELF_TEST_X_ACCEL, ax));
// E_I2C(readReg(MPU9250_SELF_TEST_Y_ACCEL, ay));
// E_I2C(readReg(MPU9250_SELF_TEST_Z_ACCEL, az));
// return true;
// }
