//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>
#include <atlbase.h>
#include <memory>

module Imaging.Video.IVideo;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Video.Video;

namespace Caustic
{

    //**********************************************************************
    // Function: CreateVideo
    // CreateVideo loads a video
    //
    // Parameters:
    // pFilename - Name of file to load
    // pRenderer - Renderer
    //
    // Returns:
    // Returns the new texture
    //**********************************************************************
    CRefObj<IVideo> CreateVideo(const wchar_t* pFilename)
    {
        std::unique_ptr<CVideo> spVideo(new CVideo());
        spVideo->LoadFromFile(pFilename);
        return CRefObj<IVideo>(spVideo.release());
    }
}