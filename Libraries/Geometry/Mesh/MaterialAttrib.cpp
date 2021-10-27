//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Geometry\Mesh\MaterialAttrib.h"
#include "Base\Core\convertStr.h"
#include <memory>
import Imaging.Image.Image;

namespace Caustic
{
    //**********************************************************************
    // Function: CreateMaterialAttrib
    // Creates a new IMaterialAttrib object
    //
    // Parameters:
    // ambientColor - Normalized [0..1] ambient color value
    // diffuseColor - Normalized [0..1] diffse color value
    // specularColor - Normalized [0..1] specular color value
    // specularExp - Specular power value
    // alpha - Alpha value
    //
    // Returns:
    // Returns the created material object
    //**********************************************************************
    CRefObj<IMaterialAttrib> CreateStandardMaterialAttrib(FRGBColor ambientColor, FRGBColor diffuseColor, FRGBColor specularColor, float specularExp, float alpha)
    {
        std::unique_ptr<CMaterialAttrib> spMaterial(new CMaterialAttrib());
        spMaterial->SetColor(L"ambientColor", ambientColor);
        spMaterial->SetColor(L"diffuseColor", diffuseColor);
        spMaterial->SetColor(L"specularColor", specularColor);
        spMaterial->SetScalar(L"specularExp", specularExp);
        spMaterial->SetScalar(L"alpha", alpha);
        return CRefObj<IMaterialAttrib>(spMaterial.release());
    }

    //**********************************************************************
    // Function: CreateMaterialAttrib
    // Creates a new empty IMaterial object
    //
    // Returns:
    // Returns the created material object
    //**********************************************************************
    CRefObj<IMaterialAttrib> CreateMaterialAttrib()
    {
        return CRefObj<IMaterialAttrib>(new CMaterialAttrib());
    }

    FRGBColor CMaterialAttrib::GetColor(const wchar_t* pName)
    {
        std::map<std::wstring, FRGBAColor>::iterator it = m_colors.find(std::wstring(pName));
        if (it != m_colors.end())
            return FRGBColor(it->second.r, it->second.g, it->second.b);
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    void CMaterialAttrib::SetColor(const wchar_t* pName, FRGBColor& v)
    {
        FRGBAColor clr(v.r, v.g, v.b, 1.0f);
        SetColor(pName, clr);
    }
    
    void CMaterialAttrib::SetColor(const wchar_t* pName, FRGBAColor& v)
    {
        if (v.a < 1.0f || (v.r < 1.0f && (_wcsicmp(pName, L"alpha") == 0 || _wcsicmp(pName, L"transparency") == 0)))
            SetIsTransparent(true);
        std::map<std::wstring, FRGBAColor>::iterator it = m_colors.find(pName);
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
        if (s < 1.0f && (_wcsicmp(pName, L"alpha") == 0 || _wcsicmp(pName, L"transparency") == 0))
            SetIsTransparent(true);
        std::map<std::wstring, float>::iterator it = m_scalars.find(pName);
        if (it != m_scalars.end())
            it->second = s;
        else
            m_scalars.insert(std::make_pair(pName, s));
    }
    
    CRefObj<IImage> CMaterialAttrib::s_spDefaultTexture;
    
    CRefObj<IImage> CMaterialAttrib::GetTexture(const wchar_t *pName)
    {
        std::map<std::wstring, std::pair<CRefObj<IImage>, EShaderAccess>>::iterator it = m_textures.find(pName);
        if (it != m_textures.end())
            return it->second.first;
        if (s_spDefaultTexture == nullptr)
        {
            s_spDefaultTexture = Caustic::CreateImage(32, 32, 32);
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
    
    void CMaterialAttrib::SetTexture(const wchar_t *pName, IImage* pImage, EShaderAccess access)
    {
        std::map<std::wstring, std::pair<CRefObj<IImage>,EShaderAccess>>::iterator it = m_textures.find(pName);
        if (it != m_textures.end())
            it->second = std::make_pair(CRefObj<IImage>(pImage), access);
        else
            m_textures.insert(std::make_pair(pName, std::make_pair(CRefObj<IImage>(pImage), access)));
    }

    void CMaterialAttrib::SetTextureViaFilename(const wchar_t *pName, std::wstring& filename, EShaderAccess access)
    {
        CRefObj<IImage> spImage = Caustic::LoadImageFile(filename.c_str());
        SetTexture(pName, spImage, access);
    }

    void CMaterialAttrib::EnumerateColors(std::function<void(const wchar_t* pName, FRGBAColor & v)> func)
    {
        for (auto &x : m_colors)
            func(x.first.c_str(), x.second);
    }

    void CMaterialAttrib::EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func)
    {
        for (auto &x : m_scalars)
            func(x.first.c_str(), x.second);
    }

    void CMaterialAttrib::EnumerateTextures(std::function<void(const wchar_t*pName, IImage *pImage, EShaderAccess access)> func)
    {
        for (auto &x : m_textures)
            func(x.first.c_str(), x.second.first, x.second.second);
    }

    //**********************************************************************
    // Method: Load
    // Loads an IMaterial object from the specified stream
    //
    // Parameters:
    // pStream - Stream to load from
    //**********************************************************************
    void CMaterialAttrib::Load(IStream* /* pStream */)
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
    void CMaterialAttrib::Store(IStream* /* pStream */)
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
