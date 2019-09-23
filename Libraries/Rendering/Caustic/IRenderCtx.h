//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"

//**********************************************************************
// File: IRenderCtx.h
// This file defines the published interface the Render context.
// The render context defines top level states that are relevant to
// the entire frame.
//**********************************************************************

namespace Caustic
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
    //**********************************************************************
    namespace RenderCtxFlags
    {
        const uint32 c_DisplayNormalsAsColors = 0x01;
        const uint32 c_DisplayNormalsAsLines = 0x02;
        const uint32 c_DisplayFaceNormals = 0x04;
        const uint32 c_DisplayWireframe = 0x08;
        const uint32 c_DisplayWorldAxis = 0x10;
        const uint32 c_DisplayUVsAsColors = 0x20;
        const uint32 c_DisplayLightDir = 0x40;
        const uint32 c_DisplayGrid = 0x80;
    }

    //**********************************************************************
    // Interface: IRenderCtx
    // Defines the main interface for setting various features in
    // the renderer
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
        virtual void SetDebugFlags(uint32 flags) = 0;

        //**********************************************************************
        // Method: GetDebugFlags
        // Returns the current debug flags used for rendering
        //**********************************************************************
        virtual uint32 GetDebugFlags() = 0;

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
        virtual uint32 GetCurrentPass() = 0;

        //**********************************************************************
        // Method: PassBlendable
        // Returns whether the current render pass supports alpha blending
        //**********************************************************************
        virtual bool PassBlendable() = 0;
    };
}
