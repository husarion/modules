#include "ServoDriver.h"
#include <string.h>

#define DEBUG_CATEGORY SERVO_DRIVER
#include "debug.h"

namespace hModules
{

#define FULL_LEN (1 + 1 + 2 + 12 * 2 + 2)
#define SINGLE_LEN (1 + 1 + 1 + 2 + 2)
#pragma pack(1)
typedef struct
{
	uint8_t type: 1;
	uint8_t addr: 2;
	uint8_t powerMode: 2;
	uint8_t powerEnabled: 1;
	uint8_t num;
	union
	{
		struct
		{
			uint16_t period;
			uint16_t width[12];
			uint16_t crc;
		} full;
		struct
		{
			uint8_t servoNum;
			uint16_t width;
			uint16_t crc;
		} single;
	} data;
} TCommPacket;
#pragma pack()

// ***************************************************************
// ********************** ServoDriver_Servo **********************
// ***************************************************************
ServoDriver_Servo::ServoDriver_Servo(ServoDriver& driver, int num)
	: driver(driver), num(num)
{
}

void ServoDriver_Servo::setWidth(uint16_t widthUs)
{
	driver.setWidth(num - 1, widthUs);
}
void ServoDriver_Servo::setPeriod(uint16_t periodUs)
{
	driver.setPeriod(periodUs);
}

// ***************************************************************
// ************************* ServoDriver *************************
// ***************************************************************
#ifdef HAS_HEXT
ServoDriver::ServoDriver(hExtClass& ext, int address)
	: s1(*this, 1), s2(*this, 2), s3(*this, 3), s4(*this, 4),
	  s5(*this, 5), s6(*this, 6), s7(*this, 7), s8(*this, 8),
	  s9(*this, 9), s10(*this, 10), s11(*this, 11), s12(*this, 12),
	  ext(&ext), serial(ext.serial), initialized(false), autoUpdate(true), pnum(0), period(20000), retransmissions(true),
		address(address)
{
	memset(width, 0, sizeof(width));
}
#endif
ServoDriver::ServoDriver(ISerial& serial, int address)
	: s1(*this, 1), s2(*this, 2), s3(*this, 3), s4(*this, 4),
	  s5(*this, 5), s6(*this, 6), s7(*this, 7), s8(*this, 8),
	  s9(*this, 9), s10(*this, 10), s11(*this, 11), s12(*this, 12),
	  serial(serial), initialized(false), autoUpdate(true), pnum(0), period(20000), retransmissions(true),
		address(address)
{
	memset(width, 0, sizeof(width));
}

void ServoDriver::init()
{
	hMutexGuard guard(mutex);

	if (initialized)
		return;
	serial.setBaudrate(115200);
#ifdef HAS_HEXT
	if (ext)
	{
	}
#endif
	initialized = true;
}

bool ServoDriver::setWidth(int num, uint16_t widthUs)
{
	hMutexGuard guard(mutex);

	init();

	width[num] = widthUs;

	if (autoUpdate)
	{
		TCommPacket packet;
		packet.type = 1; // single
		packet.addr = address;
		packet.powerMode = powerMode;
		packet.powerEnabled = powerEnabled;
		packet.num = pnum++;
		packet.data.single.servoNum = num;
		packet.data.single.width = widthUs;
		packet.data.single.crc = crc16_calc(&packet, SINGLE_LEN - 2);

		return sendAndWaitForAck(&packet, SINGLE_LEN);
	}
	else
	{
		return true;
	}
}

bool ServoDriver::update()
{
	hMutexGuard guard(mutex);

	init();

	TCommPacket packet;
	packet.type = 0;
	packet.addr = address;
	packet.powerMode = powerMode;
	packet.powerEnabled = powerEnabled;
	packet.num = pnum++;
	packet.data.full.period = period;
	for (int i = 0; i < 12; i++)
		packet.data.full.width[i] = width[i];
	packet.data.full.crc = crc16_calc(&packet, FULL_LEN - 2);

	return sendAndWaitForAck(&packet, FULL_LEN);
}

bool ServoDriver::sendAndWaitForAck(void* data, int len)
{
	TCommPacket *packet = (TCommPacket*)data;

	if (retransmissions)
	{
		for (int i = 0; i < 10; i++)
		{
			serial.write(packet, len);

			char d[1];
			int r = serial.read(d, 1, 10);
			if (r == 1 && d[0] == packet->num)
				return true;
		}
		return false;
	}
	else
	{
		serial.write(packet, len);
		return true;
	}
}

ServoDriver_Servo& ServoDriver::getServo(int num)
{
	switch (num)
	{
	default:
	case 1: return s1;
	case 2: return s2;
	case 3: return s3;
	case 4: return s4;
	case 5: return s5;
	case 6: return s6;
	case 7: return s7;
	case 8: return s8;
	case 9: return s9;
	case 10: return s10;
	case 11: return s11;
	case 12: return s12;
	}
}

}