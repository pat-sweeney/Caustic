//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "MaterialAttrib.h"
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
    CAUSTICAPI void CreateMaterial(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float specularExp, float alpha, IMaterialAttrib** ppMaterial)
    {
        std::unique_ptr<CMaterialAttrib> spMaterial(new CMaterialAttrib());
        spMaterial->AmbientColor = ambientColor;
        spMaterial->DiffuseColor = diffuseColor;
        spMaterial->SpecularColor = specularColor;
        spMaterial->SpecularExp = specularExp;
        spMaterial->Alpha = alpha;
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

    //**********************************************************************
    // Method: Load
    // Loads an IMaterial object from the specified stream
    //
    // Parameters:
    // pStream - Stream to load from
    //**********************************************************************
    void CMaterialAttrib::Load(IStream* pStream)
    {
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
    }
}
