//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
#include <assert.h>

namespace Caustic {
	//**********************************************************************
	// Class: BresenhamCircle
	// Defines a class for walking around a circle using Bresenham's algorithm
	//**********************************************************************
	class BresenhamCircle
    {
        int32 x;        // current x coordinate
        int32 y;        // current y coordinate
        int32 cx;
        int32 cy;
        int32 d;
        int32 deltaE;
        int32 deltaSE;
    public:
		//**********************************************************************
		// Constructor: BresenhamCircle
		// Default constructor
		//**********************************************************************
		BresenhamCircle()
        {
        }

		//**********************************************************************
		// Constructor: BresenhamCircle
		// Constructor
		//
		// Parameters:
		// radius - radius of the circle in pixels
		//**********************************************************************
		BresenhamCircle(int32 radius)
        {
            x = 0;
            y = radius;
            d = 1 - radius;
            deltaE = 3;
            deltaSE = -2 * radius + 5;
        }

		//**********************************************************************
		// Method: GetX
		// Returns the current pixel's X coordinate
		//**********************************************************************
		int32 GetX() { return x; }

		//**********************************************************************
		// Method: GetY
		// Returns the current pixel's Y coordinate
		//**********************************************************************
		int32 GetY() { return y; }

		//**********************************************************************
		// Method: end
		// Returns whether the iterator has reached the end
		//**********************************************************************
		bool end()
        {
            return (y <= x);
        }

		//**********************************************************************
		// Method: step
		// Steps the iterator one unit along the circle
		//**********************************************************************
		void step()
        {
            if (d < 0)
            {
                d += deltaE;
                deltaE += 2;
                deltaSE += 2;
                x++;
            }
            else
            {
                d += deltaSE;
                deltaE += 2;
                deltaSE += 4;
                x++;
                y--;
            }
        }
    };
}
