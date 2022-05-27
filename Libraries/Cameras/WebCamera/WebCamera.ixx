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
    class CMFHandler : public IMFSourceReaderCallback
    {
        ULONG m_cRef;
    public:
        CMFHandler() :
            m_cRef(0)
        {
        }

        HRESULT QueryInterface(REFIID riid, LPVOID* ppvObj)
        {
            if (!ppvObj)
                return E_INVALIDARG;
            *ppvObj = NULL;
            if (riid == __uuidof(IUnknown) || riid == __uuidof(IMFSourceReaderCallback))
            {
                *ppvObj = (LPVOID)this;
                AddRef();
                return S_OK;
            }
            return E_NOINTERFACE;
        }

        ULONG AddRef()
        {
            InterlockedIncrement(&m_cRef);
            return m_cRef;
        }

        ULONG Release()
        {
            ULONG ulRefCount = InterlockedDecrement(&m_cRef);
            if (0 == m_cRef)
            {
                delete this;
            }
            return ulRefCount;
        }

        HRESULT OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample)
        {
            return S_OK;
        }
    };

    class CWebCamera : public CRefCount, public IWebCamera
    {
        HDEVNOTIFY m_hDeviceNotification;
        void Setup();
        void Shutdown();

        CComPtr<IMFSourceReader> m_spReader;
        CRefObj<IImagePool> m_spColorImagePool;
        UINT32 m_width, m_height;
        LONG m_stride;
    public:
        CWebCamera(std::wstring deviceName);
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
