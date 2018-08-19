//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Trackball.h"
#include <algorithm>

namespace Caustic
{
    //**********************************************************************
    //! \brief Creates a new trackball object
    //! \param[out] ppTrackball Returns the created trackball
    //**********************************************************************
    CAUSTICAPI void CreateTrackball(ITrackball **ppTrackball)
    {
        std::unique_ptr<CTrackball> spTrackball(new CTrackball());
        *ppTrackball = spTrackball.release();
        (*ppTrackball)->AddRef();
    }

    //**********************************************************************
    //! \brief Converts a screen coordinate into a point on our trackball surface
    //! \param[in] screenX X position of mouse in screen coordinates
    //! \param[in] screenY Y position of mouse in screen coordinates
    //! \param[out] pPos Returns the position of the screen coordinate on the trackball surface
    //**********************************************************************
    void CTrackball::ComputeSurfacePosition(int screenX, int screenY, Vector3 *pPos)
    {
        int xp = m_centerX - screenX;
        int yp = screenY - m_centerY;
        float zp = 0.0f;
        int xp2 = xp * xp;
        int yp2 = yp * yp;
        int r2 = m_radius * m_radius;
        int hr2 = r2 /2;
        if (xp2 + yp2 > hr2)
        {
            // In hyperbolic sheet area
            zp = ((float)hr2 / (float)sqrt(xp2 + yp2));
        }
        else
        {
            // In sphere area
            zp = (float)sqrt(r2 - xp2 - yp2);
        }
        *pPos = Vector3((float)xp, (float)yp, zp);
    }

    //**********************************************************************
    //! \brief Called at start of mouse drag
    //! \param[in] screenX X position of mouse in screen coordinates
    //! \param[in] screenY Y position of mouse in screen coordinates
    //! \param[in] screenW Width of screen in pixels
    //! \param[in] screenH Height of screen in pixels
    //**********************************************************************
    void CTrackball::BeginTracking(int screenX, int screenY, int screenW, int screenH)
    {
        m_radius = std::min(screenW, screenH) / 2;
        m_width = screenW;
        m_height = screenH;
        m_startX = screenX;
        m_startY = screenY;
        m_centerX = m_width / 2;
        m_centerY = m_height / 2;
        ComputeSurfacePosition(m_startX, m_startY, &m_startPos);
    }

    //**********************************************************************
    //! \brief Called each time mouse is moved
    //! \param[in] constraint Constrain rotation to be about specific axis
    //! \param[in] screenX X position of mouse in screen coordinates
    //! \param[in] screenY Y position of mouse in screen coordinates
    //**********************************************************************
    bool CTrackball::UpdateTracking(int screenX, int screenY, ETrackballConstrain constraint, DirectX::XMMATRIX *pMatrix)
    {
        Vector3 pos;
        ComputeSurfacePosition(screenX, screenY, &pos);

        // Compute quaternion
        DirectX::XMVECTOR v1 = DirectX::XMVector3Normalize(DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f));
        DirectX::XMVECTOR v2 = DirectX::XMVector3Normalize(DirectX::XMVectorSet(m_startPos.x, m_startPos.y, m_startPos.z, 0.0f));
        DirectX::XMVECTOR cp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(v1, v2));
        if (DirectX::XMVectorGetX(DirectX::XMVector3Length(cp)) == 0.0)
            return false;
        cp = DirectX::XMVectorSetW(cp, 0.0f);
        float angle = 4.0f * (float)acos(DirectX::XMVectorGetX(DirectX::XMVector3Dot(v1, v2)));
        if (DirectX::XMVectorGetZ(cp) < 0)
            cp = DirectX::XMVectorSet(DirectX::XMVectorGetX(cp), DirectX::XMVectorGetY(cp), -DirectX::XMVectorGetZ(cp), DirectX::XMVectorGetW(cp));

        DirectX::XMVECTOR axis;
        switch (constraint)
        {
        case ETrackballConstrain::Constraint_None:
            axis = cp;
            break;
        case ETrackballConstrain::Constraint_XAxis:
            axis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
            break;
        case ETrackballConstrain::Constraint_YAxis:
            axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            break;
        case ETrackballConstrain::Constraint_ZAxis:
            axis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
            break;
        default:
            _ASSERT(false);
            axis = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
            break;
        }

        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(axis, angle);
        *pMatrix = DirectX::XMMatrixRotationQuaternion(q);
        return true;
    }

    //**********************************************************************
    //! \brief Called at end of mouse drage
    //**********************************************************************
    void CTrackball::EndTracking()
    {
    }
};
