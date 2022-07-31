#pragma once

#include <vector>

import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.Caustic.Shader;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.RenderWindow.IRenderWindow;

struct CLineVertex
{
    float x, y, z;
};

using namespace Caustic;

class CApp
{
public:
    CRefObj<IRenderWindow> m_spRenderWindow;              // Window that renderer draws to
    CRefObj<Caustic::ICausticFactory> m_spCausticFactory; // Factory used to create Caustic objects
    CRefObj<IMesh> m_spSphere;
    CRefObj<IRenderMesh> m_spSphereRenderMesh;
    CRefObj<IMaterialAttrib> m_spSphereMaterial;
    CRefObj<IMesh> m_spGroundPlaneMesh;
    CRefObj<IRenderMesh> m_spGroundPlaneRenderMesh;
    CRefObj<IMaterialAttrib> m_spGroundPlaneMaterial;
    CRefObj<ILight> m_spPointLight;
    CRefObj<ILight> m_spDirectionalLight;
    HWND m_hwnd;                                          // HWND bound to renderer
    std::vector<CRefObj<ILight>> m_lights;

    CApp()
    {
    }

    CRefObj<IRenderMesh> CreateMesh(IRenderer* pRenderer, IMesh* pMesh, IMaterialAttrib *pMaterial);
    void InitializeCaustic(HWND hwnd);
    void ShutdownCaustic();
    void Setup3DScene(IRenderWindow* pRenderWindow);
    CRefObj<IRenderMesh> CreateMesh(IRenderer* pRenderer, IMesh* pMesh);
};
