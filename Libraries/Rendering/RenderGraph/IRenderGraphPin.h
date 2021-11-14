//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Base.Math.Matrix;
#include "Rendering\Caustic\Caustic.h"
#include <vector>
#include <string>
#include <any>
#include <map>

namespace Caustic
{
    //**********************************************************************
    // Enum: ERenderGraphDataType
    // Defines the types that a <IRenderGraphPin> may return.
    //
    // Members:
    // Void - Pin does not return data. This pin is only used for linking
    // (and thus enforcing render order) on other graph nodes
    // Float[N] - returns N floats
    // Int[N] - returns N integers
    //**********************************************************************
    enum class ERenderGraphDataType
    {
        Void,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Matrix4x4,
        Texture,
        Sampler,
        Buffer,
        VertexBuffer,
        IndexBuffer,
        Mesh,
        RenderMesh,
        Material,
        RenderMaterial,
        Shader,
        LightCollection,
        Image
    };

    struct IRenderGraphNode;

    //**********************************************************************
    // Interface: IRenderGraphPin
    // Defines a connection point on a <IRenderGraphNode> object
    //
    // Members:
    // Void - Pin does not return data. This pin is only used for linking
    // (and thus causing rendering to occur) on other graph nodes
    // Float[N] - returns N floats
    // Int[N] - returns N integers
    //**********************************************************************
    struct IRenderGraphPin : public IRefCount
    {
        //**********************************************************************
        // Method: GetDataType
        // Returns the data type of pin
        //**********************************************************************
        virtual ERenderGraphDataType GetDataType() = 0;

        //**********************************************************************
        // Method: GetName
        // Returns the name of pin
        //**********************************************************************
        virtual std::string GetName() = 0;

        //**********************************************************************
        // Method: GetParent
        // Returns the parent <IRenderGraphNode> of pin
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode> GetParent() = 0;

        //**********************************************************************
        // Method: SetDefaultValue
        // Assigns a default value to a pin. If value is empty then we will return
        // what ever is connected to that pin (which might be output from another
        // node's output pin or might be input to our grandparent's node)
        //
        // Parameters:
        // v - value to assign
        //**********************************************************************
        virtual void SetDefaultValue(std::any v) = 0;

        //**********************************************************************
        // Method: GetDefaultValue
        // Returns the default value associated with the pin. If no default value
        // has been assigned to the pin than an empty std::any is returned
        //
        // Returns:
        // The default value for the pin
        //**********************************************************************
        virtual std::any GetDefaultValue() = 0;

        //**********************************************************************
        // Method: GetValue
        // Returns the value from a pin
        //
        // Parameters:
        // pRenderer - current renderer
        // pRenderCtx - render context
        //**********************************************************************
        virtual std::any GetValue(IRenderer* pRenderer, IRenderCtx* pRenderCtx) = 0;

        //**********************************************************************
        // Method: LinkTo
        // Links one pin to another. NOTE: This perform linking in only a single
        // direction. The opposite link (i.e. p1->LinkTo(p2); p2->LinkTo(p1);)
        // must also happen.
        //
        // NOTE: Clients should not be calling this method. Instead they
        // should be using <IRenderGraphNode::Link> method.
        //
        // Parameters:
        // pPin - Pin (on another IRenderGraphNode) to connect to
        //**********************************************************************
        virtual void LinkTo(IRenderGraphPin* pPin) = 0;

        //**********************************************************************
        // Method: NumberConnections
        // Returns the number of connections (i.e. other pins) this pin is connected to
        //**********************************************************************
        virtual uint32 NumberConnections() = 0;

        //**********************************************************************
        // Method: GetConnection
        // Returns the Nth connection on a pin
        //
        // Parameters:
        // index - index of pin to return
        //**********************************************************************
        virtual CRefObj<IRenderGraphPin> GetConnection(int index) = 0;

        //**********************************************************************
        // Method: IsInput
        // Returns whether the pin is an input or output pin
        //**********************************************************************
        virtual bool IsInput() = 0;

        //**********************************************************************
        // Method: ClearLink
        // Removes the Nth connection from the pin
        //
        // Generally, clients should not be calling this method. Instead they
        // should be using <IRenderGraphNode::Unlink> method.
        //
        // Parameters:
        // index - index of which link to remove
        //**********************************************************************
        virtual void ClearLink(int index) = 0;

        //**********************************************************************
        // Method: AddLink
        // Adds a link from this pin to another
        //
        // Clients should not be calling this method.
        //
        // Parameters:
        // pPin - pin to connect to
        //**********************************************************************
        virtual void AddLink(IRenderGraphPin *pPin) = 0;
    };
}
