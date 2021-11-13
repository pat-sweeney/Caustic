//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
#include "Base\Math\Quaternion.h"
#include "Base\Math\Matrix.h"

namespace Caustic
{
    Quaternion::Quaternion()
    {
    }

    Quaternion::Quaternion(Matrix4x4 &tm)
    {
        double tr = tm.v[0][0] + tm.v[1][1] + tm.v[2][2];
        if (tr > 0.0)
        {
            float s = (float)sqrt(tr + 1.0F);
            w = s * 0.5F;
            s = 0.5F / s;
            x = (tm.v[1][2] - tm.v[2][1]) * s;
            y = (tm.v[2][0] - tm.v[0][2]) * s;
            z = (tm.v[0][1] - tm.v[1][0]) * s;
        }
        else
        {
            int i = 0;
            if (tm.v[1][1] > tm.v[0][0]) i = 1;
            if (tm.v[2][2] > tm.v[i][i]) i = 2;
            int j = (i == 2) ? 0 : i + 1;
            int k = (j == 2) ? 0 : j + 1;
            float s = (float)sqrt((tm.v[i][i] - (tm.v[j][j] + tm.v[k][k])) + 1.0);
            switch (i)
            {
            case 0:
                x = s * 0.5F;
                s = 0.5F / s;
                w = (tm.v[j][k] - tm.v[k][j]) * s;
                y = (tm.v[i][j] - tm.v[j][i]) * s;
                z = (tm.v[i][k] - tm.v[k][i]) * s;
                break;
            case 1:
                y = s * 0.5F;
                s = 0.5F / s;
                w = (tm.v[j][k] - tm.v[k][j]) * s;
                z = (tm.v[j][k] - tm.v[k][j]) * s;
                x = (tm.v[j][k] - tm.v[k][j]) * s;
                break;
            case 2:
                z = s * 0.5F;
                s = 0.5F / s;
                w = (tm.v[j][k] - tm.v[k][j]) * s;
                x = (tm.v[j][k] - tm.v[k][j]) * s;
                y = (tm.v[j][k] - tm.v[k][j]) * s;
                break;
            }
        }
    }

    Quaternion::Quaternion(Matrix4x3 &tm)
    {
        double tr = tm.v[0][0] + tm.v[1][1] + tm.v[2][2];
        if (tr > 0.0)
        {
            float s = (float)sqrt(tr + 1.0F);
            w = s * 0.5F;
            s = 0.5F / s;
            x = (tm.v[1][2] - tm.v[2][1]) * s;
            y = (tm.v[2][0] - tm.v[0][2]) * s;
            z = (tm.v[0][1] - tm.v[1][0]) * s;
        }
        else
        {
            int i = 0;
            if (tm.v[1][1] > tm.v[0][0]) i = 1;
            if (tm.v[2][2] > tm.v[i][i]) i = 2;
            int j = (i == 2) ? 0 : i + 1;
            int k = (j == 2) ? 0 : j + 1;
            float s = (float)sqrt((tm.v[i][i] - (tm.v[j][j] + tm.v[k][k])) + 1.0);
            switch (i)
            {
            case 0:
                x = s * 0.5F;
                s = 0.5F / s;
                w = (tm.v[j][k] - tm.v[k][j]) * s;
                y = (tm.v[i][j] - tm.v[j][i]) * s;
                z = (tm.v[i][k] - tm.v[k][i]) * s;
                break;
            case 1:
                y = s * 0.5F;
                s = 0.5F / s;
                w = (tm.v[j][k] - tm.v[k][j]) * s;
                z = (tm.v[j][k] - tm.v[k][j]) * s;
                x = (tm.v[j][k] - tm.v[k][j]) * s;
                break;
            case 2:
                z = s * 0.5F;
                s = 0.5F / s;
                w = (tm.v[j][k] - tm.v[k][j]) * s;
                x = (tm.v[j][k] - tm.v[k][j]) * s;
                y = (tm.v[j][k] - tm.v[k][j]) * s;
                break;
            }
        }
    }

    //
    // The following function performs spherical linear interpolation
    // on two quaternions. For more information see:
    //   _Advanced Animation and Rendering Techniques_
    //   Mark Watt & Alan Watt
    //   pg 364
    //
    Quaternion Quaternion::Slerp(Quaternion &p, Quaternion &q, float t)
    {
        Quaternion qt;
        float sclp, sclq;
        float cosom = p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w;
#define EPSILON 0.00001F
#define HALFPI 1.570796326794895F
        if ((1.0 + cosom) > EPSILON)
        {
            if ((1.0 - cosom) > EPSILON)
            {
                float omega = (float)acos(cosom);
                float sinom = (float)sin(omega);
                sclp = (float)sin((1.0 - t) * omega) / sinom;
                sclq = (float)sin(t * omega) / sinom;
            }
            else
            {
                sclp = 1.0F - t;
                sclq = t;
            }
            qt.x = sclp * p.x + sclq * q.x;
            qt.y = sclp * p.y + sclq * q.y;
            qt.z = sclp * p.z + sclq * q.z;
            qt.w = sclp * p.w + sclq * q.w;
        }
        else
        {
            qt.x = -p.y;    qt.y = p.x;    qt.z = -p.w;    qt.w = p.z;
            sclp = (float)sin((1.0F - t) * HALFPI);
            sclq = (float)sin(t * HALFPI);
            qt.x = sclp * p.x + sclq * qt.x;
            qt.y = sclp * p.y + sclq * qt.y;
            qt.z = sclp * p.z + sclq * qt.z;
            qt.w = sclp * p.w + sclq * qt.w;
        }
        return qt;
    }

    Quaternion::Quaternion(Vector3 *vec, float angle)
    {
        float ca = (float)cos((double)angle / 2.0);
        float sa = (float)sin((double)angle / 2.0);

        x = vec->x * sa;
        y = vec->y * sa;
        z = vec->z * sa;
        w = ca;
    }

    //
    // This ctor creates a Quaternion for a list of floats.
    // If 'isvec' is TRUE then _x,_y,_z form a direction
    // vector and _w is the angle of rotation about the direction vector.
    // Otherwise, _x,_y,_z,_w are treated as the four components
    // of a quaternion.
    //
    Quaternion::Quaternion(float _x, float _y, float _z, float _w, bool isvec /*= false*/)
    {
        if (isvec)
        {
            float ca = (float)cos((double)_w / 2.0);
            float sa = (float)sin((double)_w / 2.0);

            x = _x * sa;
            y = _y * sa;
            z = _z * sa;
            w = ca;
        }
        else
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
    }

    Quaternion Quaternion::operator *(Quaternion &q)
    {
        Quaternion r;
        r.x = w * q.x + x * q.w + y * q.z - z * q.y;
        r.y = w * q.y + y * q.w + z * q.x - x * q.z;
        r.z = w * q.z + z * q.w + x * q.y - y * q.x;
        r.w = w * q.w - x * q.x - y * q.y - z * q.z;
        return r;
    }

    Quaternion Quaternion::Conjugate()
    {
        Quaternion nq;
        nq.x = -x;
        nq.y = -y;
        nq.z = -z;
        nq.w = w;
        return nq;
    }
}
