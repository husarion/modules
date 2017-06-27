/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hDoublePID.h"
#include "hSystem.h"

namespace hFramework
{

hDoublePID::hDoublePID()
	: scale(1), Kp_pos(1), Ki_pos(0), Kd_pos(0), Kp_neg(1), Ki_neg(0), Kd_neg(0), flags(PID_FLAG_I_ENABLED), isum(0), lastError(0)
{
}
hDoublePID::hDoublePID(float Kp_pos, float Ki_pos, float Kd_pos, float Kp_neg, float Ki_neg, float Kd_neg)
	: scale(1), Kp_pos(Kp_pos), Ki_pos(Ki_pos), Kd_pos(Kd_pos), Kp_neg(Kp_neg), Ki_neg(Ki_neg), Kd_neg(Kd_neg), flags(PID_FLAG_I_ENABLED), isum(0), lastError(0)
{
}

void hDoublePID::reset()
{
	isum = 0;
	lastError = 0;
}

void hDoublePID::setRangeMin(float min)
{
	this->min = min;
	flags |= PID_FLAG_HAS_MIN;
}
void hDoublePID::setRangeMax(float max)
{
	this->max = max;
	flags |= PID_FLAG_HAS_MAX;
}
void hDoublePID::setIRangeMin(float imin)
{
	this->imin = imin;
	flags |= PID_FLAG_HAS_IMIN;
}
void hDoublePID::setIRangeMax(float imax)
{
	this->imax = imax;
	flags |= PID_FLAG_HAS_IMAX;
}

float hDoublePID::update(float error, int dt_ms)
{
	float curErr;
	if (flags & PID_FLAG_HAS_LAST_ERROR)
	{
		curErr = error - lastError;
	}
	else
	{
		curErr = 0;
		flags |= PID_FLAG_HAS_LAST_ERROR;
	}
	lastError = error;

	float tmpIsum = 0;
	if (Ki_pos > 0.0f && Ki_neg > 0.0f && (flags & PID_FLAG_I_ENABLED))
	{
		if (error >= 0)
		{
			isum += Ki_pos * error * dt_ms;
		}
		else
		{
			isum += Ki_neg * error * dt_ms;
		}

		if ((flags & PID_FLAG_HAS_IMAX) && isum > imax)
			isum = imax;
		else if ((flags & PID_FLAG_HAS_IMIN) && isum < imin)
			isum = imin;

		tmpIsum = isum;
	}

	float val;
	if (error >= 0)
	{
		val = Kp_pos * error + tmpIsum + Kd_pos * curErr / dt_ms;
	}
	else
	{
		val = Kp_neg * error + tmpIsum + Kd_neg * curErr / dt_ms;
	}
	val *= scale;
	// sys.log("%f %f %x\r\n", min, max, flags);
	if ((flags & PID_FLAG_HAS_MAX) && val > max)
		val = max;
	else if ((flags & PID_FLAG_HAS_MIN) && val < min)
		val = min;
	return val;
}

}