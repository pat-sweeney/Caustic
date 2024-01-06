//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>
#include <map>

export module Rendering.RenderGraph.RenderGraphPin;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderer;
import Rendering.RenderGraph.IRenderGraphPin;
import Rendering.RenderGraph.IRenderGraphNode;
import Rendering.RenderGraph.RenderGraphDiagnostics;

export namespace Caustic
{
    //**********************************************************************
    // Class: CRenderGraphPin
    // Implementation of <IRenderGraphPin>
    //
    // Members:
    // m_type - type of data transfered over this pin
    // m_name - name of this pin
    // m_connections - pins this pin is connected to
    // m_parent - parent <IRenderGraphNode> this pin is exposed by
    // m_isInput - indicates whether this pin is an input (true) or output (false)
    // m_defaultVal - defines a default value to assign to the pin if the pin is not connected. If empty then parent's pins are searched for value
    //**********************************************************************
    class CRenderGraphPin : public IRenderGraphPin, public CRefCount
    {
    protected:
        ERenderGraphDataType m_type;
        std::string m_name;
        std::vector<CRefObj<IRenderGraphPin>> m_connections;
        CRefObj<IRenderGraphNode> m_parent;
        bool m_isInput;
        std::any m_defaultVal;

    public:
        CRenderGraphPin(IRenderGraphNode* pParent, std::string name, bool isInput, ERenderGraphDataType dataType, std::any defaultValue)
        {
            m_parent = pParent;
            m_isInput = isInput;
            m_name = name;
            m_defaultVal = defaultValue;
            m_type = dataType;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraphPin
        //**********************************************************************
        virtual ERenderGraphDataType GetDataType() override { return m_type; }
        virtual std::string GetName() override { return m_name; }
        virtual void SetDefaultValue(std::any v) override { m_defaultVal = v; }
        virtual std::any GetDefaultValue() override { return m_defaultVal; }
        virtual std::any GetValue(IRenderer *pRenderer, IRenderCtx *pRenderCtx) override;
        virtual CRefObj<IRenderGraphNode> GetParent() override { return m_parent; }
        virtual void LinkTo(IRenderGraphPin* pPin) override;
        virtual uint32_t NumberConnections() override { return (uint32_t)m_connections.size(); }
        virtual CRefObj<IRenderGraphPin> GetConnection(int index) override
        {
            if (index >= 0 && index < m_connections.size())
                return m_connections[index];
            return CRefObj<IRenderGraphPin>(nullptr);
        }
        virtual bool IsInput() override { return m_isInput; }
        virtual void ClearLink(int index) override;
        virtual void AddLink(IRenderGraphPin* pPin) override;
    };
}
