//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <string>
#include <memory>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <winnt.h>
#include <atlbase.h>

export module Cameras.WebCamera.WebCamera;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;
import Cameras.WebCamera.IWebCamera;
import Imaging.Image.Image;
import Imaging.Image.IImagePool;

export namespace Caustic
{
    //**********************************************************************
    // Class: CWebCamera
    // Defines the implementation for a WebCamera.
    //
    // Members:
    // CComPtr<IMFSourceReader> m_spReader - Media Foundation's source reader
    // CRefObj<IImagePool> m_spColorImagePool - image pool
    // UINT32 m_width - width of output image
    // UINT32 m_height - height of output image
    // LONG m_stride - stride of output image
    //
    // Module:
    // {Link:import Cameras.WebCamera.WebCamera;{Cameras/WebCamera/WebCamera.ixx}}
    //**********************************************************************
    class CWebCamera : public CRefCount, public IWebCamera
    {
        CComPtr<IMFSourceReader> m_spReader;
        CRefObj<IImagePool> m_spColorImagePool;
        UINT32 m_width, m_height;
        LONG m_stride;
    public:
        //**********************************************************************
        // Constructor: CWebCamera
        // Default ctor for CWebCamera
        //
        // Parameters:
        // deviceName - symbolic link name for the requested device
        // w - width in pixels of camera resolution. If unspecified (w==-1) the maximum resolution is picked
        // h - height in pixels of camera resolution. If unspecified (w==-1) the maximum resolution is picked
        //
        // Module:
        // {Link:import Cameras.WebCamera.WebCamera;{Cameras/WebCamera/WebCamera.ixx}}
        //**********************************************************************
        CWebCamera(std::wstring deviceName, int w = -1, int h = -1);
        
        ~CWebCamera() {}

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ICameraDevice
        //**********************************************************************
        virtual bool NextFrame(IImage** ppColorImage) override;
        virtual uint32 GetColorWidth() override { return 0; }
        virtual uint32 GetColorHeight() override { return 0; }

        //**********************************************************************
        // IWebCamera
        //**********************************************************************
    };
}
