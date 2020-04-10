//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "MaterialAttrib.h"
#include "Base\Core\convertStr.h"
#include "Rendering\Caustic\CausticFactory.h"
#include <memory>

namespace Caustic
{
    //**********************************************************************
    // Function: CreateMaterial
    // Creates a new IMaterialAttrib object
    //
    // Parameters:
    // ambientColor - Normalized [0..1] ambient color value
    // diffuseColor - Normalized [0..1] diffse color value
    // specularColor - Normalized [0..1] specular color value
    // specularExp - Specular power value
    // alpha - Alpha value
    // ppMaterial - Returns the created material object
    //**********************************************************************
    CAUSTICAPI void CreateStandardMaterial(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float specularExp, float alpha, IMaterialAttrib** ppMaterial)
    {
        std::unique_ptr<CMaterialAttrib> spMaterial(new CMaterialAttrib());
        spMaterial->SetColor(L"ambientColor", ambientColor);
        spMaterial->SetColor(L"diffuseColor", diffuseColor);
        spMaterial->SetColor(L"specularColor", specularColor);
        spMaterial->SetScalar(L"specularExp", specularExp);
        spMaterial->SetScalar(L"alpha", alpha);
        *ppMaterial = spMaterial.release();
        (*ppMaterial)->AddRef();
    }

    //**********************************************************************
    // Function: CreateMaterial
    // Creates a new empty IMaterial object
    //
    // Parameters:
    // ppMaterial - Returns the created material object
    //**********************************************************************
    CAUSTICAPI void CreateMaterial(IMaterialAttrib** ppMaterial)
    {
        std::unique_ptr<CMaterialAttrib> spMaterial(new CMaterialAttrib());
        *ppMaterial = spMaterial.release();
        (*ppMaterial)->AddRef();
    }

    Vector3 CMaterialAttrib::GetColor(const wchar_t* pName)
    {
        std::map<std::wstring, Vector3>::iterator it = m_colors.find(std::wstring(pName));
        if (it != m_colors.end())
            return it->second;
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    void CMaterialAttrib::SetColor(const wchar_t *pName, Vector3& v)
    {
        std::map<std::wstring, Vector3>::iterator it = m_colors.find(pName);
        if (it != m_colors.end())
            it->second = v;
        else
            m_colors.insert(std::make_pair(pName, v));
    }

    float CMaterialAttrib::GetScalar(const wchar_t *pName)
    {
        std::map<std::wstring, float>::iterator it = m_scalars.find(pName);
        if (it != m_scalars.end())
            return it->second;
        return 0.0f;
    }
    
    void CMaterialAttrib::SetScalar(const wchar_t *pName, float s)
    {
        std::map<std::wstring, float>::iterator it = m_scalars.find(pName);
        if (it != m_scalars.end())
            it->second = s;
        else
            m_scalars.insert(std::make_pair(pName, s));
    }
    
    CRefObj<IImage> CMaterialAttrib::s_spDefaultTexture;
    
    CRefObj<IImage> CMaterialAttrib::GetTexture(const wchar_t *pName)
    {
        std::map<std::wstring, CRefObj<IImage>>::iterator it = m_textures.find(pName);
        if (it != m_textures.end())
            return it->second;
        if (s_spDefaultTexture == nullptr)
        {
            Caustic::CreateImage(32, 32, 32, &s_spDefaultTexture);
            CImageIter32 row(s_spDefaultTexture, 0, 0);
            for (int y = 0; y < (int)s_spDefaultTexture->GetHeight(); y++)
            {
                CImageIter32 col = row;
                for (int x = 0; x < (int)s_spDefaultTexture->GetWidth(); x++)
                {
                    col.SetRed(255);
                    col.SetBlue(255);
                    col.SetGreen(((x < 16 && y < 16) || (x >= 16 && y >= 16)) ? 0 : 255);
                    col.SetAlpha(255);
                    col.Step(CImageIter::Right);
                }
                row.Step(CImageIter::Down);
            }
        }
        return s_spDefaultTexture;
    }
    void CMaterialAttrib::SetTexture(const wchar_t *pName, IImage* pImage)
    {
        std::map<std::wstring, CRefObj<IImage>>::iterator it = m_textures.find(pName);
        if (it != m_textures.end())
            it->second = CRefObj<IImage>(pImage);
        else
            m_textures.insert(std::make_pair(pName, CRefObj<IImage>(pImage)));
    }

    void CMaterialAttrib::SetTextureViaFilename(const wchar_t *pName, std::wstring& filename)
    {
        CRefObj<IImage> spImage;
        Caustic::LoadImage(filename.c_str(), &spImage);
        SetTexture(pName, spImage);
    }

    void CMaterialAttrib::EnumerateColors(std::function<void(const wchar_t* pName, Vector3 & v)> func)
    {
        for (auto x : m_colors)
            func(x.first.c_str(), x.second);
    }
    void CMaterialAttrib::EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func)
    {
        for (auto x : m_scalars)
            func(x.first.c_str(), x.second);
    }
    void CMaterialAttrib::EnumerateTextures(std::function<void(const wchar_t* pName, IImage * pTexture)> func)
    {
        for (auto x : m_textures)
            func(x.first.c_str(), x.second);
    }
    //**********************************************************************
    // Method: Load
    // Loads an IMaterial object from the specified stream
    //
    // Parameters:
    // pStream - Stream to load from
    //**********************************************************************
    void CMaterialAttrib::Load(IStream* pStream)
    {
#if 0
        DWORD dwBytesRead;
        pStream->Read(&AmbientColor.x, sizeof(AmbientColor.x), &dwBytesRead);
        pStream->Read(&AmbientColor.y, sizeof(AmbientColor.y), &dwBytesRead);
        pStream->Read(&AmbientColor.z, sizeof(AmbientColor.z), &dwBytesRead);
        pStream->Read(&DiffuseColor.x, sizeof(DiffuseColor.x), &dwBytesRead);
        pStream->Read(&DiffuseColor.y, sizeof(DiffuseColor.y), &dwBytesRead);
        pStream->Read(&DiffuseColor.z, sizeof(DiffuseColor.z), &dwBytesRead);
        pStream->Read(&SpecularColor.x, sizeof(SpecularColor.x), &dwBytesRead);
        pStream->Read(&SpecularColor.y, sizeof(SpecularColor.y), &dwBytesRead);
        pStream->Read(&SpecularColor.z, sizeof(SpecularColor.z), &dwBytesRead);
        pStream->Read(&SpecularExp, sizeof(SpecularExp), &dwBytesRead);
        pStream->Read(&Alpha, sizeof(Alpha), &dwBytesRead);
        pStream->Read(&MaterialID, sizeof(MaterialID), &dwBytesRead);
        uint32 len;
        pStream->Read(&len, sizeof(len), &dwBytesRead);
        AmbientTextureFN.resize(len + 1);
        pStream->Read((LPVOID)AmbientTextureFN.c_str(), len, &dwBytesRead);
        pStream->Read(&len, sizeof(len), &dwBytesRead);
        DiffuseTextureFN.resize(len + 1);
        pStream->Read((LPVOID)DiffuseTextureFN.c_str(), len, &dwBytesRead);
        pStream->Read(&len, sizeof(len), &dwBytesRead);
        SpecularTextureFN.resize(len + 1);
        pStream->Read((LPVOID)SpecularTextureFN.c_str(), len, &dwBytesRead);
#endif
    }

