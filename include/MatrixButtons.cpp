#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"
#include "MatrixButtons.h"
#include <iostream>
#include <cstdio>
#include <vector>

MatrixButtons::MatrixButtons(unsigned int t_width, unsigned int t_height)
{
    width = t_width;
    height = t_height;

    enable = true;
}

void MatrixButtons::init()
{
    for (size_t i = 0; i < width; i++)
    {
        pins_width[i].setOutOD();
    }
    for (size_t i = 0; i < height; i++)
    {
        pins_height[i].setIn_pd();
    }
}

bool MatrixButtons::debaunce(int pin_num)
{
    sys.delay(20);
    if (pins_height[pin_num].read())
    {
        return true;
    }
    return false;
}

unsigned int MatrixButtons::find(unsigned int t_width, unsigned int t_height)
{
    for (size_t i = 0; i < width_poz.size(); i++)
    {
        if (width_poz[i] == t_width)
            if (height_poz[i] == t_height)
                return i;
    }
    return NULL;
}

void MatrixButtons::update()
{
    if (enable)
    {

        for (size_t i = 0; i < width; i++)
        {

            pins_width[i].setOutOD_pu();
            pins_width[i].write(true);
            for (size_t j = 0; j < height; j++)
            {
                if (pins_height[j].read())
                {
                    if (debaunce(j))
                    {
                        handler[find(i, j)]();
                    }
                }
            }
            pins_width[i].reset();
        }
    }
}

void MatrixButtons::addButon(unsigned int poz_width, unsigned int poz_height, std::function<void(void)> function)
{
    handler.push_back(function);
    width_poz.push_back(poz_width);
    height_poz.push_back(poz_height);
}

void MatrixButtons::addPinOnHeight(hGPIO pin)
{
    pins_height.push_back(pin);
}

void MatrixButtons::addPinOnWidth(hGPIO pin)
{
    pins_width.push_back(pin);
}
