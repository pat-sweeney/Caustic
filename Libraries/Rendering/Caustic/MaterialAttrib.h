//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "IMaterialAttrib.h"
#include "Base\Core\RefCount.h"
#include "Imaging\Image\Image.h"
#include "Imaging\Image\ImageIter.h"
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace Caustic
{
    //**********************************************************************
    // Structure: CMaterialAttrib
    // Defines the basic material
    //
    // Header:
    // [Link:Rendering/Caustic/MaterialAttrib.h]
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

        CMaterialAttrib() :
            m_materialID(0),
            m_isTransparent(false),
            m_isShadowReceiver(true)
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IMaterialAttrib
        //**********************************************************************
        virtual void SetName(const char* pName) override { m_name = std::string(pName); }
        virtual std::string GetName() override { return m_name; }
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
        virtual uint32 GetMaterialID() override { return m_materialID; }
        virtual void SetMaterialID(uint32 v) override { m_materialID = v; }
        virtual void EnumerateColors(std::function<void(const wchar_t* pName, FRGBAColor& v)> func) override;
        virtual void EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func) override;
        virtual void EnumerateTextures(std::function<void(const wchar_t* pName, IImage * pTexture, EShaderAccess access)> func) override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override;
        virtual void Store(IStream* pStream) override;
    };
}
