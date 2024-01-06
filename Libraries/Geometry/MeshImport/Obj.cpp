//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//
// This file defines an implementation of .obj file format.
// For complete details on the format see:
// http://paulbourke.net/dataformats/mtl/
//**********************************************************************
module;
#include <Windows.h>
#include <ShlWapi.h>
#include <string>
#include <vector>
#include <map>
#include <d3d11.h>
#include <memory>
#include <any>

module Geometry.MeshImport.Obj;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.ConvertStr;
import Base.Math.Vector;
import Geometry.Mesh.MaterialAttrib;
import Geometry.MeshImport;
import Imaging.Color;

namespace Caustic
{
    class CObjParser
    {
    public:
        std::string m_Folder;
        std::string m_materialFn;
        std::vector<Vector3> m_Verts;
        std::vector<Vector3> m_Norms;
        std::vector<Vector3> m_UVs;

        std::vector<CGeomVertex> m_Vertices;
        std::vector<int> m_FaceVertIndices;

        std::string m_groupName;
        int m_smoothingGroup;
        std::string m_materialName;
        bool m_normalsSpecified;
        std::map<std::string, CRefObj<IMaterialAttrib>> m_matmap;
        CRefObj<IMesh> m_spMesh;

        void ReadLine(const char *&pData, std::string &line, bool returnEmptyLines);
        void ParseVector3(const char *p, std::vector<Vector3> &data);
        void ParseFace(const char *p);
        void ParseMaterial(const char *pData, std::map<std::wstring, std::any>* pDefaultMaterialValues);
        void SetupDefaultMaterialValues(IMaterialAttrib* pMaterial, std::map<std::wstring, std::any>* pDefaultMaterialValues);
    public:
        CObjParser();

        void ParseAscii(const wchar_t *pFilename, const char *pData, std::map<std::wstring, std::any>* pDefaultMaterialValues = nullptr);
    };

    CObjParser::CObjParser() :
        m_normalsSpecified(false)
    {
    }

    void CObjParser::ReadLine(const char *&pData, std::string &line, bool returnEmptyLines)
    {
        line.clear();
        if (!returnEmptyLines)
        {
            while (*pData == '\n' || *pData == '\r')
                pData++;
        }
        while (*pData && *pData != '\n' && *pData != '\r')
        {
            line += *pData;
            pData++;
        }
        if (returnEmptyLines)
        {
            while (*pData == '\n' || *pData == '\r')
                pData++;
        }
    }

    void CObjParser::ParseVector3(const char *p, std::vector<Vector3> &data)
    {
        Vector3 pos;
        while (isspace(*p))
            p++;
        sscanf_s(p, "%f %f %f", &pos.x, &pos.y, &pos.z);
        data.push_back(pos);
    }

