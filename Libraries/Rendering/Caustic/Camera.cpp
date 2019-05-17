//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\Error.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <DirectXMath.h>

namespace Caustic
{
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
    CCamera::~CCamera()
    {
    }

    //**********************************************************************
    DirectX::XMMATRIX CCamera::GetProjection()
    {
        return m_Pers;
    }

    //**********************************************************************
    DirectX::XMMATRIX CCamera::GetView()
    {
        return m_View;
    }

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
    void CCamera::SetOffset(Vector3 &offset)
    {
        m_offset = offset;
    }

	//**********************************************************************
	void CCamera::GetOffset(Vector3 &offset)
    {
        offset = m_offset;
    }

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
