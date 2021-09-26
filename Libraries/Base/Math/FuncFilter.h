//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Caustic
{
    //**********************************************************************
    // Class: MovingAverage
    // Filters data points using a simple moving average.
    //**********************************************************************
    class MovingAverage
    {
        std::vector<float> m_data;
        int m_index;
        float m_sum;
        int m_windowSize;
        int m_count;
    public:
        //**********************************************************************
        // Ctor: MovingAverage
        // Filters data points using a simple moving average.
        //
        // Parameters:
        // windowSize - size of sliding window (in samples)
        //**********************************************************************
        MovingAverage(int windowSize) : m_index(0), m_sum(0.0f), m_windowSize(windowSize), m_count(0)
        {
            m_data.resize(windowSize, 0.0f);
        }

        //**********************************************************************
        // Method: Sample
        // Filters the specified sample
        //
        // Parameters:
        // v - sample to filter
        //
        // Returns:
        // New filtered sample value
        //**********************************************************************
        float Sample(float v)
        {
            m_index = (m_index + 1) % m_windowSize;
            m_sum = m_sum - m_data[m_index] + v;
            m_data[m_index] = v;
            m_count++;
            if (m_count > m_windowSize)
                m_count = m_windowSize;
            return m_sum / m_count;
        }
    };
    
    //**********************************************************************
    // Class: LowPassFilter
    // Performs low pass filtering on a set of samples (using an exponential filter)
    //**********************************************************************
    class LowPassFilter
    {
        float m_last;
        float m_alpha;
    public:
        //**********************************************************************
        // Ctor: LowPassFilter
        // Filters a sample using an low pass filter
        //
        // Parameters:
        // alpha - exponential weight to blend between previous sample and current sample
        //**********************************************************************
        LowPassFilter(float alpha) : m_alpha(alpha), m_last(0.0f)
        {
        }

        //**********************************************************************
        // Method: Sample
        // Filters the specified sample
        //
        // Parameters:
        // v - sample to filter
        //
        // Returns:
        // New filtered sample value
        //**********************************************************************
        float Sample(float v)
        {
            m_last = m_alpha * v + (1 - m_alpha) * m_last;
            return m_last;
        }
    };

    //**********************************************************************
    // Class: DoubleExponentialFilter
    // Performs filtering on a set of samples using an double exponential filter
    // (where we blend both the derivative and the value)
    //**********************************************************************
    class DoubleExponentialFilter
    {
        float m_lastLevel;
        float m_lastTrend;
        float m_alpha;
        float m_gamma;
    public:
        //**********************************************************************
        // Ctor: DoubleExponentialFilter
        // Filters a sample using an double exponential filter
        //
        // Parameters:
        // alpha - exponential weight to blend between previous derivative and current derivative
        // gamma - exponential weight to blend between previous sample and current sample
        //**********************************************************************
        DoubleExponentialFilter(float alpha, float gamma) : 
            m_alpha(alpha), 
            m_gamma(gamma), 
            m_lastLevel(0.0f), 
            m_lastTrend(0.0f)
        {
        }

        //**********************************************************************
        // Method: Sample
        // Filters the specified sample
        //
        // Parameters:
        // v - sample to filter
        //
        // Returns:
        // New filtered sample value
        //**********************************************************************
        float Sample(float v)
        {
            float level = m_alpha * v + (1 - m_alpha) * m_lastLevel;
            float trend = m_gamma * level + (1 - m_gamma) * m_lastTrend;
            m_lastLevel = level;
            m_lastTrend = trend;
            return 2 * m_lastLevel - m_lastTrend;
        }
    };

    //**********************************************************************
    // Class: OneEulerFilter
    // Performs filtering on a set of samples using the 1-Euro filter.
    // See https://cristal.univ-lille.fr/~casiez/1euro/ for details.
    //**********************************************************************
    class OneEulerFilter
    {
        bool m_initialized;
        float m_beta;
        float m_lastV;
        float m_lastDX;
        float m_rate;
        float m_mincutoff;
        float m_dcutoff;
        float m_dxAlpha;
    public:
        //**********************************************************************
        // Ctor: OneEulerFilter
        // Filters using the 1-Euro filter
        //
        // Parameters:
        // rate - frequency of our input data
        // mincutoff - 
        // beta -
        // dcutoff -
        //**********************************************************************
        OneEulerFilter(float rate, float mincutoff, float beta, float dcutoff) :
            m_initialized(false),
            m_rate(rate),
            m_mincutoff(mincutoff),
            m_beta(beta),
            m_dcutoff(dcutoff)
        {
            float tau = 1.0f / (2.0f * float(M_PI) * m_dcutoff);
            m_dxAlpha = 1.0f / (1.0f + tau * m_rate);
        }
        
        //**********************************************************************
        // Method: Sample
        // Filters the specified sample
        //
        // Parameters:
        // v - sample to filter
        //
        // Returns:
        // New filtered sample value
        //**********************************************************************
        float Sample(float v)
        {
            float dx;
            if (!m_initialized)
            {
                m_initialized = true;
                dx = 0.0f;
            }
            else
            {
                dx = (v - m_lastV) * m_rate;
            }
            m_lastDX = m_dxAlpha * dx + (1.0f - m_dxAlpha) * m_lastDX;

            float cutoff = float(m_mincutoff + m_beta * fabs(m_lastDX));
            float tau = 1.0f / (2.0f * float(M_PI) * cutoff);
            float te = 1.0f / m_rate;
            float alpha = 1.0f / (1.0f + tau / te);
            m_lastV = alpha * v + (1.0f - alpha) * m_lastV;
            return m_lastV;
        }
    };
}