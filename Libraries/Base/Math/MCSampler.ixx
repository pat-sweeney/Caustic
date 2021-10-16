//**********************************************************************
// Copyright Patrick Sweeney 2021
// All Rights Reserved
//**********************************************************************
module;
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <cstdlib>
#include <algorithm>

export module Base.Math.MCSampler;
import Base.Core.Core;
import Base.Math.Vector;
import Base.Math.Ray;

export namespace Caustic
{
	// Define a base class for Monte Carlo sampling
	struct CMCSampler2D
	{
        virtual Vector2 Next() = 0;
    };

    class CRandomSampler2D : public CMCSampler2D
    {
        int m_w;
        int m_h;
    public:
        CRandomSampler2D(int w, int h)
        {
            m_w = w;
            m_h = h;
        }

        virtual Vector2 Next() override
        {
            return Vector2(m_w * float(rand()) / float(RAND_MAX), m_h * float(rand()) / float(RAND_MAX));
        }
    };

    class CUniformSampler2D : public CMCSampler2D
    {
        int m_curx;
        int m_cury;
        int m_w;
        int m_h;
    public:
        CUniformSampler2D(int w, int h)
        {
            m_curx = 0;
            m_cury = 0;
            m_w = w;
            m_h = h;
        }

        virtual Vector2 Next() override
        {
            m_curx++;
            if (m_curx == m_w)
            {
                m_curx = 0;
                m_cury++;
            }
            return Vector2(float(m_curx), float(m_cury));
        }
    };

    class CJitteredSampler2D : public CUniformSampler2D
    {
    public:
        virtual Vector2 Next() override
        {
            Vector2 pos = CUniformSampler2D::Next();
            pos.x = pos.x + ((float(rand()) / float(RAND_MAX)) - 0.5f);
            pos.y = pos.y + ((float(rand()) / float(RAND_MAX)) - 0.5f);
            return pos;
        }
    };

    struct CMCSampler3D
    {
        virtual Vector3 Next() = 0;
    };

    class CRandomHemisphere3D : public CMCSampler3D
    {
    public:
        virtual Vector3 Next() override
        {
            Vector3 pos;
            float theta = float(rand()) / float(RAND_MAX) * 2.0f * float(M_PI) - float(M_PI);
            float phi = float(rand()) / float(RAND_MAX) * float(M_PI);
            pos.x = sinf(theta) * cosf(phi);
            pos.y = sinf(theta) * sinf(phi);
            pos.z = cosf(theta);
            return pos;
        }
    };

    class CUniformHemisphere3D : public CMCSampler3D
    {
    public:
        virtual Vector3 Next() override
        {
            // For complete breakdown of hos this code is derived, see:
            // Physically Based Rendering by Pharr, et al.
            // Section 14.5.1 (Uniformly Sampling a Hemisphere)
            Vector3 pos;
            pos.z = float(rand()) / float(RAND_MAX);
            float r = sqrtf(std::max<float>(0.0f, 1.0f - pos.z * pos.z));
            float theta = float(rand()) / float(RAND_MAX) * 2.0f * float(M_PI);
            pos.x = r * cosf(theta);
            pos.y = r * sinf(theta);
            return pos;
        }
    };
}