    void CObjParser::ParseFace(const char *p)
    {
        const char *orig = p;
        int vIndex[4] = { 0 }, tIndex[4] = { 0 }, nIndex[4] = { 0 };
        int count = 0;
        while (*p && *p != '\r' && *p != '\n' && count < 4)
        {
            while (isspace(*p))
                p++;
            if (!*p || *p == '\r' || *p == '\n')
                break;
            sscanf_s(p, "%d", &vIndex[count]);
            while (*p && !isspace(*p) && *p != '/')
                p++;
            if (*p == '/')
            {
                p++;
                sscanf_s(p, "%d", &tIndex[count]);
                while (*p && !isspace(*p) && *p != '/')
                    p++;
                if (*p == '/')
                {
                    p++;
                    sscanf_s(p, "%d", &nIndex[count]);
                    while (*p && !isspace(*p))
                        p++;
                }
            }
            count++;
        }

        auto AddVertex = [&](int i)
        {
            CGeomVertex vert;
            vert.pos = m_Verts[vIndex[i] - 1];
            //vert.pos.x = -vert.pos.x;
            if (m_UVs.empty() || tIndex[i] == 0)
                vert.uvs[0] = Vector2(0.0f, 0.0f);
            else
                vert.uvs[0] = Vector2(m_UVs[tIndex[i] - 1].x, 1.0f - m_UVs[tIndex[i] - 1].y);
            if (m_Norms.empty() || nIndex[i] == 0)
                vert.norm = Vector3(0.0f, 0.0f, 0.0f);
            else
                vert.norm = m_Norms[nIndex[i] - 1];
            m_Vertices.push_back(vert);
            m_FaceVertIndices.push_back((int)m_Vertices.size() - 1);
        };

        // Verify winding order
        Vector3 v0 = m_Verts[vIndex[1] - 1] - m_Verts[vIndex[0] - 1];
        Vector3 v1 = m_Verts[vIndex[2] - 1] - m_Verts[vIndex[0] - 1];
        Vector3 n;
        if (m_Norms.empty() || nIndex[0] == 0)
            n = Vector3(0.0f, 0.0f, 0.0f);
        else
            n = m_Norms[nIndex[0] - 1];
        bool flipWindingOrder = (v0.Cross(v1).Dot(n) < 0) ? true : false;
        if (count == 3)
        {
            for (int i = 0; i < 3; i++)
            {
                AddVertex((flipWindingOrder) ? 2 - i : i);
            }
        }
        else
        {
            int order[2][3] = { { 0, 1, 2 }, { 0, 2, 3 }};
            for (int k = 0; k < 2; k++)
            {
                CFace face;
                for (int i = 0; i < 3; i++)
                {
                    AddVertex((flipWindingOrder) ? order[k][2 - i] : order[k][i]);
                }
            }
        }
    }

    //**********************************************************************
    // Method: SetupDefaultMaterialValues
    // Assigns the default material values to the material. If a default material
    // is specified in 'pDefaultMaterialValues' then that value is used, otherwise
    // the default value will be used.
    // 
    // Parameters:
    // pMaterial - material object to set default values on
    // pDefaultMaterialValues - list of default material values
    //**********************************************************************
    void CObjParser::SetupDefaultMaterialValues(IMaterialAttrib* pMaterial, std::map<std::wstring, std::any>* pDefaultMaterialValues)
    {
        // Setup initial values
        auto spBlackImage = Caustic::CreateImage(1024, 1024, EImageType::RGBA_32bpp);
        ZeroMemory(spBlackImage->GetData(), spBlackImage->GetHeight() * spBlackImage->GetStride());
        FRGBAColor white(1.0f, 1.0f, 1.0f, 1.0f);
        FRGBAColor black(0.0f, 0.0f, 0.0f, 1.0f);

        auto SetDefaultTexture = [&](const wchar_t *propName) {
            CRefObj<IImage> spImage;
            std::map<std::wstring, std::any>::iterator it;
            if (pDefaultMaterialValues != nullptr &&
                (it = pDefaultMaterialValues->find(propName)) != pDefaultMaterialValues->end())
                spImage = std::any_cast<CRefObj<IImage>>(it->second);
            else
                spImage = spBlackImage;
            pMaterial->SetTexture(propName, spImage, EShaderAccess::PixelShader);
        };
        auto SetDefaultColor = [&](const wchar_t* propName, const wchar_t* propAvailName, FRGBAColor defaultColor) {
            pMaterial->SetScalar(propAvailName, 0.0f);
            FRGBAColor value;
            std::map<std::wstring, std::any>::iterator it;
            if (pDefaultMaterialValues != nullptr &&
                (it = pDefaultMaterialValues->find(propName)) != pDefaultMaterialValues->end())
            {
                value = std::any_cast<FRGBAColor>(it->second);
                pMaterial->SetScalar(propAvailName, 1.0f);
            }
            else
                value = defaultColor;
            pMaterial->SetColor(propName, value);
        };
        auto SetDefaultScalar = [&](const wchar_t* propName, const wchar_t* propAvailName) {
            pMaterial->SetScalar(propAvailName, 0.0f);
            float value;
            std::map<std::wstring, std::any>::iterator it;
            if (pDefaultMaterialValues != nullptr &&
                (it = pDefaultMaterialValues->find(propName)) != pDefaultMaterialValues->end())
            {
                value = std::any_cast<float>(it->second);
                pMaterial->SetScalar(propAvailName, 1.0f);
            }
            else
                value = 0.0f;
            pMaterial->SetScalar(propName, value);
        };
        SetDefaultTexture(L"ambientTexture");
        SetDefaultTexture(L"diffuseTexture");
        SetDefaultTexture(L"specularTexture");
        SetDefaultTexture(L"specularExpTexture");
        SetDefaultTexture(L"transparencyTexture");
        SetDefaultTexture(L"reflectionTexture");
        SetDefaultTexture(L"emissiveTexture");
        SetDefaultTexture(L"bumpTexture");
        SetDefaultColor(L"ambientColor", L"ambientAvail", FRGBAColor(0.1f, 0.1f, 0.1f, 1.0f));
        SetDefaultColor(L"diffuseColor", L"diffuseAvail", white);
        SetDefaultColor(L"specularColor", L"specularAvail", white);
        SetDefaultColor(L"specularExp", L"specularExpAvail", white);
        SetDefaultColor(L"transparency", L"transparencyAvail", white);
        SetDefaultColor(L"reflectionColor", L"reflectionAvail", white);
        SetDefaultColor(L"emissiveColor", L"emissiveAvail", black);
        SetDefaultScalar(L"bumpFactor", L"bumpAvail");
        std::map<std::wstring, std::any>::iterator it;
        if (pDefaultMaterialValues != nullptr &&
            (it = pDefaultMaterialValues->find(L"transparent")) != pDefaultMaterialValues->end())
            pMaterial->SetIsTransparent(std::any_cast<bool>(it->second));
        else
            pMaterial->SetIsTransparent(true);
    }

