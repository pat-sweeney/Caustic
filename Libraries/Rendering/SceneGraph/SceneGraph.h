//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Base\Math\Matrix.h"

namespace Caustic
{
    enum ESceneElemType
    {
        SceneGraph,
        Mesh,
        Group,
        Renderable,
        PointLight,
        Material
    };

    struct SceneCtx
    {
        CComPtr<ID3D11Device> m_spDevice;
        Matrix4x4 m_Transform;
        std::vector<CRefObj<IPointLight>> m_lights;
        int m_CurrentPass;
        CRefObj<IMaterialAttrib> m_spCurrentMaterial;
        CRefObj<IGraphics> m_spGraphics;
    };

    enum ESceneElemFlags
    {
        Hidden = 0x01,
        Selected = 0x02,

        // List of private flags.
        // TODO: Move this to private header
        BBoxDirty = 0x04,
        RenderableDirty = 0x08
    };

    struct ISceneElem : public ISerialize
    {
        virtual ESceneElemType GetType() = 0;
        virtual std::wstring &Name() = 0;
        virtual uint32 GetFlags() = 0;
        virtual void SetFlags(uint32 flags) = 0;
        virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) = 0;
        virtual void GetBBox(BBox3 *pBBox) = 0;
    };

    struct ISceneGroupElem : public ISceneElem
    {
        virtual uint32 NumberChildren() = 0;
        virtual CRefObj<ISceneElem> GetChild(uint32 index) = 0;
        virtual void AddChild(ISceneElem *pElem) = 0;
        virtual void InsertChild(ISceneElem *pElem, uint32 index) = 0;
        virtual void SetTransform(Matrix4x4 &transform) = 0;
    };

    struct ISceneMaterialElem : public ISceneGroupElem
    {
        virtual void SetPixelShader(IShader *pShader) = 0;
        virtual void SetVertexShader(IShader *pShader) = 0;
        virtual void GetMaterial(IMaterialAttrib **ppMaterial) = 0;
    };
    
    struct IScenePointLightElem : public ISceneElem
    {
        virtual void SetPosition(Caustic::Vector3 &pos) = 0;
        virtual void SetColor(Caustic::Vector3 &clr) = 0;
    };

    struct ISceneMeshElem : public ISceneElem
    {
        virtual void SetMesh(IMesh *pMesh) = 0;
    };

    struct ISceneGraph : public ISceneGroupElem
    {
        virtual void Merge(ISceneGraph *pGraph) = 0;
        virtual void Lock() = 0;
        virtual void Unlock() = 0;
    };

    namespace Scene
    {
        CAUSTICAPI void CreateMaterialElem(ISceneMaterialElem **ppElem); //!< Create material element (pixel shader, vertex shader, and texture)
        CAUSTICAPI void CreatePointLightElem(IScenePointLightElem **ppLight); //!< Create point light
        CAUSTICAPI void CreateGroupElem(ISceneGroupElem **ppGroup); //!< Creates a group element
        CAUSTICAPI void CreateMeshElem(ISceneMeshElem **ppMesh); //!< Create mesh element
        CAUSTICAPI void CreateSceneGraph(ISceneGraph **ppGraph); //!< Create scene graph
    };
};
