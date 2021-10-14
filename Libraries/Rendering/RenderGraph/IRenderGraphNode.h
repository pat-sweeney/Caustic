//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.RefCount;
#include "Rendering\Caustic\Caustic.h"
#include "IRenderGraphPin.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    enum ERenderGraphNodeType
    {
        UnknownNode,
        MeshNode,
        CustomRenderNode,
        GroupNode,
        RenderableNode,
        LightCollectionNode,
        PointLightNode,
        MaterialNode,
        ComputeShaderNode,
        ImageToTextureNode
    };

    //**********************************************************************
    // Interface: IRenderGraphNode
    // Defines an interface to a node in our render graph
    //**********************************************************************
    struct IRenderGraphNode : public IRefCount
    {
        //**********************************************************************
        // Method: GetType
        // Returns the type of this node
        //**********************************************************************
        virtual ERenderGraphNodeType GetType() = 0;

        //**********************************************************************
        // Method: GetName
        // Returns the name of this node
        //**********************************************************************
        virtual std::string GetName() = 0;

        //**********************************************************************
        // Method: SetName
        // Sets the name of this node
        //
        // Parameters:
        // name - name to assign to this node
        //**********************************************************************
        virtual void SetName(std::string name) = 0;

        //**********************************************************************
        // Method: SetProperty
        // Assigns the named property the specified value. Properties are values
        // that a node may use but that aren't produced by another IRenderGraphNode
        // (think constant buffer values)
        //
        // Parameters:
        // name - Name of property
        // value - value to assign
        //**********************************************************************
        virtual void SetProperty(std::string name, std::any value) = 0;

        //**********************************************************************
        // Method: NumberInputPins
        // Returns the number of input pins
        //**********************************************************************
        virtual uint32 NumberInputPins() = 0;

        //**********************************************************************
        // Method: NumberOutputPins
        // Returns the number of input pins
        //**********************************************************************
        virtual uint32 NumberOutputPins() = 0;

        //**********************************************************************
        // Method: GetInputPin
        // Returns the Nth input pin
        //
        // Parameters:
        // index - index of input pin to return
        //**********************************************************************
        virtual CRefObj<IRenderGraphPin> GetInputPin(int index) = 0;

        //**********************************************************************
        // Method: GetOutputPin
        // Returns the Nth Output pin
        //
        // Parameters:
        // index - index of Output pin to return
        //**********************************************************************
        virtual CRefObj<IRenderGraphPin> GetOutputPin(int index) = 0;

        //**********************************************************************
        // Method: FindInputPin
        // Returns the input put with the specified name
        //
        // Parameters:
        // name - name of pin to find
        //**********************************************************************
        virtual CRefObj<IRenderGraphPin> FindInputPin(std::string name) = 0;

        //**********************************************************************
        // Method: FindOutputPin
        // Returns the output put with the specified name
        //
        // Parameters:
        // name - name of pin to find
        //**********************************************************************
        virtual CRefObj<IRenderGraphPin> FindOutputPin(std::string name) = 0;

        //**********************************************************************
        // Method: GetParent
        // Returns the parent of this node. Maybe nullptr
        //**********************************************************************
        virtual CRefObj<IRenderGraphNode> GetParent() = 0;

        //**********************************************************************
        // Method: SetParent
        //
        // Parameters:
        // pParent - this node's parent
        //**********************************************************************
        virtual void SetParent(IRenderGraphNode *pParent) = 0;

        //**********************************************************************
        // Method: GetPinValue
        // Retrieves the value currently assigned to a pin. This value is what
        // a node will return as its output value for the specified pin.
        //
        // NOTE: Clients are not expected to call this method. It is only used during
        // graph traversal. It will cause rendering and further traversal of the graph.
        //
        // Parameters:
        // pPin - pin from which to get value
        // pRenderer - the renderer
        // pRenderCtx - current render context
        //**********************************************************************
        virtual std::any GetPinValue(IRenderGraphPin *pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx) = 0;
    };
}