    void CObjParser::ParseMaterial(const char *pData, std::map<std::wstring, std::any> *pDefaultMaterialValues)
    {
        CRefObj<IMaterialAttrib> spMaterial;
        uint32_t materialID = 0;
        while (true)
        {
            std::string line;
            ReadLine(pData, line, true);
            if (line.empty())
                break;
            if (line.substr(0, 6) == "newmtl")
            {
                std::string matname = line.substr(7);
                spMaterial = CreateMaterialAttrib();
                spMaterial->SetName(matname.c_str());
                spMaterial->SetMaterialID(materialID);
                materialID++;
                m_matmap.insert(std::make_pair(matname, spMaterial));
                SetupDefaultMaterialValues(spMaterial, pDefaultMaterialValues);
            }
            else
            {
                const char *p = line.c_str();
                while (isspace(*p))
                    p++;
                if (_strnicmp(p, "map_Ka", 6) == 0)
                {
                    p += 6;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"ambientTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"ambientAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_Kd", 6) == 0)
                {
                    p += 6;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"diffuseTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"diffuseAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_Ks", 6) == 0)
                {
                    p += 6;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"specularTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"specularAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_Ns", 6) == 0)
                {
                    p += 6;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"specularExpTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"specularExpAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_d", 5) == 0)
                {
                    p += 5;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"transparencyTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"transparencyAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_refl", 8) == 0 || _strnicmp(p, "refl", 4) == 0)
                {
                    p += ((p[0] == 'm') ? 8 : 4);
                    while (isspace(*p))
                        p++;
                    if (_strnicmp(p, "-type", 5) == 0)
                    {
                        p += 5;
                        while (isspace(*p))
                            p++;
                        if (_strnicmp(p, "sphere", 6) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 0.0f);
                            p += 6;
                        }
                        if (_strnicmp(p, "cube_top", 8) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 1.0f);
                            p += 8;
                        }
                        if (_strnicmp(p, "cube_bottom", 11) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 2.0f);
                            p += 11;
                        }
                        if (_strnicmp(p, "cube_front", 10) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 3.0f);
                            p += 10;
                        }
                        if (_strnicmp(p, "cube_back", 9) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 4.0f);
                            p += 9;
                        }
                        if (_strnicmp(p, "cube_left", 9) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 5.0f);
                            p += 9;
                        }
                        if (_strnicmp(p, "cube_right", 10) == 0)
                        {
                            spMaterial->SetScalar(L"reflType", 6.0f);
                            p += 10;
                        }
                        while (!isspace(*p))
                            p++;
                        while (isspace(*p))
                            p++;
                    }
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"reflectionTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"reflectionAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_Ke", 6) == 0)
                {
                    p += 6;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"emissiveTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"emissiveAvail", 1.0f);
                }
                else if (_strnicmp(p, "map_bump", 8) == 0 || _strnicmp(p, "bump", 4) == 0)
                {
                    p += ((p[0] == 'm') ? 8 : 4);
                    while (isspace(*p))
                        p++;
                    float bumpScalar = 1.0f;
                    if (_strnicmp(p, "-bm", 3) == 0)
                    {
                        p += 3;
                        while (isspace(*p))
                            p++;
                        bumpScalar = (float)atof(p);
                        while (!isspace(*p))
                            p++;
                        while (isspace(*p))
                            p++;
                    }
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"bumpTexture", wfn, EShaderAccess::PixelShader);
                    spMaterial->SetScalar(L"bumpAvail", 1.0f);
                    spMaterial->SetScalar(L"bumpFactor", bumpScalar);
                }
                else if (*p == 'N' && p[1] == 's')
                {
                    float v = (float)atof(p + 3);
                    FRGBAColor c(v, v, v, v);
                    spMaterial->SetColor(L"specularExp", c);
                }
                else if (*p == 'K' && p[1] == 'a')
                {
                    FRGBColor v;
                    p += 3;
                    v.r = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.g = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.b = (float)atof(p);
                    spMaterial->SetColor(L"ambientColor", v);
                }
                else if (*p == 'K' && p[1] == 's')
                {
                    FRGBColor v;
                    p += 3;
                    v.r = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.g = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.b = (float)atof(p);
                    spMaterial->SetColor(L"specularColor", v);
                }
                else if (*p == 'K' && p[1] == 'd')
                {
                    FRGBColor v;
                    p += 3;
                    v.r = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.g = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.b = (float)atof(p);
                    spMaterial->SetColor(L"diffuseColor", v);
                }
                else if (*p == 'd')
                {
                    p += 2;
                    float transparency = (float)atof(p);
                    FRGBAColor c(transparency, transparency, transparency, transparency);
                    spMaterial->SetColor(L"transparency", c);
                }
            }
        }
    }

    //**********************************************************************
    // Method: ParseAscii
    // Loads an ASCII encoded OBJ file
    // 
    // Parameters:
    // pFilename - Path to the OBJ file. This is only used for resolving paths to textures
    // pData - the actual contents of the OBJ file
    // pDefaultMaterialValues - map of default material parameters
    //**********************************************************************
    void CObjParser::ParseAscii(const wchar_t *pFilename, const char *pData, std::map<std::wstring, std::any> *pDefaultMaterialValues /* = nullptr */)
    {
        wchar_t wfn[MAX_PATH];
        wcscpy_s(wfn, pFilename);
        PathRemoveFileSpec(wfn);
        size_t len = wcslen(wfn);
        wfn[len] = L'\\';
        wfn[len+1] = L'\0';
        std::wstring ws(wfn);
        m_Folder = Caustic::wstr2str(ws);

        bool foundFace = false;
        auto FlushSubMesh = [&]()
        {
            if (m_FaceVertIndices.size() > 0)
            {
                int materialID = (m_matmap.find(m_materialName) == m_matmap.end()) ? 0 : m_matmap[m_materialName]->GetMaterialID();
                CRefObj<ISubMesh> spSubMesh = Caustic::CreateSubMesh(m_Vertices, m_FaceVertIndices, materialID);
                m_spMesh->AddSubMesh(spSubMesh);
                spSubMesh->SetName(m_groupName.c_str());
                m_Vertices.clear();
                m_FaceVertIndices.clear();
                foundFace = false;
            }
        };
        while (*pData)
        {
            std::string line;
            ReadLine(pData, line, false);
            if (line.substr(0, 6) == "mtllib")
            {
                std::string fn = m_Folder + line.substr(7);
                HANDLE h = CreateFileA(fn.c_str(), FILE_GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
                DWORD dwSize = GetFileSize(h, nullptr);
                std::unique_ptr<BYTE[]> spBuffer(new BYTE[dwSize + 1]);
                DWORD dwBytesRead;
                if (!ReadFile(h, spBuffer.get(), dwSize, &dwBytesRead, nullptr))
                    CT(HRESULT_FROM_WIN32(GetLastError()));
                CloseHandle(h);
                spBuffer.get()[dwSize] = '\0';
                ParseMaterial((const char*)spBuffer.get(), pDefaultMaterialValues);
                std::vector<CRefObj<IMaterialAttrib>> materials;
                for (auto m : m_matmap)
                {
                    // m_matmap is indexed based on materialName=>IMaterialAttrib. We want
                    // materials to be indexed based on MaterialID=>IMaterialAttrib. Thus we
                    // need to get the materialID from m and use it as the index into materials.
                    uint32_t materialID = m.second->GetMaterialID();
                    if (materialID >= materials.size())
                        materials.resize(materialID + 1, nullptr);
                    materials[materialID] = m.second;
                }
                m_spMesh->SetMaterials(materials);
            }
            else if (line[0] == 'v' && line[1] == 'n')
            {
                ParseVector3(line.c_str() + 2/* skip 'vn' */, m_Norms);
                m_normalsSpecified = true;
            }
            else if (line[0] == 'v' && line[1] == 't')
                ParseVector3(line.c_str() + 2/* skip 'vt' */, m_UVs);
            else if (line[0] == 'v')
                ParseVector3(line.c_str() + 1/* skip 'v' */, m_Verts);
            else if (line[0] == 'g' && line.length() > 2)
            {
                if (foundFace)
                    FlushSubMesh();
                m_groupName = line.substr(2);
            }
            else if (line.substr(0, 6) == "usemtl")
            {
                if (foundFace)
                    FlushSubMesh();
                m_materialName = line.substr(7);
            }
            else if (line[0] == 's')
            {
                if (foundFace)
                    FlushSubMesh();
                m_smoothingGroup = atoi(line.c_str() + 1/*skip 's'*/);
            }
            else if (line[0] == 'f')
            {
                foundFace = true;
                ParseFace(line.c_str() + 1/*skip 'f'*/);
            }
        }
        FlushSubMesh();
    }
    namespace MeshImport
    {
        //**********************************************************************
        // Function: LoadObj
        // Loads a mesh from a .obj file
        //
        // Parameters:
        // pFilename - path to .obj file
        // pDefaultMaterialValues - list of default material values
        //
        // Returns:
        // IMesh object
        //**********************************************************************
        CRefObj<IMesh> LoadObj(const wchar_t *pFilename, std::map<std::wstring, std::any>* pDefaultMaterialValues /* = nullptr */)
        {
            HANDLE h = CreateFile(pFilename, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
            if (h == INVALID_HANDLE_VALUE)
                CT(HRESULT_FROM_WIN32(GetLastError()));
            DWORD dwSize = GetFileSize(h, nullptr);
            std::unique_ptr<BYTE[]> spBuffer(new BYTE[dwSize + 1]);
            DWORD dwBytesRead;
            if (!ReadFile(h, spBuffer.get(), dwSize, &dwBytesRead, nullptr))
                CT(HRESULT_FROM_WIN32(GetLastError()));
            CloseHandle(h);
            spBuffer.get()[dwSize] = '\0';
            CObjParser parser;
            parser.m_spMesh = Caustic::CreateEmptyMesh();
            parser.ParseAscii(pFilename, (const char*)spBuffer.get(), pDefaultMaterialValues);
            if (!parser.m_normalsSpecified)
                parser.m_spMesh->ComputeNormals();
            return parser.m_spMesh;
        }
    };
};
