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

#pragma once

#define WS281X_LED_COUNT	12
#define WS281X_DMA		5
#define WS281X_TARGET_FREQ	800000
#define WS281X_STRIP_TYPE	WS2811_STRIP_GRB

namespace freelss
{

/**
 * Singleton class for controlling the lighting system
 */
class Lighting
{
public:
	/** Returns the singleton instance */
	static Lighting * get();

	/** Releases the singleton instance */
	static void release();

	Lighting();

	/** 0 (off) to 100 (full intensity) */
	void setIntensity(int intensity);

	int getIntensity() const;

	// 0x00RRGGBB
	void setRGB(uint32_t rgb);

	// Count is number of pixels to set to white
	void setCount(int count);
private:
	static Lighting * m_instance;
	int m_pin;
	int m_intensity;
};

}
