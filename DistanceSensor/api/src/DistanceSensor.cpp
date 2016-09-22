/*
 * This file is part of hFramework
 *
 * Copyright (C) 2014 Husarion Sp. z o.o. <husarion.com> -  All Rights Reserved
 *
 * Unauthorized copying of this file and the hFramework library, via any medium
 * is strictly prohibited.
 * Proprietary and confidential.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------
 *
 * ----------------------------------------------------------------------------
 */

#include "DistanceSensor.h"

namespace hModules
{

class DistanceSensorPimpl
{
public:
	uint32_t start;
	uint32_t distance;
	bool initialized;
	ISensor* s;
};

void distSensCallback(void* p)
{
	DistanceSensorPimpl* impl = ((DistanceSensorPimpl*)p);

	uint32_t a = sys.getUsTimVal();
	if (impl->s->getPin1().read() == 1)
	{
		impl->start  = a;
	}
	else
	{
		impl->distance = a - impl->start;
		impl->distance = 34 * (impl->distance) / 2000; //distance in [cm]
	}
}

DistanceSensor::DistanceSensor(ISensor& sens)
{
	impl = new DistanceSensorPimpl();
	impl->initialized = false;
	impl->s = &sens;
}

void DistanceSensor::init()
{
	if (impl->initialized)
		return;

	impl->s->getPin1().interruptOn(InterruptEdge::Both, distSensCallback, impl);
	impl->s->getPin3().setOut();
	impl->s->getPin3().write(0);
	impl->initialized = true;
}

int16_t DistanceSensor::getDistance()
{
	init();

	impl->start = 0;
	impl->s->getPin3().write(1);
	sys.delay(1);
	impl->s->getPin3().write(0);
	sys.delay(24); //delay for max distance 4m

	if (impl->start != 0 && impl->distance < 401)
	{
		return (int16_t)impl->distance;
	}
	else
	{
		return -1;
	}
}

}
