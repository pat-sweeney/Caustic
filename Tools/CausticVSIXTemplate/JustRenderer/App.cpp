#include "stdafx.h"
#include "App.h"
import Caustic.Base;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Base.Math.Vector;
import Rendering.Caustic.Shader;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.RendererFlags;
import Rendering.RenderWindow.IRenderWindow;
import Geometry.Mesh.MeshFuncs;

CRefObj<IRenderMesh> CApp::CreateMesh(IRenderer* pRenderer, IMesh *pMesh, IMaterialAttrib *pMaterial)
{
    // Find the default shader
    CRefObj<IShader> spShader = pRenderer->GetShaderMgr()->FindShader(L"Default");

    // Convert our mesh into a render mesh
    // The render mesh is an optimized version (vertex buffer/index buffer) of the mesh
    CRefObj<IRenderMesh> spRenderMesh = pRenderer->ToRenderMesh(pMesh, spShader);

    // Convert our materials into render materials.
    // Each of the materials that was assigned to the mesh (specifically it's submeshes)
    // will be converted into a renderable material.
    pRenderer->ToRenderMaterials(pMesh, spShader, spRenderMesh, pMaterial);
    return spRenderMesh;
}

void CApp::Setup3DScene(IRenderWindow* pRenderWindow)
{
    // Create a simple scene with a sphere on top of ground plane
    CRefObj<IRenderer> spRenderer = m_spRenderWindow->GetRenderer();
    m_spSphere = Caustic::CreateSphere(10);
    m_spSphereMaterial = m_spCausticFactory->CreateMaterialAttrib();
    FRGBColor ambient(0.2f, 0.2f, 0.2f);
    FRGBColor diffuse(1.0f, 0.0f, 0.0f);
    m_spSphereMaterial->SetColor(L"ambientColor", ambient);
    m_spSphereMaterial->SetColor(L"diffuseColor", diffuse);
    m_spSphereMaterial->SetCullMode(D3D11_CULL_NONE);
    m_spSphereRenderMesh = CreateMesh(spRenderer, m_spSphere, m_spSphereMaterial);


    m_spGroundPlaneMesh = Caustic::CreateGrid(2, 2);
    m_spGroundPlaneMaterial = m_spCausticFactory->CreateMaterialAttrib();
    ambient = FRGBColor(0.2f, 0.2f, 0.2f);
    diffuse = FRGBColor(0.4f, 0.4f, 0.4f);
    m_spGroundPlaneMaterial->SetColor(L"ambientColor", ambient);
    m_spGroundPlaneMaterial->SetColor(L"diffuseColor", diffuse);
    m_spGroundPlaneMaterial->SetCullMode(D3D11_CULL_NONE);
    m_spGroundPlaneRenderMesh = CreateMesh(spRenderer, m_spGroundPlaneMesh, m_spGroundPlaneMaterial);

    // Create two lights for the scene
    Vector3 lightPos(1000.0f, 1000.0f, 0.0f);
    FRGBColor lightColor(1.0f, 1.0f, 1.0f);
    m_spPointLight = m_spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f);

    Vector3 lightDir(-1.0f, -1.0f, -1.0f);
    m_spDirectionalLight = m_spCausticFactory->CreateDirectionalLight(lightPos, lightDir, lightColor, 1.0f);

    m_lights.push_back(m_spPointLight);
    m_lights.push_back(m_spDirectionalLight);
}

//**********************************************************************
// Method: InitializeCaustic
// This method is called right before the main window is displayed
// It will setup the rendering engine
//
// Parameters:
// hwnd - application's main window
//**********************************************************************
void CApp::InitializeCaustic(HWND hwnd)
{
    m_hwnd = hwnd;

    Caustic::SystemStartup();

    // Create the Caustic factory. This object is used to create
    // new objects in the renderer.
    m_spCausticFactory = Caustic::CreateCausticFactory();

    // The shaderFolder is the path to where the compiled shaders are
    // located. SHADERPATH is set by the .vcproj to point to:
    //     $(CausticRoot)\$(Configuration)
    std::wstring shaderFolder(SHADERPATH);

    // Create our render window. This is a window bound to an HWND
    // that has a renderer attached to it (the renderer will display
    // its results in the HWND). It also will handle mouse input for
    // features such as the trackball.
    // The CreateRenderWindow is passed two lambda functions which
    // are callbacks by the renderer. The first is called when the current
    // frame is about to render the scene. The second is called just before
    // Present().
    BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
    m_spRenderWindow = CreateRenderWindow(hwnd, viewport, shaderFolder,
            [this](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)
            {
                if (pass != Caustic::c_PassOpaque)
                    return;

                auto groundPlaneTM = DirectX::XMMatrixScaling(10.0f, 10.0f, 1.0f) * DirectX::XMMatrixRotationX(Caustic::DegreesToRadians(90.0f));
                if (m_spGroundPlaneRenderMesh != nullptr)
                    m_spGroundPlaneRenderMesh->Render(pRenderer, pRenderCtx, (IRenderMaterial*)nullptr, nullptr, m_lights, &groundPlaneTM);
                auto sphereTM = DirectX::XMMatrixTranslation(0.0f, 1.0f, 0.0f);
                if (m_spSphereRenderMesh != nullptr)
                    m_spSphereRenderMesh->Render(pRenderer, pRenderCtx, (IRenderMaterial*)nullptr, nullptr, m_lights, &sphereTM);

                //std::any lightPos(Float3(lightPos.x, lightPos.y, lightPos.z));
                //spBokehShader->SetPSParam(L"lightPos", lightPos);
                //std::any lightClr(Float3(lightClr.x, lightClr.y, lightClr.z));
                //spBokehShader->SetPSParam(L"lightClr", lightClr);
            },
            [](IRenderer* pRenderer)
            {
            },
            true);

    // When we called CreateRenderWindow() we told it to create the renderer
    // in a frozen state (not rendering anything). To start everything back up
    // we now call Unfreeze()
    m_spRenderWindow->GetRenderer()->Unfreeze();

    // We have to call Setup3DScene after we unfreeze the renderer.
    // The reason is that Setup3DScene will call ToRenderMesh which
    // will marshall the call to the renderer, but if the renderer
    // is frozen it will block until its unfrozen (i.e. deadlock).
    Setup3DScene(m_spRenderWindow);
}

//**********************************************************************
// Method: ShutdownCaustic
// This method is called to shutdown the rendering engine when
// the application exits
//**********************************************************************
void CApp::ShutdownCaustic()
{
    //        m_spDirectionalLight = nullptr;
    //        m_spPointLight = nullptr;
    //        m_spGroundPlaneMaterial = nullptr;
    //        m_spGroundPlaneRenderMesh = nullptr;
    //        m_spGroundPlaneMesh = nullptr;
    //        m_spSphereMaterial = nullptr;
    //        m_spSphereRenderMesh = nullptr;
    //        m_spSphere = nullptr;
    m_spCausticFactory = nullptr;
    m_spRenderWindow = nullptr;
    Caustic::SystemShutdown();
}
