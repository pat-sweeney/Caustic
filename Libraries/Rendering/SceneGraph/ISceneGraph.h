//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
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
	// PointLight - a point light
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
		PointLight,
		Material,
		ComputeShaderElem
	};

	//**********************************************************************
	// Struct: SceneCtx
	// Used to maintain state while rendering the scene graph
	//**********************************************************************
	struct SceneCtx
	{
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
		std::vector<CRefObj<IPointLight>> m_lights;

		//**********************************************************************
		// Property: m_CurrentPass
		// Defines which render pass we are in
		//**********************************************************************
		int m_CurrentPass;

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

		//**********************************************************************
		// Property: m_spGraphics
		// Defines our graphics renderer
		//**********************************************************************
		CRefObj<IGraphics> m_spGraphics;
	};

	//**********************************************************************
	// Enum: ESceneElemFlags
	//
	// Hidden - Indicates that the scene element is hidden (not rendered)
	// Selected - Indicates that the scene element is currently selected
	// BBoxDirty - (private) Indicates scene element's bounding box is dirty (changed but not updated)
	// RenderableDirty - (private) Indicates scene element was modified but render version not updated
	//**********************************************************************
	enum ESceneElemFlags
	{
		Hidden = 0x01,
		Selected = 0x02,

		// List of private flags.
		// TODO: Move this to private header
		BBoxDirty = 0x04,
		RenderableDirty = 0x08,
		MaterialDirty = 0x10
	};

	//**********************************************************************
	// Interface: ISceneElem
	// Defines our base interface common to all scene elements
	//**********************************************************************
	struct ISceneElem : public ISerialize
	{
		//**********************************************************************
		// Method: GetType
		// Returns the type of this scene element
		//**********************************************************************
		virtual ESceneElemType GetType() = 0;

		//**********************************************************************
		// Method: Name
		// Returns the name of this scene element
		//**********************************************************************
		virtual std::wstring &Name() = 0;

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
		virtual void GetBBox(BBox3 *pBBox) = 0;
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
		virtual void SetInputBuffer(const wchar_t* pBufferName, uint8 *pData, uint32 bufSize, uint32 stride) = 0;
		virtual void SetOutputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 stride) = 0;
		virtual void SetInputOutputBuffer(const wchar_t* pBufferName, uint8* pInputData, uint8* pOutputData, uint32 bufSize, uint32 stride) = 0;
		virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) = 0;
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
		virtual void SetColor(Caustic::Vector3 &clr) = 0;
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
