//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Ported from Nestegg Software (Copyright 1995-1997)
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <assert.h>
#include <cinttypes>

export module Geometry.Rast.Bresenham;
import Base.Core.Core;

export namespace Caustic
{
    //**********************************************************************
    // Class: Bresenham
    // Defines a class for walking a line using Bresenham's algorithm
    //
    // Module:
    // {Link:import Geometry.Rast.Bresenham{Geometry/Rast/Bresenham.ixx}}
    //**********************************************************************
    class Bresenham
    {
        int32_t x;        // current x coordinate
        int32_t y;        // current y coordinate
        int32_t dx;        // direction to walk in x
        int32_t dy;        // direction to walk in y
        int32_t ex;        // error in x
        int32_t ey;        // error in y
        int32_t stopx, stopy;    // end of line
        bool swapped;        // slope greater than 1
    public:
        int32_t err;        // total error

        //**********************************************************************
        // Constructor: Bresenham
        // Default constructor
        //**********************************************************************
        Bresenham()
        {
        }

        //**********************************************************************
        // Constructor: Bresenham
        // Constructor
        //
        // Parameters:
        // fx - starting X coordinate of line
        // fy - starting Y coordinate of line
        // tx - ending X coordinate of line
        // ty - ending Y coordinate of line
        //**********************************************************************
        Bresenham(int32_t fx, int32_t fy, int32_t tx, int32_t ty)
        {
            stopx = tx;
            stopy = ty;
            x = fx;
            y = fy;
            ex = tx - fx;
            ey = ty - fy;
            if (ex < 0)
            {
                ex = -ex;
                dx = -1;
            }
            else
                dx = 1;
            if (ey < 0)
            {
                ey = -ey;
                dy = -1;
            }
            else
                dy = 1;
            if (ey > ex)
            {
                ex ^= ey ^= ex ^= ey; // swap
                swapped = true;
            }
            else
                swapped = false;
            err = (ey << 1) - ex;
            ex <<= 1;
            ey <<= 1;
        }

        //**********************************************************************
        // Method: get_x
        // Returns the X coordinate where the iterator is currently positioned
        //**********************************************************************
        int32_t get_x() { return x; }

        //**********************************************************************
        // Method: get_y
        // Returns the Y coordinate where the iterator is currently positioned
        //**********************************************************************
        int32_t get_y() { return y; }

        //**********************************************************************
        // Method: get_endx
        // Returns the ending pixel's X coordinate
        //**********************************************************************
        int32_t get_endx() { return stopx; }

        //**********************************************************************
        // Method: get_endy
        // Returns the ending pixel's Y coordinate
        //**********************************************************************
        int32_t get_endy() { return stopy; }

        //**********************************************************************
        // Method: eol
        // Returns true if iterator has reached end of line. False otherwise.
        //**********************************************************************
        bool eol()
        {
            return (stopx == get_x() && stopy == get_y());
        }

        //**********************************************************************
        // Method: step
        // Steps one unit along the line
        //**********************************************************************
        void step()
        {
            if (swapped)
            {
                if (err > 0 || (err == 0 && dx > 0))
                {
                    x += dx;
                    err -= ex;
                }
                y += dy;
                err += ey;
            }
            else
            {
                if (err > 0 || (err == 0 && dy > 0))
                {
                    y += dy;
                    err -= ex;
                }
                x += dx;
                err += ey;
            }
            return;
        }

        //**********************************************************************
        // Method: step_in_x
        // Returns if a call to step() will result in a step in the X direction
        //**********************************************************************
        bool step_in_x()
        {
            if (swapped)
                return (err > 0 || (err == 0 && dx > 0)) ? true : false;
            return true;
        }

        //**********************************************************************
        // Method: step_in_y
        // Returns if a call to step() will result in a step in the Y direction
        //**********************************************************************
        bool step_in_y()
        {
            if (swapped)
                return true;
            return (err > 0 || (err == 0 && dy > 0)) ? true : false;
        }

        //**********************************************************************
        // Method: step_in_minor
        // Determines whether we are about to step along the minor axis.
        //**********************************************************************
        bool step_in_minor()
        {
            return ((swapped) ?
                ((err > 0 || (err == 0 && dx > 0)) ? true : false) :
                ((err > 0 || (err == 0 && dy > 0)) ? true : false));
        }

        //**********************************************************************
        // Method: step_x
        // Moves the iterator 'dx' units along the x axis
        //
        // Parameters:
        // dx - Number of units to step along the x axis
        //**********************************************************************
        void step_x(int32_t dx)
        {
            int32_t _dy;

            assert(!swapped);
            assert(ex >= 0);
            assert(ey >= 0);
            err += (ey * dx);
            if (ex != 0)
            {
                _dy = (err / ex);
                err -= (_dy * ex);
                if ((_dy < 0 && dy > 0) || (_dy > 0 && dy < 0))
                    _dy = -_dy;
                if (dx < 0)
                {
                    int32_t _diff;

                    _diff = ey - ex;
                    if (err < _diff || (err == _diff && dy > 0))
                    {
                        err += ex;
                        _dy += dy;
                    }
                }
                else
                {
                    if (((err > ey) || (err == ey && dy > 0)))
                    {
                        err -= ex;
                        _dy += dy;
                    }
                }
                y += _dy;
            }
            x += dx;
            return;
        }

        //**********************************************************************
        // Method: step_y
        // Moves the iterator 'dy' units along the y axis
        //
        // Parameters:
        // dy - Number of units to step along the y axis
        //**********************************************************************
        void step_y(int32_t dy)
        {
            int32_t _dx;

            assert(swapped);
            assert(ex >= 0);
            assert(ey >= 0);
            err += (ey * dy);
            if (ex != 0)
            {
                _dx = (err / ex);
                err -= (_dx * ex);
                if ((_dx < 0 && dx > 0) || (_dx > 0 && dx < 0))
                    _dx = -_dx;
                if (dy < 0)
                {
                    int32_t _diff;

                    _diff = ey - ex;
                    if (err < _diff || (err == _diff && dx > 0))
                    {
                        err += ex;
                        _dx += dx;
                    }
                }
                else
                {
                    if (((err > ey) || (err == ey && dx > 0)))
                    {
                        err -= ex;
                        _dx += dx;
                    }
                }
                x += _dx;
            }
            y += dy;
        }

        //**********************************************************************
        // Method: step_inc
        // Moves 'delta' units along the major axis
        //
        // Parameters:
        // delta - Number of units to step along the major axis
        //**********************************************************************
        void step_inc(int32_t delta)
        {
            if (swapped)
                step_y(delta);
            else
                step_x(delta);
            return;
        }
    };
}
