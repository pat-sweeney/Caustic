//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "IMaterialAttrib.h"
#include "Base\Core\RefCount.h"
#include "Imaging\Image\Image.h"
#include "Imaging\Image\ImageIter.h"
#include <vector>

namespace Caustic
{
    //**********************************************************************
    // Structure: CMaterialAttrib
    // Defines the basic material
    //**********************************************************************
    struct CMaterialAttrib : public IMaterialAttrib, public CRefCount
    {
        Caustic::Vector3 AmbientColor;  // Defines the ambient color
        Caustic::Vector3 DiffuseColor;  // Defines the diffuse color
        Caustic::Vector3 SpecularColor; // Defines the specular color
        float SpecularExp;              // Defines the specular exponent
        float Alpha;                    // Defines an alpha blending value (0=transparent,1.0=opaque)
        int MaterialID;                 // Defines the material ID associated with this material
        CRefObj<IImage> AmbientTexture;   // Texture image for the ambient component
        CRefObj<IImage> DiffuseTexture;   // Texture image for the diffuse component
        CRefObj<IImage> SpecularTexture;  // Texture image for the specular component
        std::string AmbientTextureFN;   // Name of an texture image file for the ambient component
        std::string DiffuseTextureFN;   // Name of an texture image file for the diffuse component
        std::string SpecularTextureFN;  // Name of an texture image file for the specular component

        CMaterialAttrib() :
            AmbientColor(0.1f, 0.1f, 0.1f),
            DiffuseColor(0.7f, 0.7f, 0.7f),
            SpecularColor(1.0f, 1.0f, 1.0f),
            SpecularExp(60.0f),
            Alpha(1.0f),
            MaterialID(0)
        {
            // Create dummy textures
            CRefObj<IImage> spDefaultImage;
            Caustic::CreateImage(32, 32, 32, &spDefaultImage);
            CImageIter32 row(spDefaultImage, 0, 0);
            for (int y = 0; y < (int)spDefaultImage->GetHeight(); y++)
            {
                CImageIter32 col = row;
                for (int x = 0; x < (int)spDefaultImage->GetWidth(); x++)
                {
                    col.SetRed(255);
                    col.SetBlue(255);
                    col.SetGreen(((x < 16 && y < 16) || (x >= 16 && y >= 16)) ? 0 : 255);
                    col.SetAlpha(255);
                    col.Step(CImageIter::Right);
                }
                row.Step(CImageIter::Down);
            }
            AmbientTexture = spDefaultImage;
            DiffuseTexture = spDefaultImage;
            SpecularTexture = spDefaultImage;
        }

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IMaterialAttrib
        //**********************************************************************
        virtual Caustic::Vector3 GetAmbientColor() override { return AmbientColor; }        // Returns the current ambient color
        virtual Caustic::Vector3 GetDiffuseColor() override { return DiffuseColor; }        // Returns the current diffuse color
        virtual Caustic::Vector3 GetSpecularColor() override { return SpecularColor; }      // Returns the current specular color
        virtual float GetSpecularExp() override { return SpecularExp; }                     // Returns the current specular exponent
        virtual float GetAlpha() override { return Alpha; }                                 // Returns the current transparency value (0=transparent, 1=opaque)
        virtual uint32 GetMaterialID() override { return MaterialID; }                      // Returns the material ID associated with this material
        virtual void SetAmbientColor(Caustic::Vector3& v) override { AmbientColor = v; }    // Sets the current ambient color
        virtual void SetDiffuseColor(Caustic::Vector3& v) override { DiffuseColor = v; }    // Sets the current diffuse color
        virtual void SetSpecularColor(Caustic::Vector3& v) override { SpecularColor = v; }  // Sets the current specular color
        virtual void SetSpecularExp(float v) override { SpecularExp = v; }                  // Sets the specular exponent
        virtual void SetAlpha(float v) override { Alpha = v; }                              // Sets the transparency level (0=transparent, 1=opaque)
        virtual void SetMaterialID(uint32 v) override { MaterialID = v; }                   // Sets the material ID
        virtual void SetAmbientTexture(IImage *pImage) override { AmbientTexture = pImage; }    // Sets the ambient texture
        virtual void SetDiffuseTexture(IImage *pImage) override { DiffuseTexture = pImage; }    // Sets the diffuse texture
        virtual void SetSpecularTexture(IImage *pImage) override { SpecularTexture = pImage; }  // Sets the specular texture
        virtual void SetAmbientTextureFN(std::string p) override { AmbientTextureFN = p; }      // Sets the ambient texture from a file
        virtual void SetDiffuseTextureFN(std::string p) override { DiffuseTextureFN = p; }      // Sets the diffuse texture from a file
        virtual void SetSpecularTextureFN(std::string p) override { SpecularTextureFN = p; }    // Sets the specular texture from a file
        virtual CRefObj<IImage> GetAmbientTexture() override { return AmbientTexture; }      // Gets the ambient texture
        virtual CRefObj<IImage> GetDiffuseTexture() override { return DiffuseTexture; }      // Gets the diffuse texture
        virtual CRefObj<IImage> GetSpecularTexture() override { return SpecularTexture; }   // Gets the specular texture
        virtual std::string GetAmbientTextureFN() override { return AmbientTextureFN; }     // Returns the ambient texture filename
        virtual std::string GetDiffuseTextureFN() override { return DiffuseTextureFN; }     // Returns the diffuse texture filename
        virtual std::string GetSpecularTextureFN() override { return SpecularTextureFN; }     // Returns the specular texture filename

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override;
        virtual void Store(IStream* pStream) override;
    };
}
