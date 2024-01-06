//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <DirectXMath.h>

export module Rendering.Caustic.Camera;
import Base.Core.Core;
import Base.Core.RefCount;
import Rendering.Caustic.ICamera;

//**********************************************************************
// File: Camera.ixx
// Contains the declaration for the <CCamera> class.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CCamera
    // Implements <ICamera>
    //
    // Members:
    // m_leftHanded - Indicates if camera uses left or right handed coordinates
    // m_FOV - Field of view in radians
    // m_AspectRation - Aspect ratio of the output camera
    // m_NearZ - Near clipping plane in meters
    // m_FarZ - Far clipping plane in meters
    // m_Eye - Position of camera in world coordinates (in meters)
    // m_Look - Point camera is looking at in world coordinates (in meters)
    // m_Up - Up vector
    // m_offset - Additional offset added to camera's position (in meters)
    // m_Pers - Current perspective matrix
    // m_View - Current view matrix (world to camera transform)
    //
    // Module:
    // {Link:import Rendering.Caustic.Camera;{Rendering/Caustic/Camera.ixx}}
    //**********************************************************************
    _declspec(align(16)) class CCamera : public ICamera, public CRefCount
    {
    protected:
        bool m_leftHanded;        // Indicates if camera uses left or right handed coordinates
        float m_FOV;              // Field of view in radians
        float m_AspectRatio;      // Aspect ratio of the output camera
        float m_NearZ;            // Near clipping plane in meters
        float m_FarZ;             // Far clipping plane in meters
        Vector3 m_Eye;
        Vector3 m_Look;
        Vector3 m_Up;
        Vector3 m_offset;
        DirectX::XMMATRIX m_Pers; // Our perspective matrix
        DirectX::XMMATRIX m_View; // Our view matrix (world to camera transform)
    public:
        CCamera(bool leftHanded);
        ~CCamera();
        
        void* operator new(size_t i)
        {
            return _mm_malloc(i, 16);
        }

        void operator delete(void* p)
        {
            _mm_free(p);
        }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }
        
        //**********************************************************************
        // ICamera
        //**********************************************************************
        virtual void BuildMatrices() override;
        virtual void SetParams(float fov, float aspectRatio, float nearZ, float farZ) override;
        virtual void GetParams(float* fov, float* aspectRatio, float* nearZ, float* farZ) override;
        virtual void SetPosition(const Vector3 &eye, const Vector3 &look, const Vector3 &up) override;
        virtual void GetPosition(Vector3 *pEye, Vector3 *pLook, Vector3 *pUp, Vector3 *pU, Vector3 *pV, Vector3 *pN) override;
        virtual void SetOffset(Vector3 &offset) override;
        virtual void GetOffset(Vector3 &offset) override;
        virtual DirectX::XMMATRIX GetProjection() override;
        virtual DirectX::XMMATRIX GetView() override;
        virtual void SetView(DirectX::XMMATRIX& mat) override;
        virtual void SetProjection(DirectX::XMMATRIX& mat) override;
        virtual void GetUVN(Vector3 *u, Vector3 *v, Vector3 *n) override;
        virtual float GetNear() override { return m_NearZ; }
        virtual float GetFar() override { return m_FarZ; }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;
    };

    CRefObj<ICamera> CreateCameraInternal(bool leftHanded);
};
