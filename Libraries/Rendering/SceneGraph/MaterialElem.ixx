//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

export module Rendering.SceneGraph.SceneMaterialElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Base.Math.Ray;
import Base.Core.ConvertStr;
import Rendering.Caustic.IShader;
import Rendering.SceneGraph.SceneGroupElem;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneMaterialElem;

export namespace Caustic
{
	//**********************************************************************
	// Class: CSceneMaterialElem
	// Defines a material that can be assigned to a scene element
	//**********************************************************************
	class CSceneMaterialElem :
        public CSceneGroupElem,
        public ISceneMaterialElem
    {
        CRefObj<IMaterialAttrib> m_spMaterial;
        CRefObj<IShader> m_spShader;
    public:
		//**********************************************************************
		// Constructor: CSceneMaterialElem
		// Default constructor
		//**********************************************************************
		CSceneMaterialElem()
        {
        }

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISceneElem
		//**********************************************************************
		virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
		{
			auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Material", nullptr);
			auto spBase = CSceneGroupElem::AsJson(pPropertyName, pParser);
			spObj->AddElement(spBase);


			if (m_spMaterial)
			{
				const char* cullMode = "";
				switch (m_spMaterial->GetCullMode())
				{
				case D3D11_CULL_MODE::D3D11_CULL_BACK: cullMode = "Back"; break;
				case D3D11_CULL_MODE::D3D11_CULL_FRONT: cullMode = "Front"; break;
				case D3D11_CULL_MODE::D3D11_CULL_NONE: cullMode = "None"; break;
				}
				const char* fillMode = "";
				switch (m_spMaterial->GetFillMode())
				{
				case D3D11_FILL_MODE::D3D11_FILL_SOLID: fillMode = "Solid"; break;
				case D3D11_FILL_MODE::D3D11_FILL_WIREFRAME: fillMode = "Wireframe"; break;
				}
				auto spMat = pParser->CreateJSonMap("Material",
					pParser->CreateJSon("Name", m_spMaterial->GetName().c_str()),
					pParser->CreateJSon("CullMode", cullMode),
					pParser->CreateJSon("FillMode", fillMode),
					pParser->CreateJSon("Transparent", m_spMaterial->GetIsTransparent()),
					pParser->CreateJSon("ShadowReceiver", m_spMaterial->GetIsShadowReceiver()),
					pParser->CreateJSon("MaterialID", (int)m_spMaterial->GetMaterialID()),
					nullptr);
				auto colorObj = pParser->CreateJSonMap("Colors", nullptr);
				IJSonObj* pColorObj = colorObj;
				m_spMaterial->EnumerateColors([pColorObj, pParser](const wchar_t* pName, FRGBAColor& v) {
					auto name = Caustic::wstr2str(pName);
					pColorObj->AddElement(pParser->CreateJSonMap(name.c_str(),
						pParser->CreateJSon("r", v.r),
						pParser->CreateJSon("g", v.g),
						pParser->CreateJSon("b", v.b),
						nullptr));
					});
				spMat->AddElement(colorObj);

				auto scalarObj = pParser->CreateJSonMap("Scalars", nullptr);
				IJSonObj* pScalarObj = scalarObj;
				m_spMaterial->EnumerateScalars([pScalarObj, pParser](const wchar_t* pName, float v) {
					auto name = Caustic::wstr2str(pName);
					pScalarObj->AddElement(pParser->CreateJSon(name.c_str(), v));
					});
				spMat->AddElement(scalarObj);

				auto texObj = pParser->CreateJSonMap("Textures", nullptr);
				IJSonObj* pTexObj = texObj;
				m_spMaterial->EnumerateTextures([pTexObj, pParser](const wchar_t* pName, IImage* pTexture, EShaderAccess access) {
					auto name = Caustic::wstr2str(pName);
					const char* accessStr = "";
					switch (access)
					{
					case EShaderAccess::PixelShader: accessStr = "PixelShader"; break;
					case EShaderAccess::VertexShader: accessStr = "VertexShader"; break;
					case EShaderAccess::Both: accessStr = "PixelShader | VertexShader"; break;
					}
					pTexObj->AddElement(
						pParser->CreateJSonMap("Texture",
							pParser->CreateJSon("Name", name.c_str()),
							pParser->CreateJSon("Access", accessStr),
							pParser->CreateJSon("Width", (int)pTexture->GetWidth()),
							pParser->CreateJSon("Height", (int)pTexture->GetHeight()),
							pParser->CreateJSon("BytesPerPixel", (int)pTexture->GetBytesPerPixel()),
							pParser->CreateJSon("RGBOrder", pTexture->GetRGBOrder()),
							pParser->CreateJSon("ImageType", (int)pTexture->GetImageType()),
							pParser->CreateJSon("SubX", (int)pTexture->GetSubX()),
							pParser->CreateJSon("SubY", (int)pTexture->GetSubY()),
							pParser->CreateJSon("Stride", (int)pTexture->GetStride()),
							nullptr));
					});
				spMat->AddElement(texObj);
				spObj->AddElement(spMat);
			}
//			if (m_spShader)
//				spObj->AddElement(m_spShader->AsJSon("Shader", pParser));
			return spObj;
		}

		virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
		virtual ESceneElemType GetType() { return ESceneElemType::Material; }
		virtual std::wstring GetName() override { return CSceneElem::GetName(); }
		virtual void SetName(const wchar_t *name) override { return CSceneElem::SetName(name); }
		virtual void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback) override
		{
			CSceneElem::SetPreRenderCallback(prerenderCallback);
		}
		virtual void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback) override
		{
			CSceneElem::SetPostRenderCallback(postrenderCallback);
		}
		virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
		virtual void GetBBox(BBox3* pBBox) override
		{
			return CSceneGroupElem::GetBBox(pBBox);
		}
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
		virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
		virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;

		//**********************************************************************
		// ISceneGroupElem
		//**********************************************************************
		virtual uint32 NumberChildren() override { return CSceneGroupElem::NumberChildren(); }
        virtual CRefObj<ISceneElem> GetChild(uint32 index) override { return CSceneGroupElem::GetChild(index); }
        virtual void AddChild(ISceneElem *pElem) override { CSceneGroupElem::AddChild(pElem); }
        virtual void InsertChild(ISceneElem *pElem, uint32 index) override { CSceneGroupElem::InsertChild(pElem, index); }
		virtual Matrix4x4 GetTransform() override { return m_Transform; }
		virtual void SetTransform(Matrix4x4 &transform) override { CSceneGroupElem::SetTransform(transform); }

		//**********************************************************************
		// ISceneMaterialElem
		//**********************************************************************
		virtual void SetShader(IShader *pShader) override
        {
            m_spShader = pShader;
        }

		virtual CRefObj<IMaterialAttrib> GetMaterial() override
		{
			return m_spMaterial;
		}

		virtual void SetMaterial(IMaterialAttrib *pMaterial) override
		{
			m_spMaterial = pMaterial;
		}
	};
};
