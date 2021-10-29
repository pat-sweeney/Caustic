//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.ITrackball;
import Rendering.Caustic.Trackball;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;

namespace Caustic
{
    //**********************************************************************
    // Function: CreateTrackball
    // Global function for creating a track ball. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Returns:
    // Returns the newly created trackball.
    //**********************************************************************
    CRefObj<ITrackball> CreateTrackball()
    {
        return CRefObj<ITrackball>(new CTrackball());
    }
}