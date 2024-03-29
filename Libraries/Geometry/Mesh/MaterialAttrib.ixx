//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <map>
#include <string>
#include <functional>

export module Geometry.Mesh.MaterialAttrib;
import Base.Core.Core;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Geometry.Mesh.IMaterialAttrib;

export namespace Caustic
{
    //**********************************************************************
    // Structure: CMaterialAttrib
    // Defines the basic material
    //
    // Module:
    // {Link:import Rendering.Caustic.MaterialAttrib;{Rendering/Caustic/MaterialAttrib.ixx}}
    //**********************************************************************
    struct CMaterialAttrib : public IMaterialAttrib, public CRefCount
    {
        std::string m_name;
        std::map<std::wstring, FRGBAColor> m_colors;
        std::map<std::wstring, float> m_scalars;
        std::map<std::wstring, std::pair<CRefObj<IImage>, EShaderAccess>> m_textures;
        static CRefObj<IImage> s_spDefaultTexture;
        int m_materialID;                 // Defines the material ID associated with this material
        bool m_isTransparent;
        bool m_isShadowReceiver;
        D3D11_CULL_MODE m_cullMode;
        D3D11_FILL_MODE m_fillMode;

        CMaterialAttrib() :
            m_materialID(0),
            m_isTransparent(false),
            m_isShadowReceiver(true),
            m_cullMode(D3D11_CULL_BACK),
            m_fillMode(D3D11_FILL_MODE::D3D11_FILL_SOLID)
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IMaterialAttrib
        //**********************************************************************
        virtual void SetName(const char* pName) override { m_name = std::string(pName); }
        virtual std::string GetName() override { return m_name; }
        virtual void SetCullMode(D3D11_CULL_MODE cullMode) override { m_cullMode = cullMode; }
        virtual D3D11_CULL_MODE GetCullMode() override { return m_cullMode; }
        virtual void SetFillMode(D3D11_FILL_MODE fillmode) override { m_fillMode = fillmode; }
        virtual D3D11_FILL_MODE GetFillMode() override { return m_fillMode; }
        virtual bool GetIsTransparent() override { return m_isTransparent; }
        virtual void SetIsTransparent(bool isTransparent) override { m_isTransparent = isTransparent; }
        virtual bool GetIsShadowReceiver() override { return m_isShadowReceiver; }
        virtual void SetIsShadowReceiver(bool receivesShadow) override { m_isShadowReceiver = receivesShadow; }
        virtual FRGBColor GetColor(const wchar_t* pName) override;
        virtual void SetColor(const wchar_t* pName, FRGBColor& v) override;
        virtual void SetColor(const wchar_t* pName, FRGBAColor& v) override;
        virtual float GetScalar(const wchar_t* pName) override;
        virtual void SetScalar(const wchar_t *pName, float s) override;
        virtual CRefObj<IImage> GetTexture(const wchar_t* pName) override;
        virtual void SetTexture(const wchar_t* pName, IImage* pImage, EShaderAccess access) override;
        virtual void SetTextureViaFilename(const wchar_t* pName, std::wstring& filename, EShaderAccess access) override;
        virtual uint32_t GetMaterialID() override { return m_materialID; }
        virtual void SetMaterialID(uint32_t v) override { m_materialID = v; }
        virtual void EnumerateColors(std::function<void(const wchar_t* pName, FRGBAColor& v)> func) override;
        virtual void EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func) override;
        virtual void EnumerateTextures(std::function<void(const wchar_t* pName, IImage * pTexture, EShaderAccess access)> func) override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override;
        virtual void Store(IStream* pStream) override;
    };

    //**********************************************************************
    // Function: CreateStandardMaterial
    // Creates a standard material (as specified in the Phong lighting model)
    //
    // Parameters:
    // ambientColor - ambient color
    // diffuseColor - diffuse color
    // specularColor - specular color
    // specularExp - value of exponent in specular
    // alpha - transparency value
    //
    // Returns:
    // Returns the new material object
    //
    // Module:
    // {Link:import Rendering.Caustic.IMaterialAttrib;{Rendering/Caustic/IMaterialAttrib.ixx}}
    //**********************************************************************
    CRefObj<IMaterialAttrib> CreateStandardMaterialAttrib(FRGBColor ambientColor, FRGBColor diffuseColor,
        FRGBColor specularColor, float specularExp, float alpha);

    //**********************************************************************
    // Function: CreateMaterial
    // Creates an empty material object
    //
    // Returns:
    // Returns the new material object
    //
    // Module:
    // {Link:import Rendering.Caustic.IMaterialAttrib;{Rendering/Caustic/IMaterialAttrib.ixx}}
    //**********************************************************************
    CRefObj<IMaterialAttrib> CreateMaterialAttrib();
}
