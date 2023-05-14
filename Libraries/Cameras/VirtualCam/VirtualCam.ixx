//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <string>
#include <mfvirtualcamera.h>
#include <atlbase.h>

export module Cameras.VirtualCamera.VirtualCamera;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Cameras.VirtualCamera.IVirtualCamera;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Interface: IVirtualCam
    // Defines the interface for a virtual camera
    //
    // Module:
    // {Link:import Cameras.VirtualCam.IVirtualCam;{Cameras/VirtualCam/IVirtualCam.ixx}}
    //**********************************************************************
    class CVirtualCamera : public IVirtualCamera, public CRefCount
    {
        HANDLE m_hFrameReady;
        HANDLE m_hFrameMutex;
        HANDLE m_hFileMapping;
        BYTE* m_pSharedMemory;
    public:
        CVirtualCamera();
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IVirtualCamera
        //**********************************************************************
        virtual void SendFrame(IImage* pTexture) override;
    };

    CRefObj<IVirtualCamera> CreateVirtualCamera()
    {
        return CRefObj<IVirtualCamera>(new CVirtualCamera());
    }
}
