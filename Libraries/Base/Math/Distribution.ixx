//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>

export module Base.Math.Distributions;
import Base.Core.Core;
import Base.Math.Vector;

// Namespace: Caustic
export namespace Caustic
{
    //*******************************************************************
    // Class: GaussianDistribution
    // Creates a gaussian distrbution based on the sigma passed to the ctor
    // The domain of the distrbution is expected to run from -1..+1
    //**********************************************************************
    struct GaussianDistribution
    {
        float m_sigma;
        double* m_weights;
        int m_kernelWidth;

        GaussianDistribution(float sigma)
        {
            m_sigma = sigma;
            ComputeWeights();
        }

        void ComputeWeights()
        {
            // First compute kernel
            // A normal distribution is defined by:
            //
            //    f(x|u,s^2) = e^(-(x-u)^2/(2*s*s)) / sqrt(2*s*s*PI)
            // u = average, which is zero since we are centered at 0
            // s = sigma
            // We also know that 255*e^-5.541 ~= 1. Thus we can solve for s such that whatever is our maximum
            // x (i.e. the width of our kernel) will result in a value of 1 in our kernel weight.
            float magic = 5.541f;
            int kw = (int)(sqrtf(magic * 2 * m_sigma * m_sigma));
            m_kernelWidth = kw * 2 + 1;
            m_weights = new double[m_kernelWidth];
            for (int i = -kw; i <= kw; i++)
            {
                double x = (double)i;
                double e = exp(-(x * x) / (2 * m_sigma * m_sigma));
                m_weights[i + kw] = e / sqrt(2 * m_sigma * m_sigma * 3.1415926535f);
            }
        }

        //**********************************************************************
        // Method: Sample
        // Samples the distribution. The distribution domain is from -1..+1.
        //
        // Parameters:
        // t - domain value on the distrbution where to sample from
        //**********************************************************************
        float Sample(float t)
        {
            t = (t + 1.0f) / 2.0f;
            int index = (int)(t * m_kernelWidth);
            return (float)m_weights[index];
        }
    };
}
