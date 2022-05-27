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
#include <winerror.h>
#include <new>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <strsafe.h>
#include <assert.h>
#include <atlbase.h>
#include <vector>

module Cameras.WebCamera.IWebCamera;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;
import Cameras.WebCamera.WebCamera;

namespace Caustic
{
    static bool initialized = false;
    static void CWebCamera_Setup()
    {
        if (!initialized)
        {
            CT(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));
            CT(MFStartup(MF_VERSION));
            initialized = true;
        }
    }

    static void CWebCamera_Shutdown()
    {
        //UnregisterDeviceNotification(m_hDeviceNotification);
        MFShutdown();
        CoUninitialize();
    }

    std::vector<CameraInfo> IWebCamera::GetAvailableDevices()
    {
        CWebCamera_Setup();
        std::vector<CameraInfo> cameras;
        CComPtr<IMFAttributes> spAttributes;
        CT(MFCreateAttributes(&spAttributes, 1));
        CT(spAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
        IMFActivate** ppDevices;
        UINT32 count;
        CT(MFEnumDeviceSources(spAttributes, &ppDevices, &count));
        for (int i = 0; i < (int)count; i++)
        {
            GUID guid;
            CT(ppDevices[i]->GetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &guid));
            if (guid == MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID)
            {
                CameraInfo caminfo;
                wchar_t buffer[1024];
                UINT32 length;
                CT(ppDevices[i]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, buffer, 1024, &length));
                caminfo.name = std::wstring(buffer);
                CT(ppDevices[i]->GetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, buffer, 1024, &length));
                caminfo.symlink = std::wstring(buffer);
                UINT32 blobSize;
                CT(ppDevices[i]->GetBlob(MF_DEVSOURCE_ATTRIBUTE_MEDIA_TYPE, (UINT8*)&caminfo.info, sizeof(caminfo.info), &blobSize));
                cameras.push_back(caminfo);
            }
            ppDevices[i]->Release();
        }
        if (ppDevices != nullptr)
            CoTaskMemFree(ppDevices);
        return cameras;
    }

    //**********************************************************************
    // Function: CreateWebCamera
    // CreateWebCamera instantiates an instance of an Web camera
    //**********************************************************************
    CRefObj<IWebCamera> CreateWebCamera(std::wstring deviceName)
    {
        std::unique_ptr<CWebCamera> pCamera(new CWebCamera(deviceName));
        return CRefObj<IWebCamera>(pCamera.release());
    }
}
