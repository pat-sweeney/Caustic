#pragma once

using namespace System;

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Imaging\Image\Image.h"

namespace CausticInterop
{
	public ref class Image
	{
        Caustic::IImage *m_pSourceImage;
        Caustic::IImage *m_pAlphaImage;
        System::Windows::Media::Imaging::WriteableBitmap^ m_bitmap;
    public:
        Image() : 
            m_pSourceImage(nullptr),
            m_pAlphaImage(nullptr),
            m_bitmap(nullptr)
        {
        }

        ~Image()
        {
            if (m_pSourceImage)
                m_pSourceImage->Release();
            if (m_pAlphaImage)
                m_pAlphaImage->Release();
        }
        
        System::Windows::Media::Imaging::WriteableBitmap^ Load(String ^fn);
        void Update(Caustic::IImage *pImage);
        void ExtractMesh();
        Image^ Clone();
        System::Windows::Media::Imaging::WriteableBitmap^ GetWritableBitmap() { return m_bitmap; }
    };
}
