#pragma once

using namespace System;

//import Base.Core.IRefCount;
//#include "Imaging\Image\Image.h"

namespace CausticInterop
{
    public ref class Image
    {
        void *m_pSourceImage;
        void *m_pAlphaImage;
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
#if 0
            if (m_pSourceImage)
                m_pSourceImage->Release();
            if (m_pAlphaImage)
                m_pAlphaImage->Release();
#endif
        }
        
        System::Windows::Media::Imaging::WriteableBitmap^ Load(String ^fn);
        void Update(void *pImage);
 //       void ExtractMesh();
        Image^ Clone();
        System::Windows::Media::Imaging::WriteableBitmap^ GetWritableBitmap() { return m_bitmap; }
    };
}
