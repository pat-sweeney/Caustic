//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.RefCount;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\RenderGraph\IRenderGraphNode.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    //**********************************************************************
    // Interface: IRenderGraph
    // Defines the main interface for dealing with our render graph.
    //
    // A Render graph is similar to a scene graph except that each node
    // exposes input & output pins which allow for the flow of data between
    // the nodes. This allows us to specify a oragnizational hierarchy along
    // with enforcing rendering order. Rendering traversal occurs from back
    // to front. For instance, in the graph shown below:
    // --- Code
    //     Transform                    Group
    //   +---------+        xform +--------------------------------------------------+
    //   |         |---o ---> o---|<-+                                               |
    //   |         |              |  |    Compute                                    |
    //   |         |              |  |  +---------+   out (CRefObj<IBuffer>)         |
    //   +---------+              |  |  |         |---o -+                           |
    //                            |  |  |         |      |           Mesh            |
    //                            |  |  |         |      |         +-------+         |
    //                            |  |  +---------+      ----> o---|       |         |
    //                            |  |                          VB |       |         |
    //                            |  |                             |       |         |
    //                            |  +------------------------ o---|       |         |
    //                            |                          xform +-------+         |
    //                            |                                                  |
    //                            +--------------------------------------------------+
    // ---
    // The renderer will start with the "Group" node and traversal its
    // members. Each member that has no output is considered the end
    // of the graph. So in this case it will start with the "Mesh" node.
    // The Mesh node gets its vertex buffer from the "Compute" node. So,
    // the renderer will next traverse the Compute node, which will then
    // render a buffer that is passed to the mesh node. The Mesh node also
    // requires a "xform" property. The "xform" pin on "Mesh" is not connected
    // to any node, so the renderer will ask the parent if it has this input
    // pin. If so it will use that value. In this case the Mesh will get its
    // transform from the outer "Transform" element. This mechanism allows
    // us to pass a value from a single node (i.e. the "Transform") to many
    // nodes in the group (e.g. "Mesh" node) without having to explicitly
    // wire each transform to every mesh in the "Group".
    //**********************************************************************
    struct IRenderGraph : public IRefCount
    {
        //**********************************************************************
        // Method: AddChild
        // Adds a new node to our render graph
        //
        // Parameters:
        // pChild - node to add
        //**********************************************************************
        virtual void AddChild(IRenderGraphNode* pChild) = 0;

        //**********************************************************************
        // Method: Render
        // Renders our graph
        //
        // Parameters:
        // pRenderer - pointer to the renderer
        // pRenderCtx - current render context
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) = 0;

        //**********************************************************************
        // Method: Lock
        // Locks the graph so it may be updated
        //**********************************************************************
        virtual void Lock() = 0;

        //**********************************************************************
        // Method: Unlock
        // Unlocks the graph
        //**********************************************************************
        virtual void Unlock() = 0;
    };
}
