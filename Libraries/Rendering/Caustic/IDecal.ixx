//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.Caustic.IDecal;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Base.Math.Matrix;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: IDecal.ixx
// Defines the published interface for projected decals. A decal is an
// oriented bounding box (OBB) that projects an albedo texture onto
// underlying geometry using depth-buffer reconstruction.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Interface: IDecal
    // Represents a projected decal in the scene.
    //
    // Module:
    // {Link:import Rendering.Caustic.IDecal;{Rendering/Caustic/IDecal.ixx}}
    //**********************************************************************
    struct IDecal : public IRefCount
    {
        //**********************************************************************
        // Method: SetTransform
        // Sets the world transform for the decal OBB. The unit cube
        // [-0.5,+0.5]^3 is transformed by this matrix to produce the OBB.
        //**********************************************************************
        virtual void SetTransform(Matrix4x4& mat) = 0;

        //**********************************************************************
        // Method: GetTransform
        // Returns the world transform of the decal OBB.
        //**********************************************************************
        virtual Matrix4x4 GetTransform() = 0;

        //**********************************************************************
        // Method: SetAlbedoTexture
        // Sets the albedo/color texture to project.
        //**********************************************************************
        virtual void SetAlbedoTexture(ITexture* pTexture) = 0;

        //**********************************************************************
        // Method: GetAlbedoTexture
        // Returns the albedo texture.
        //**********************************************************************
        virtual CRefObj<ITexture> GetAlbedoTexture() = 0;

        //**********************************************************************
        // Method: SetOpacity
        // Sets the overall opacity of the decal (0..1).
        //**********************************************************************
        virtual void SetOpacity(float opacity) = 0;

        //**********************************************************************
        // Method: GetOpacity
        // Returns the decal opacity.
        //**********************************************************************
        virtual float GetOpacity() = 0;
    };

    //**********************************************************************
    // Function: CreateDecal
    // Creates a new decal.
    //
    // Parameters:
    // transform - world transform for the decal OBB
    // pAlbedoTexture - albedo texture to project (may be nullptr)
    // opacity - opacity multiplier (default 1.0)
    //
    // Module:
    // {Link:import Rendering.Caustic.IDecal;{Rendering/Caustic/IDecal.ixx}}
    //**********************************************************************
    CRefObj<IDecal> CreateDecal(Matrix4x4& transform, ITexture* pAlbedoTexture = nullptr, float opacity = 1.0f);
}
