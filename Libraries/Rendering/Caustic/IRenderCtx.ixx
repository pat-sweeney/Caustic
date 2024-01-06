//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <cinttypes>

export module Rendering.Caustic.IRenderCtx;

import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;

//**********************************************************************
// File: IRenderCtx.h
// This file defines the published interface the Render context.
// The render context defines top level states that are relevant to
// the entire frame.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Namespace: RenderCtxFlags
    // Defines flags for setting various render modes
    //
    // c_DisplayNormalsAsColors - Render mesh using the normal vector as a color
    // c_DisplayNormalsAsLines - Render the mesh and display normal vectors as lines emanating from the mesh
    // c_DisplayFaceNormals - Render the normals at each
    // c_DisplayWireframe - Render the mesh as a wireframe
    // c_DisplayWorldAxis - Draw a set of world coordinate axis at the origin
    // c_DisplayUVsAsColors - Render the mesh using UV texture coordinates as the mesh surface color
    // c_DisplayLightDir - Render geometry to indicate the light direction
    // c_DisplayGrid - Render an XY grid plane at the origin
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderCtx{Rendering.Caustic.IRenderCtx.ixx}}
    //**********************************************************************
    namespace RenderCtxFlags
    {
        const uint32_t c_DisplayNormalsAsColors = 0x01;
        const uint32_t c_DisplayNormalsAsLines = 0x02;
        const uint32_t c_DisplayFaceNormals = 0x04;
        const uint32_t c_DisplayWireframe = 0x08;
        const uint32_t c_DisplayWorldAxis = 0x10;
        const uint32_t c_DisplayUVsAsColors = 0x20;
        const uint32_t c_DisplayLightDir = 0x40;
        const uint32_t c_DisplayGrid = 0x80;
    }

    //**********************************************************************
    // Interface: IRenderCtx
    // Defines the main interface for setting various features in
    // the renderer
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderCtx{Rendering.Caustic.IRenderCtx.ixx}}
    //**********************************************************************
    struct IRenderCtx : public IRefCount
    {
        //**********************************************************************
        // Method: SetDebugFlags
        // Sets debug flags used for rendering
        //
        // Parameters:
        // flags - new debug flags
        //**********************************************************************
        virtual void SetDebugFlags(uint32_t flags) = 0;

        //**********************************************************************
        // Method: GetDebugFlags
        // Returns the current debug flags used for rendering
        //**********************************************************************
        virtual uint32_t GetDebugFlags() = 0;

        //**********************************************************************
        // Method: SetNormalScale
        // Sets the scale used to determine how large debug normal vectors are drawn
        //
        // Parameters:
        // normalScale - sets the scale factor for drawing debug normal vectors
        //**********************************************************************
        virtual void SetNormalScale(float normalScale) = 0;

        //**********************************************************************
        // Method: GetNormalScale
        // Returns the scale used for determining how large debug normal vectors are drawn
        //**********************************************************************
        virtual float GetNormalScale() = 0;

        //**********************************************************************
        // Method: GetCurrentPass
        // Returns the current render pass
        //**********************************************************************
        virtual uint32_t GetCurrentPass() = 0;

        //**********************************************************************
        // Method: PassBlendable
        // Returns whether the current render pass supports alpha blending
        //**********************************************************************
        virtual bool PassBlendable() = 0;

        //**********************************************************************
        // Method: IncrementEpoch
        // Increment to the next epoch
        //
        // Returns:
        // Returns the new epoch
        //**********************************************************************
        virtual uint32_t IncrementEpoch() = 0;

        //**********************************************************************
        // Method: GetEpoch
        // Returns the current epoch. The epoch is updated each time something changes
        //
        // Returns:
        // Returns the current epoch
        //**********************************************************************
        virtual uint32_t GetEpoch() = 0;

        //**********************************************************************
        // Method: GetMostRecentEpoch
        // Returns the most recent epoch found while traversing the render graph
        //
        // Returns:
        // Returns the most recent epoch
        //**********************************************************************
        virtual uint32_t GetMostRecentEpoch() = 0;

        //**********************************************************************
        // Method: SetMostRecentEpoch
        // Sets the most recent epoch found while traversing the render graph
        //
        // Parameters:
        // v - value to set most recent epoch to
        //**********************************************************************
        virtual void SetMostRecentEpoch(uint32_t v) = 0;
    };
}
