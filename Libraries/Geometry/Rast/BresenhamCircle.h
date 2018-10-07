//**********************************************************************
// Copyright CausticPixel LLC 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include <assert.h>

namespace Caustic {
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
        BresenhamCircle()
        {
        }

        BresenhamCircle(int32 radius)
        {
            x = 0;
            y = radius;
            d = 1 - radius;
            deltaE = 3;
            deltaSE = -2 * radius + 5;
        }

        int32 GetX() { return x; }
        int32 GetY() { return y; }

        bool end()
        {
            return (y <= x);
        }

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