    //**********************************************************************
    // Method: Store
    // Stores an IMaterial object to the specified stream
    //
    // Parameters:
    // pStream - Stream to store to
    //**********************************************************************
    void CMaterialAttrib::Store(IStream* pStream)
    {
#if 0
        DWORD dwBytesWritten;
        pStream->Write(&AmbientColor.x, sizeof(AmbientColor.x), &dwBytesWritten);
        pStream->Write(&AmbientColor.y, sizeof(AmbientColor.y), &dwBytesWritten);
        pStream->Write(&AmbientColor.z, sizeof(AmbientColor.z), &dwBytesWritten);
        pStream->Write(&DiffuseColor.x, sizeof(DiffuseColor.x), &dwBytesWritten);
        pStream->Write(&DiffuseColor.y, sizeof(DiffuseColor.y), &dwBytesWritten);
        pStream->Write(&DiffuseColor.z, sizeof(DiffuseColor.z), &dwBytesWritten);
        pStream->Write(&SpecularColor.x, sizeof(SpecularColor.x), &dwBytesWritten);
        pStream->Write(&SpecularColor.y, sizeof(SpecularColor.y), &dwBytesWritten);
        pStream->Write(&SpecularColor.z, sizeof(SpecularColor.z), &dwBytesWritten);
        pStream->Write(&SpecularExp, sizeof(SpecularExp), &dwBytesWritten);
        pStream->Write(&Alpha, sizeof(Alpha), &dwBytesWritten);
        pStream->Write(&MaterialID, sizeof(MaterialID), &dwBytesWritten);
        uint32 len = (uint32)AmbientTextureFN.length();
        pStream->Write(&len, sizeof(len), &dwBytesWritten);
        pStream->Write((void*)AmbientTextureFN.c_str(), len, &dwBytesWritten);
        len = (uint32)DiffuseTextureFN.length();
        pStream->Write(&len, sizeof(len), &dwBytesWritten);
        pStream->Write((void*)DiffuseTextureFN.c_str(), len, &dwBytesWritten);
        len = (uint32)SpecularTextureFN.length();
        pStream->Write(&len, sizeof(len), &dwBytesWritten);
        pStream->Write((void*)SpecularTextureFN.c_str(), len, &dwBytesWritten);
#endif
    }
}
