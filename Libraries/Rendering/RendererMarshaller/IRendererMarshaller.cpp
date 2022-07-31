//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.RendererMarshaller.IRendererMarshaller;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;

//**********************************************************************
// File: IRendererMarshaller.cpp
// Contains implementation of public parts of IRendererMarshaller.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateRendererMarshaller
    // Global function for creating the RendererMarshaller. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Returns:
    // Returns the newly created RendererMarshaller.
    // 
    // Module:
    // {Link:import Rendering.Caustic.IRendererMarshaller;{Rendering/Caustic/IRendererMarshaller.cpp}}
    //**********************************************************************
    CRefObj<IRendererMarshaller> CreateRendererMarshaller()
    {
        extern CRefObj<IRendererMarshaller> CreateRendererMarshallerInternal();
        return CreateRendererMarshallerInternal();
    }
}
