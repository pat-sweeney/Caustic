//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Math\BBox.h"
#include "Base\Math\Vector.h"

namespace Caustic
{
    //**********************************************************************
    // Struct: CGeomVertex
    // Defines a vertex in our mesh. 
    //
    // TODO: This used to be part of the Mesh library, but I have since moved
    // it into the renderer. The problem is that I want to be able to share
    // building of vertex buffers between the renderer and the mesh object
    // without having to copy a lot of data around. Thus I need this type in
    // this layer. This needs to be rethought about how to handle properly.
    //
    // Defines a vertex on our mesh. We have two types of vertex data:
    // 1) data that is specific to a vertex relative to a given face
    // 2) data that is relative to a vertex regardless of face
    // The reason for this split is so that positional information
    // can be shared across the mesh (useful for performing operations
    // on the geometry) while data that can change from face to face
    // is stored separately (such as normal information).
    // Typically most mesh data is stored in CGeomVertex. Only faces
    // at material boundaries or smoothing groups contain CFaceVertex data.
    //
    // Header:
    // [Link:Rendering/Caustic/RenderTypes.h]
    //**********************************************************************
    struct CGeomVertex
    {
        Vector3 pos;        // Defines the position
        Vector3 norm;       // Defines the normal
        Vector2 uvs[4];     // Defines UV coordinates
        int index;          // Index (used for serialization)
    };

    //**********************************************************************
    // Struct: CGeomFace
    // Defines a face (triangle) on our mesh as a tuple of vertex indices.
    //**********************************************************************
    struct CGeomFace
    {
        int indices[3];
    };
}
