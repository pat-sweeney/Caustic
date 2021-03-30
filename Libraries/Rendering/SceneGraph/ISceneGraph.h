//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Rendering\Caustic\PathTrace.h"
#include "Base\Math\Matrix.h"

namespace Caustic
{
	//**********************************************************************
	// Enum: ESceneElemType
	// Defines types for each scene element
	//
	// SceneGraph - Top level scene graph
	// Mesh - Mesh element
	// Group - Group element
	// Renderable - Renderable element
	// LightCollection - Collection of lights
	// Material - material
	//**********************************************************************
	enum ESceneElemType
	{
		Unknown,
		SceneGraph,
		Mesh,
		CustomRenderElem,
		Group,
		Renderable,
		LightCollection,
		Material,
		ComputeShaderElem,
		Overlay2D,
		LineElem,
		CubeElem,
		CylinderElem,
		SphereElem
	};

	//**********************************************************************
	// Struct: SceneCtx
	// Used to maintain state while rendering the scene graph
	//**********************************************************************
	struct SceneCtx
	{
		SceneCtx()
		{
			m_Transform = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
			m_CurrentPass = c_PassFirst;
			m_inShadowLightGroup = false;
		}
		
		//**********************************************************************
		// Property: m_spDevice
		// Defines our current rendering device
		//**********************************************************************
		CComPtr<ID3D11Device> m_spDevice;

		//**********************************************************************
		// Property: m_Transform
		// Defines the current transform
		//**********************************************************************
		Matrix4x4 m_Transform;

		//**********************************************************************
		// Property: m_lights
		// Defines list of current lights effecting this portion of the scene graph
		//**********************************************************************
		std::vector<CRefObj<ILight>> m_lights;

		//**********************************************************************
		// Property: m_CurrentPass
		// Defines which render pass we are in
		//**********************************************************************
		int m_CurrentPass;

		//**********************************************************************
		// Property: m_inShadowLightGroup
		// Indicates whether we are currently rendering a group of lights+children
		// that cast shadows. Otherwise, during the shadow pass we will skip rendering.
		//**********************************************************************
		bool m_inShadowLightGroup;

		//**********************************************************************
		// Property: m_spCurrentMaterial
		// Defines the current material
		//**********************************************************************
		CRefObj<IMaterialAttrib> m_spCurrentMaterial;

		//**********************************************************************
		// Property: m_spCurrentShader
		// Defines the current shader
		//**********************************************************************
		CRefObj<Caustic::IShader> m_spCurrentShader;
	};

	//**********************************************************************
	// Enum: ESceneElemFlags
	//
	// Hidden - Indicates that the scene element is hidden (not rendered)
	// Selected - Indicates that the scene element is currently selected
	// DepthTested - Is element depth tested during rendering
	// BBoxDirty - (private) Indicates scene element's bounding box is dirty (changed but not updated)
	// RenderableDirty - (private) Indicates scene element was modified but render version not updated
	//**********************************************************************
	enum ESceneElemFlags
	{
		Hidden = 0x01,
		Selected = 0x02,
		DepthTested = 0x4,

		// List of private flags.
		// TODO: Move this to private header
		BBoxDirty = 0x040,
		RenderableDirty = 0x080,
		MaterialDirty = 0x100
	};

	//**********************************************************************
	// Interface: ISceneElem
	// Defines our base interface common to all scene elements
	//**********************************************************************
	struct ISceneElem : public ISerialize
	{
		//**********************************************************************
		// Method: RayIntersect
		// Traces a single path through the scene
		//
		// Parameters:
		// ray - ray to trace
		// pIntersection - returns ray intersection info
		// pMaterial - returns the material at the intersection point
		//
		// Returns:
		// True if ray intersects the scene element. False otherwise.
		//**********************************************************************
		virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) = 0;

		//**********************************************************************
		// Method: GetType
		// Returns the type of this scene element
		//**********************************************************************
		virtual ESceneElemType GetType() = 0;

		//**********************************************************************
		// Method: GetName
		// Returns the name of this scene element
		//
		// Returns:
		// User-defined name for this element
		//**********************************************************************
		virtual std::wstring GetName() = 0;

		//**********************************************************************
		// Method: SetName
		// Sets the name of this scene element
		//
		// Parameters:
		// name - user-defined name for this element
		//**********************************************************************
		virtual void SetName(const wchar_t *name) = 0;

