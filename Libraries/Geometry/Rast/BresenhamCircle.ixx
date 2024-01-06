//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <assert.h>
#include <cinttypes>

export module Geometry.Rast.BresenhamCircle;
import Base.Core.Core;

export namespace Caustic
{
    //**********************************************************************
    // Class: BresenhamCircle
    // Defines a class for walking around a circle using Bresenham's algorithm
    //
    // Module:
    // {Link:import Geometry.Rast.BresenhamCircle{Geometry/Rast/BresenhamCircle.ixx}}
    //**********************************************************************
    class BresenhamCircle
    {
        int32_t x;        // current x coordinate
        int32_t y;        // current y coordinate
        int32_t cx;
        int32_t cy;
        int32_t d;
        int32_t deltaE;
        int32_t deltaSE;
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
        BresenhamCircle(int32_t radius)
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
        int32_t GetX() { return x; }

        //**********************************************************************
        // Method: GetY
        // Returns the current pixel's Y coordinate
        //**********************************************************************
        int32_t GetY() { return y; }

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
