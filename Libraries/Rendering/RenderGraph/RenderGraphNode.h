//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include "IRenderGraphNode.h"
#include "RenderGraphPin.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderGraphNode
    // Base class used for objects implementing <IRenderGraphNode>
    //
    // Members:
    // m_type - type of node
    // m_name - name for this node
    // m_inputs - list of input pins
    // m_outputs - list of output pins
    // m_properties - list of constant buffer values
    // m_parent - our parent node
    // m_lastEpochModified - last epoch this node was modified
    //**********************************************************************
    class CRenderGraphNode
    {
    protected:
        ERenderGraphNodeType m_type;
        std::string m_name;
        std::vector<CRefObj<IRenderGraphPin>> m_inputs;
        std::vector<CRefObj<IRenderGraphPin>> m_outputs;
        std::map<std::string, std::any> m_properties;
        CRefObj<IRenderGraphNode> m_parent;
        uint32 m_lastEpochModified;
    public:
        CRenderGraphNode() :
            m_type(ERenderGraphNodeType::UnknownNode),
            m_lastEpochModified(0)
        {
        }
        
        CRefObj<IRenderGraphPin> CreatePin(IRenderGraphNode *pParent, bool input, std::string pinName, ERenderGraphDataType dataType, std::any defaultValue)
        {
            CRefObj<IRenderGraphPin> spPin(new CRenderGraphPin(pParent, pinName, input, dataType, defaultValue));
            if (input)
                m_inputs.push_back(spPin);
            else
                m_outputs.push_back(spPin);
            return spPin;
        }
        
        //**********************************************************************
        ERenderGraphNodeType GetType() { return m_type; }
        std::string GetName() { return m_name; }
        void SetProperty(std::string name, std::any value) { m_properties[name] = value; }
        void SetName(std::string name) { m_name = name; }
        uint32 NumberInputPins() { return (uint32)m_inputs.size(); }
        uint32 NumberOutputPins() { return (uint32)m_outputs.size(); }
        CRefObj<IRenderGraphPin> GetInputPin(int index) { return m_inputs[index]; }
        CRefObj<IRenderGraphPin> GetOutputPin(int index) { return m_outputs[index]; }
        CRefObj<IRenderGraphPin> FindInputPin(std::string name);
        CRefObj<IRenderGraphPin> FindOutputPin(std::string name);
        CRefObj<IRenderGraphNode> GetParent() { return m_parent; }
        void SetParent(IRenderGraphNode* pParent) { m_parent = pParent; }
    };
}
