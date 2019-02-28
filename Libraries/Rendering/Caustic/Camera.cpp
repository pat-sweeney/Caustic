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
    // Function: CreateCamera
    //! \brief CreateCamera returns a new ICamera object
    //!
    //! This method creates a new camera object. This camera can then
    //! be passed to IRenderer to be used for rendering.
    //! \param[in] leftHanded Defines whether we have a left or right handed coordinate system
    //! \param[out] ppCamera Returns the newly created camera object
    //!
    //**********************************************************************
    CAUSTICAPI void CreateCamera(bool leftHanded, ICamera **ppCamera)
    {
        std::unique_ptr<CCamera> pCamera(new CCamera(leftHanded));
        *ppCamera = pCamera.release();
        (*ppCamera)->AddRef();
    }

    //**********************************************************************
    //! \brief Defines our camera object
    //!
    //! This is the ctor for CCamera.
    //! \param[in] leftHanded Defines whether we have a left or right handed coordinate system
    //!
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
    //! \brief Defines the dtor for CCamera
    //**********************************************************************
    CCamera::~CCamera()
    {
    }

    //**********************************************************************
    //! \brief Returns the camera's projection matrix
    //**********************************************************************
    DirectX::XMMATRIX CCamera::GetProjection()
    {
        return m_Pers;
    }

    //**********************************************************************
    //! \brief Returns the camera's view matrix
    //**********************************************************************
    DirectX::XMMATRIX CCamera::GetView()
    {
        return m_View;
    }

    //**********************************************************************
    //! \brief Returns the camera's U,V,N axis in world coordinates
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
    //! \brief Defines an offset to apply to the camera's eye and look position
    //**********************************************************************
    void CCamera::SetOffset(Vector3 &offset)
    {
        m_offset = offset;
    }

    //**********************************************************************
    //! \brief Defines an offset to apply to the camera's eye and look position
    //**********************************************************************
    void CCamera::GetOffset(Vector3 &offset)
    {
        offset = m_offset;
    }

    //**********************************************************************
    //! \brief Set parameters on the camera
    //! \param[in] fov Field of view in radians
    //! \param[in] aspectRatio Aspect ratio of our window
    //! \param[in] nearZ Near plane in millimeters
    //! \param[in] farZ Far plane in millimeters
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
    //! \brief Sets the camera's position, look point, and orientation
    //! \param[in] eye Position of camera in world coordinates (millimeters)
    //! \param[in] look Position in world coordinates that camera is looking at
    //! \param[in] up Up vector in world coordinates
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
    //! \brief Returns the camera's current position, look point, and orientation
    //! \param[out] pEye Position of camera in world coordinates (millimeters). May be nullptr.
    //! \param[out] pLook Position in world coordinates that camera is looking at. May be nullptr.
    //! \param[out] pUp Up vector in world coordinates. May be nullptr.
    //! \param[out] pU Camera's right vector (in world coordinates). May be nullptr.
    //! \param[out] pV Camera's up vector (in world coordinates). May be nullptr.
    //! \param[out] pN Camera's forward vector (in world coordinates). May be nullptr.
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
