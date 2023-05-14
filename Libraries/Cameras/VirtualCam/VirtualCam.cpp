//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <windows.h>
#include <string>
#include <atlbase.h>

module Cameras.VirtualCamera.VirtualCamera;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.RefCount;
import Base.Core.Error;
import Rendering.Caustic.ITexture;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Cameras.VirtualCamera.IVirtualCamera;

// Namespace: Caustic
namespace Caustic
{
#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080
    CVirtualCamera::CVirtualCamera()
    {
        m_hFrameReady = CreateEvent(NULL, FALSE, FALSE, L"FrameReady");
        m_hFrameMutex = CreateMutex(NULL, FALSE, L"FrameMutex");
        ULARGE_INTEGER size;
        size.QuadPart = IMAGE_WIDTH * IMAGE_HEIGHT * 3;
        m_hFileMapping = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, size.HighPart, size.LowPart, L"FrameData");
        m_pSharedMemory = (BYTE*)MapViewOfFile(m_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    }

    void CVirtualCamera::SendFrame(IImage *pImage)
    {
        // create or open events and shared memory:
        if (!WaitForSingleObject(m_hFrameMutex, 1000))
        {
            auto pDst = m_pSharedMemory;
            CImageIter32 rowSrc(pImage, 0, pImage->GetHeight() - 1);
            for (uint32 y = 0; y < pImage->GetHeight(); y++)
            {
                CImageIter32 colSrc = rowSrc;
                for (uint32 x = 0; x < pImage->GetWidth(); x++)
                {
                    *pDst++ = colSrc.GetBlue();
                    *pDst++ = colSrc.GetGreen();
                    *pDst++ = colSrc.GetRed();
                    colSrc.Step(CImageIter::Right);
                }
                if (IMAGE_WIDTH > pImage->GetWidth())
                    for (uint32 x2 = 0; x2 < IMAGE_WIDTH - pImage->GetWidth(); x2++)
                    {
                        *pDst++ = 0;
                        *pDst++ = 0;
                        *pDst++ = 0;
                    }
                rowSrc.Step(CImageIter::Up);
            }    
            ReleaseMutex(m_hFrameMutex);
            SetEvent(m_hFrameReady);
        }
    }
}
