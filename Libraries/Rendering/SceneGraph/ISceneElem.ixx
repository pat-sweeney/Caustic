//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

export module Rendering.SceneGraph.ISceneElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Imaging.Color;
import Base.Math.Ray;
import Base.Math.BBox;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.RendererFlags;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.SceneGraph.SceneCtx;
import Parsers.JSon.IJSonParser;

export namespace Caustic
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
		CameraCollection,
		Material,
		ComputeShaderElem,
		Overlay2D,
		LineElem,
		CubeElem,
		CylinderElem,
		SphereElem
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
		// Method: AsJson
		// Creates a JSON representation for the object
		//
		// Parameters:
		// pPropertyName - name of property
		// pParser - JSON parser object
		//
		// Returns:
		// JSON object representing the scene element
		//**********************************************************************
		virtual CRefObj<IJSonObj> AsJson(const char *pPropertyName, IJSonParser *pParser) = 0;

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
		virtual void SetName(const wchar_t* name) = 0;

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
		virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) = 0;

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
}