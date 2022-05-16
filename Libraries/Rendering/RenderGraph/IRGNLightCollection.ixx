//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Rendering.RenderGraph.IRGNLightCollection;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ILight;
import Rendering.Caustic.IPointLight;
import Rendering.RenderGraph.ILightCollection;
import Rendering.RenderGraph.IRenderGraphNode;

export namespace Caustic
{
    //**********************************************************************
    // Interface: IRenderGraphNode_LightCollection
    // Defines interface for adding lights to a collection
    //**********************************************************************
    struct IRenderGraphNode_LightCollection : public IRenderGraphNode
    {
        virtual void AddLight(IPointLight* pLight) = 0;
        virtual void Clear() = 0;
        virtual std::vector<CRefObj<ILight>> ToVector() = 0;
        virtual CRefObj<ILightCollection> GetLights() = 0;
    };
}
