//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "ISceneGraph.h"
#include "SceneElem.h"
#include <d3d12.h>
#include <atlbase.h>

namespace Caustic
{
	//**********************************************************************
	// Class: CPointLightElem
	// Defines a point light
	//**********************************************************************
	class CPointLightElem :
        public CSceneElem,
        public IScenePointLightElem,
        public CRefCount
    {
        CRefObj<IPointLight> m_spPointLight;

    public:
		//**********************************************************************
		// Constructor: CPointLightElem
		// Default ctor
		//
		// Parameters:
		// pPointLight - underlying point light this scene element controls
		//**********************************************************************
		CPointLightElem(IPointLight *pPointLight)
        {
			m_spPointLight = pPointLight;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual ESceneElemType GetType() { return ESceneElemType::PointLight; }
        virtual std::wstring &Name() override;
        virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
        virtual void GetBBox(BBox3 *pBBox) override;
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;

        //**********************************************************************
        // IPointLightElem
        //**********************************************************************
        virtual void SetPosition(Caustic::Vector3 &pos) override;
        virtual void SetColor(Caustic::Vector3 &clr) override;
    };
}