		//**********************************************************************
		// Method: Name
		// Returns the name of this scene element
		//**********************************************************************
		virtual void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback) = 0;

		//**********************************************************************
		// Method: Name
		// Returns the name of this scene element
		//**********************************************************************
		virtual void SetPostRenderCallback(std::function<void(int pass)> prerenderCallback) = 0;

		//**********************************************************************
		// Method: GetFlags
		// Returns the flags associated with this scene element
		// See also: ESceneElemFlags
		//**********************************************************************
		virtual uint32 GetFlags() = 0;

		//**********************************************************************
		// Method: SetFlags
		// Sets the flags associated with this scene element
		//**********************************************************************
		virtual void SetFlags(uint32 flags) = 0;

		//**********************************************************************
		// Method: Render
		// Renders the current element
		//**********************************************************************
		virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) = 0;

		//**********************************************************************
		// Method: GetBBox
		// Returns the bounding box of this scene element
		//**********************************************************************
		virtual void GetBBox(BBox3* pBBox) = 0;

		//**********************************************************************
		// Method: SetInPass
		// Sets which render passes this element is rendered in
		//
		// Parameters:
		// pass - pass to render this element in
		//**********************************************************************
		virtual void SetInPass(uint32 pass) = 0;

		//**********************************************************************
		// Method: GetInPass
		// Gets which render passes this element is rendered in
		//
		// Returns:
		// mask indicating which passes this element is rendered in
		//**********************************************************************
		virtual uint32 GetInPass() = 0;
	};

	//**********************************************************************
	// Interface: ISceneOverlay2DElem
	// Defines a overlay element in our scene graph. Overlays are used
	// to draw 2D graphics in screen coordinates (e.g. for UI elements).
	//**********************************************************************
	struct ISceneOverlay2DElem : public ISceneElem
	{
		//**********************************************************************
		// Method: SetRect
		// Sets the screen extents for the element
		//**********************************************************************
		virtual void SetRect(BBox2& bbox) = 0;

		//**********************************************************************
		// Method: SetTexture
		// Sets the texture to render as the 2D overlay
		//**********************************************************************
		virtual void SetTexture(ITexture* pTexture) = 0;

		//**********************************************************************
		// Method: SetShader
		// Sets a shader to render the 2D overlay with. This is an override. If
		// it isn't called the default shader is used.
		//**********************************************************************
		virtual void SetShader(IShader* pShader) = 0;
	};

	//**********************************************************************
	// Interface: ISceneLineElem
	// Defines a line element in our scene graph.
	//**********************************************************************
	struct ISceneLineElem : public ISceneElem
	{
		virtual void SetPosition(Vector3& p0, Vector3& p1) = 0;
		virtual void GetPosition(Vector3 *p0, Vector3 *p1) = 0;
	};

	//**********************************************************************
	// Interface: ISceneSphereElem
	// Defines a sphere element in our scene graph.
	//**********************************************************************
	struct ISceneSphereElem : public ISceneElem
	{
		virtual void SetPosition(Vector3& center, float& radius) = 0;
		virtual void GetPosition(Vector3* pCenter, float* pRadius) = 0;
	};

	//**********************************************************************
	// Interface: ISceneCubeElem
	// Defines a cube element in our scene graph.
	//**********************************************************************
	struct ISceneCubeElem : public ISceneElem
	{
		virtual void SetPosition(Vector3& center, float width, float height, float depth) = 0;
		virtual void GetPosition(Vector3* pCenter, float* pWidth, float* pHeight, float* pDepth) = 0;
	};

	//**********************************************************************
	// Interface: ISceneCylinderElem
	// Defines a cylinder element in our scene graph.
	//**********************************************************************
	struct ISceneCylinderElem : public ISceneElem
	{
		virtual void SetPosition(Vector3& center, float height, float topRadius, float bottomRadius) = 0;
		virtual void GetPosition(Vector3* pCenter, float* pHeight, float* pTopRadius, float *pBottomRadius) = 0;
	};

	//**********************************************************************
	// Interface: ISceneGroupElem
	// Defines a group element in our scene graph. Groups contain other scene elements.
	//**********************************************************************
	struct ISceneGroupElem : public ISceneElem
	{
		//**********************************************************************
		// Method: NumberChildren
		// Returns the number of child elements part of this group
		//**********************************************************************
		virtual uint32 NumberChildren() = 0;

		//**********************************************************************
		// Method: GetChild
		// Gets the Nth child element
		//**********************************************************************
		virtual CRefObj<ISceneElem> GetChild(uint32 index) = 0;

		//**********************************************************************
		// Method: AddChild
		// Adds a new scene element to the group
		//**********************************************************************
		virtual void AddChild(ISceneElem *pElem) = 0;

		//**********************************************************************
		// Method: InsertChild
		// Adds a new scene element to the group. The order of elements defines the render order.
		//**********************************************************************
		virtual void InsertChild(ISceneElem *pElem, uint32 index) = 0;

		//**********************************************************************
		// Method: SetTransform
		// Sets the transform associated with this group.
		//**********************************************************************
		virtual void SetTransform(Matrix4x4 &transform) = 0;
	};

	//**********************************************************************
	// Interface: ISceneMaterialElem
	// Defines a material object. Materials define the vertex and pixel shader
	// associated with a given scene element.
	//**********************************************************************
	struct ISceneMaterialElem : public ISceneGroupElem
	{
		//**********************************************************************
		// Method: SetShader
		// Sets our shader
		//**********************************************************************
		virtual void SetShader(IShader *pShader) = 0;

		//**********************************************************************
		// Method: GetMaterial
		// Returns the material associated with this element
		//**********************************************************************
		virtual void GetMaterial(IMaterialAttrib **ppMaterial) = 0;

		//**********************************************************************
		// Method: SetMaterial
		// Sets the underlying material associated with the scene material
		//
		// Parameters:
		// pMaterial - material to associate with this scene material
		//**********************************************************************
		virtual void SetMaterial(IMaterialAttrib *pMaterial) = 0;
	};

	//**********************************************************************
	// Interface: ISceneComputeShaderElem
	// Defines a compute shader in our scene graph. Compute shaders are run
	// during scene graph traversal but before any rendering elements.
	//**********************************************************************
	struct ISceneComputeShaderElem : public ISceneElem
	{
		virtual CRefObj<IShader> GetShader() = 0;
		virtual void SetInputThreads(uint32 width, uint32 height, uint32 depth = 1) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, float value) = 0;
		virtual void SetBuffer(IRenderer* pRenderer, const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 elemSize) = 0;
		virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) = 0;
	};

	//**********************************************************************
	// Interface: ISceneMarchingCubesElem
	// Implements marching cubes on a signed distance function
	//**********************************************************************
	struct ISceneMarchingCubesElem : public ISceneElem
	{
		virtual CRefObj<IShader> GetShader() = 0;
		virtual void SetSubdivisions(uint32 numCells) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, float value) = 0;
		virtual void SetSignedDistanceFunction(std::function<float(Vector3&v)> fn) = 0;
	};

	//**********************************************************************
	// Interface: IScenePointLightElem
	// Defines a point light source in our scene
	//**********************************************************************
	struct IScenePointLightElem : public ISceneElem
	{
		//**********************************************************************
		// Method: SetPosition
		// Sets the position of our light
		//**********************************************************************
		virtual void SetPosition(Caustic::Vector3 &pos) = 0;

		//**********************************************************************
		// Method: SetColor
		// Defines the color of the light
		//**********************************************************************
		virtual void SetColor(Caustic::FRGBColor &clr) = 0;
	};

	//**********************************************************************
	// Interface: ISceneLightCollectionElem
	// Defines a collection of lights. These lights only effect the children of this group
	//**********************************************************************
	struct ISceneLightCollectionElem : public ISceneGroupElem
	{
		//**********************************************************************
		// Method: AddLight
		// Adds a light to the collection
		//**********************************************************************
		virtual void AddLight(ILight* pLight) = 0;

		//**********************************************************************
		// Method: RemoveLight
		// Removes a light from the collection
		//**********************************************************************
		virtual void RemoveLight(ILight* pLight) = 0;

		//**********************************************************************
		// Method: NumberLights
		// Returns the number of lights in the collection
		//**********************************************************************
		virtual uint32 NumberLights() = 0;

		//**********************************************************************
		// Method: GetLight
		// Returns the Nth light from the collection
		//**********************************************************************
		virtual CRefObj<ILight> GetLight(int index) = 0;
	};

	//**********************************************************************
	// Interface: ISceneMeshElem
	// Defines a single mesh in our scene
	//**********************************************************************
	struct ISceneMeshElem : public ISceneElem
	{
		//**********************************************************************
		// Method: SetMesh
		// Updates the mesh elements underlying mesh object
		//**********************************************************************
		virtual void SetMesh(IMesh* pMesh) = 0;

		//**********************************************************************
		// Method: SetShader
		// Assigns shader used for rendering this mesh
		//**********************************************************************
		virtual void SetShader(IShader* pShader) = 0;
	};

	//**********************************************************************
	// Interface: ISceneCustomRenderElem
	// Defines a custom render element in our scene
	//**********************************************************************
	struct ISceneCustomRenderElem : public ISceneElem
	{
		//**********************************************************************
		// Method: SetCallback
		// Updates the callback function associated with the custom render element
		//**********************************************************************
		virtual void SetCallback(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx *pSceneCtx)> clientCallback) = 0;
	};

	//**********************************************************************
	// Interface: ISceneGraph
	// Our main interface for manipulating a scene graph
	//**********************************************************************
	struct ISceneGraph : public ISceneGroupElem
	{
		//**********************************************************************
		// Method: PathTrace
		// Computes view of the scene using path tracing
		//
		// Parameters:
		// pRenderer - renderer
		// pCtx - path context
		// pDest - Image to render results to
		//**********************************************************************
		virtual void PathTrace(IRenderer *pRenderer, PathTraceCtx *pCtx, IImage *pDest) = 0;

		//**********************************************************************
		// Method: Merge
		// Merges a scene graph with another scene graph
		//**********************************************************************
		virtual void Merge(ISceneGraph *pGraph) = 0;

		//**********************************************************************
		// Method: Lock
		// Locks a scene graph so it can be updated
		//**********************************************************************
		virtual void Lock() = 0;

		//**********************************************************************
		// Method: Unlock
		// Unlocks a scene graph allowing the renderer to use it
		//**********************************************************************
		virtual void Unlock() = 0;
	};
};
