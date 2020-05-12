//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\Error.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <DirectXMath.h>

//**********************************************************************
// File: Camera.cpp
// Contains the methods used to implement the <CCamera> object.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateCamera
    // Global function for creating a camera. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Parameters:
    // leftHanded - indicates whether we are creating a left handed coordinate system
    //    or a right handed coordinates system.
    //
    // Returns:
    // Returns the newly created camera.
    //**********************************************************************
    CAUSTICAPI CRefObj<ICamera> CreateCamera(bool leftHanded)
    {
        return CRefObj<ICamera>(new CCamera(leftHanded));
    }

    //**********************************************************************
    // Method: CCamera
    // Defines the implementation of <ICamera>
    //
    // Parameters:
    // leftHanded - indicates we should create a camera using Left handed coordinates
    //      instead of right handed coordinates.
    //**********************************************************************
    CCamera::CCamera(bool leftHanded) :
        m_leftHanded(leftHanded)
    {
        m_offset = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 eye(0, 0, -10.0f);
        Vector3 look(0.0f, 0.0f, 0.0f);
        Vector3 up(0.0f, 1.0f, 0.0f);
        SetPosition(eye, look, up);
        SetParams(60.0f * (float)M_PI / 180.0f, 1.0f, 0.01f, 10000.0f);
    }

    //**********************************************************************
    // Method: ~CCamera
    // Implements the dtor for CCamera
    //**********************************************************************
    CCamera::~CCamera()
    {
    }

    //**********************************************************************
    // Method: GetProjection
    // See <ICamera::GetProjection>
    //**********************************************************************
    DirectX::XMMATRIX CCamera::GetProjection()
    {
        return m_Pers;
    }

    //**********************************************************************
    // Method: GetView
    // See <ICamera::GetView>
    //**********************************************************************
    DirectX::XMMATRIX CCamera::GetView()
    {
        return m_View;
    }

	//**********************************************************************
    // Method: GetUVN
    // See <ICamera::GetUVN>
    //**********************************************************************
    void CCamera::GetUVN(Vector3 *u, Vector3 *v, Vector3 *n)
    {
        if (u)
        {
            u->x = DirectX::XMVectorGetX(m_View.r[0]);
            u->y = DirectX::XMVectorGetY(m_View.r[0]);
            u->z = DirectX::XMVectorGetZ(m_View.r[0]);
            *u = u->Normalize();
        }
        if (v)
        {
            v->x = DirectX::XMVectorGetX(m_View.r[1]);
            v->y = DirectX::XMVectorGetY(m_View.r[1]);
            v->z = DirectX::XMVectorGetZ(m_View.r[1]);
            *v = v->Normalize();
        }
        if (n)
        {
            n->x = DirectX::XMVectorGetX(m_View.r[2]);
            n->y = DirectX::XMVectorGetY(m_View.r[2]);
            n->z = DirectX::XMVectorGetZ(m_View.r[2]);
            *n = n->Normalize();
        }
    }

	//**********************************************************************
    // Method: SetOffset
    // See <ICamera::SetOffset>
    //**********************************************************************
    void CCamera::SetOffset(Vector3 &offset)
    {
        m_offset = offset;
    }

	//**********************************************************************
    // Method: GetOffset
    // See <ICamera::GetOffset>
    //**********************************************************************
    void CCamera::GetOffset(Vector3 &offset)
    {
        offset = m_offset;
    }

    //**********************************************************************
    // Method: SetParams
    // See <ICamera::SetParams>
    //**********************************************************************
    void CCamera::SetParams(float fov, float aspectRatio, float nearZ, float farZ)
    {
        m_FOV = fov;
        m_AspectRatio = aspectRatio;
        m_NearZ = nearZ;
        m_FarZ = farZ;
        if (m_leftHanded)
            m_Pers = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);
        else
            m_Pers = DirectX::XMMatrixPerspectiveFovRH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);
    }

    //**********************************************************************
    // Method: SetPosition
    // See <ICamera::SetPosition>
    //**********************************************************************
    void CCamera::SetPosition(Vector3 &eye, Vector3 &look, Vector3 &up)
    {
        if ((look - eye).Length() < 0.001)
            return;
        m_Eye = eye;
        m_Look = look;
        m_Up = up;
        DirectX::XMVECTOR vEye = DirectX::XMVectorSet(eye.x + m_offset.x, eye.y + m_offset.y, eye.z + m_offset.z, 0.0f);
        DirectX::XMVECTOR vLook = DirectX::XMVectorSet(look.x + m_offset.x, look.y + m_offset.y, look.z + m_offset.z, 0.0f);
        DirectX::XMVECTOR vUp = DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f);
        if (m_leftHanded)
            m_View = DirectX::XMMatrixLookAtLH(vEye, vLook, vUp);
        else
            m_View = DirectX::XMMatrixLookAtRH(vEye, vLook, vUp);
    }

    //**********************************************************************
    // Method: GetPosition
    // See <ICamera::GetPosition>
    //**********************************************************************
    void CCamera::GetPosition(Vector3 *pEye, Vector3 *pLook, Vector3 *pUp, Vector3 *pU, Vector3 *pV, Vector3 *pN)
    {
        if (pEye)
            *pEye = m_Eye;
        if (pLook)
            *pLook = m_Look;
        if (pUp)
            *pUp = m_Up;
        if (pU)
        {
            pU->x = DirectX::XMVectorGetX(m_View.r[0]);
            pU->y = DirectX::XMVectorGetX(m_View.r[1]);
            pU->z = DirectX::XMVectorGetX(m_View.r[2]);
        }
        if (pV)
        {
            pV->x = DirectX::XMVectorGetY(m_View.r[0]);
            pV->y = DirectX::XMVectorGetY(m_View.r[1]);
            pV->z = DirectX::XMVectorGetY(m_View.r[2]);
        }
        if (pN)
        {
            pN->x = DirectX::XMVectorGetZ(m_View.r[0]);
            pN->y = DirectX::XMVectorGetZ(m_View.r[1]);
            pN->z = DirectX::XMVectorGetZ(m_View.r[2]);
        }
    }

	//**********************************************************************
    // Method: Load
    // See <ISerialize::Load>
    //**********************************************************************
    void CCamera::Load(IStream *pStream)
    {
        ULONG bytesRead;
        CT(pStream->Read(&m_leftHanded, sizeof(m_leftHanded), &bytesRead));
        CT(pStream->Read(&m_FOV, sizeof(m_FOV), &bytesRead));
        CT(pStream->Read(&m_AspectRatio, sizeof(m_AspectRatio), &bytesRead));
        CT(pStream->Read(&m_NearZ, sizeof(m_NearZ), &bytesRead));
        CT(pStream->Read(&m_FarZ, sizeof(m_FarZ), &bytesRead));
        CT(pStream->Read(&m_Eye, sizeof(m_Eye), &bytesRead));
        CT(pStream->Read(&m_Look, sizeof(m_Look), &bytesRead));
        CT(pStream->Read(&m_Up, sizeof(m_Up), &bytesRead));
        CT(pStream->Read(&m_offset, sizeof(m_offset), &bytesRead));
    }

	//**********************************************************************
    // Method: Store
    // See <ISerialize::Store>
    //**********************************************************************
    void CCamera::Store(IStream *pStream)
    {
        ULONG bytesWritten;
        CT(pStream->Write(&m_leftHanded, sizeof(m_leftHanded), &bytesWritten));
        CT(pStream->Write(&m_FOV, sizeof(m_FOV), &bytesWritten));
        CT(pStream->Write(&m_AspectRatio, sizeof(m_AspectRatio), &bytesWritten));
        CT(pStream->Write(&m_NearZ, sizeof(m_NearZ), &bytesWritten));
        CT(pStream->Write(&m_FarZ, sizeof(m_FarZ), &bytesWritten));
        CT(pStream->Write(&m_Eye, sizeof(m_Eye), &bytesWritten));
        CT(pStream->Write(&m_Look, sizeof(m_Look), &bytesWritten));
        CT(pStream->Write(&m_Up, sizeof(m_Up), &bytesWritten));
        CT(pStream->Write(&m_offset, sizeof(m_offset), &bytesWritten));
    }
}
