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
#include <Wmcodecdsp.h>

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
            CT(MFTRegisterLocalByCLSID(CLSID_CColorConvertDMO, MFT_CATEGORY_VIDEO_PROCESSOR,
                L"", MFT_ENUM_FLAG_SYNCMFT, 0, NULL, 0, NULL));
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


                // Enumerate the available formats
                CComPtr<IMFMediaSource> spSource;
                HRESULT hr = ppDevices[i]->ActivateObject(IID_PPV_ARGS(&spSource));
                if (SUCCEEDED(hr))
                {
                    CComPtr<IMFPresentationDescriptor> spPD;
                    CT(spSource->CreatePresentationDescriptor(&spPD));

                    // Find the video stream index
                    DWORD streamCount;
                    CT(spPD->GetStreamDescriptorCount(&streamCount));
                    for (DWORD streamIndex = 0; streamIndex < streamCount; streamIndex++)
                    {
                        BOOL fSelected;
                        CComPtr<IMFStreamDescriptor> spSD;
                        CT(spPD->GetStreamDescriptorByIndex(streamIndex, &fSelected, &spSD));

                        CComPtr<IMFMediaTypeHandler> spHandler;
                        CT(spSD->GetMediaTypeHandler(&spHandler));

                        GUID mediaType;
                        CT(spHandler->GetMajorType(&mediaType));
                        if (mediaType != MFMediaType_Video)
                            continue;

                        DWORD cTypes = 0;
                        CT(spHandler->GetMediaTypeCount(&cTypes));
                        for (DWORD i = 0; i < cTypes; i++)
                        {
                            CComPtr<IMFMediaType> spType;
                            CT(spHandler->GetMediaTypeByIndex(i, &spType));
                            UINT32 w, h;
                            MFGetAttributeSize(spType, MF_MT_FRAME_SIZE, &w, &h);
                            Point2 wxh((int)w, (int)h);
                            caminfo.resolutions.push_back(wxh);
                        }
                        break;
                    }
                }

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
    // 
    // Parameters:
    // deviceName - symbolic link name for the camera as returned by
    // <IWebCamera::GetAvailableDevices>
    // w - width in pixels of camera resolution
    // h - height in pixels of camera resolution
    //**********************************************************************
    CRefObj<IWebCamera> CreateWebCamera(std::wstring deviceName, int w /* = -1*/, int h /* = -1*/)
    {
        std::unique_ptr<CWebCamera> pCamera(new CWebCamera(deviceName, w, h));
        return CRefObj<IWebCamera>(pCamera.release());
    }
}
