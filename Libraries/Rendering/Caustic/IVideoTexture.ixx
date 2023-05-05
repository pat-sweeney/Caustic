//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.Caustic.IVideoTexture;
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: IVideoTexture.ixx
// This file defines the published interface for video textures.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IVideoTexture
    // Defines how clients interact with video textures
    //
    // Module:
    // {Link:import Rendering.Caustic.IVideoTexture;{Rendering/Caustic/IVideoTexture.ixx}}
    //**********************************************************************
    struct IVideoTexture : public ITexture
    {
        //**********************************************************************
        // Method: EndOfStream
        // Returns:
        // Returns true if at end of stream. false otherwise.
        //**********************************************************************
        virtual bool EndOfStream() = 0;

        //**********************************************************************
        // Method: Restart
        // Restarts the stream from the beginning of the video
        //**********************************************************************
        virtual void Restart() = 0;
    };
}
