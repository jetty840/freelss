/*
 ****************************************************************************
 *  Copyright (c) 2014 Uriah Liggett <freelaserscanner@gmail.com>           *
 *	This file is part of FreeLSS.                                           *
 *                                                                          *
 *  FreeLSS is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  FreeLSS is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with FreeLSS.  If not, see <http://www.gnu.org/licenses/>.       *
 ****************************************************************************
*/
#include "Main.h"
#include "Lighting.h"
#include "Setup.h"
#include <softPwm.h>
#include <ws2811.h>

#define WS281X_LED_COUNT        12
#define WS281X_DMA              5
#define WS281X_TARGET_FREQ      800000
#define WS281X_STRIP_TYPE       WS2811_STRIP_GRB

namespace freelss
{
Lighting * Lighting::m_instance = NULL;

ws2811_t ledstring =
{
    .device = NULL,
    .rpi_hw = NULL,
    .freq = 0,
    .dmanum = 0,
    .channel =
    {
        [0] =
        {
            .gpionum = 0,
            .invert = 0,
            .count = 0,
            .brightness = 255,
            .strip_type = 0,
	    .leds = NULL,
        },
        [1] =
        {
            .gpionum = 0,
            .invert = 0,
            .count = 0,
            .brightness = 0,
	    .strip_type = 0,
	    .leds = NULL,
        },
    },
};


Lighting * Lighting::get()
{
	if (Lighting::m_instance == NULL)
	{
		Lighting::m_instance = new Lighting();
	}

	return Lighting::m_instance;
}

void Lighting::release()
{
	delete Lighting::m_instance;
	Lighting::m_instance = NULL;
}

Lighting::Lighting() :
	m_pin(-1),
	m_intensity(0)
{
	Setup * setup = Setup::get();
	m_pin = setup->lightingPin;

	if (setup->enableLighting )
	{
		if ( setup->lightingType == LT_PWM )
			softPwmCreate(m_pin, m_intensity, 100);
		if ( setup->lightingType == LT_WS281X )
		{
			// Setup the Neopixels and set to black
			ledstring.freq = WS281X_TARGET_FREQ;
			ledstring.dmanum = WS281X_DMA;
			ledstring.channel[0].gpionum = m_pin;
			ledstring.channel[0].count = WS281X_LED_COUNT;
			ledstring.channel[0].strip_type = WS281X_STRIP_TYPE;

			if ( ws2811_init(&ledstring) )
				std::cerr << "Error ws2811_init failed" << std::endl;
		}
		setPreset(LP_OFF);
	}
}


void Lighting::setIntensity(int intensity)
{
	Setup *setup = Setup::get();

	if (setup->enableLighting && setup->lightingType == LT_PWM && intensity >= 0 && intensity <= 100)
	{
		softPwmWrite (m_pin, intensity);
		m_intensity = intensity;
	}
}

int Lighting::getIntensity() const
{
	return m_intensity;
}


static void setRGBLed(uint32_t rgb, int led)
{
	ledstring.channel[0].leds[led] = rgb;
}


// 0x00RRGGBB
void Lighting::setRGB(uint32_t rgb)
{
	Setup *setup = Setup::get();

	if ( setup->enableLighting )
	{
		if ( setup->lightingType == LT_WS281X )
		{
			for (int i = 0; i < WS281X_LED_COUNT; i ++ )
				setRGBLed(rgb, i);
			ws2811_render(&ledstring);
			ws2811_wait(&ledstring);
		}

		if ( setup->lightingType == LT_PWM )
		{
			int intensity = (int)(((float)(rgb & 0xFF)/255.0) * 100.0);
			setIntensity(intensity);
		}
	}
}



void Lighting::setPreset(enum LightingPreset preset)
{
	Setup *setup = Setup::get();

	switch (preset)
	{
		case LP_OFF:
			setRGB(0);
			break;

		case LP_MODEL:
			setRGB(setup->lightingIlluminationRGB);
			break;

		case LP_LASER:
			setRGB(setup->lightingLaserRGB);
			break;

		default:
			break;
	}	
}


// Count is number of pixels to set to white
void Lighting::setCount(int count)
{
	Setup *setup = Setup::get();

	if ( setup->enableLighting )
	{
		if ( setup->lightingType == LT_WS281X )
		{
			for (int i = 0; i < WS281X_LED_COUNT; i ++)
				setRGBLed((i < count) ? 0x00FFFFFF : 0, i);
			ws2811_render(&ledstring);
			ws2811_wait(&ledstring);
		}
		if ( setup->lightingType == LT_PWM )
		{
			for (int i = 0; i < count; i ++ )
			{
				setIntensity(100);
				sleep(1);
				setIntensity(0);
				sleep(1);
			}
			sleep(3);
		}
	}
}

}
