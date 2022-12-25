//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneOverlay2DElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.IShader;
import Rendering.Caustic.ITexture;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
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

    CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader* pShader = nullptr);
}
