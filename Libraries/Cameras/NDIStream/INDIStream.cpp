//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

module Cameras.NDIStream.INDIStream;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Cameras.CameraBase.ICamera;
import Cameras.NDIStream.NDIStream;

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    CRefObj<INDIStream> CreateNDIStream()
    {
        CRefObj<INDIStream> spObj = CRefObj<INDIStream>(new CNDIStream());
        return spObj;
    }
}
