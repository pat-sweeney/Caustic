//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\convertStr.h"
#include "Rendering\Caustic\Caustic.h"
#include <atlbase.h>
#include <map>

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
        std::map<std::string, CRefObj<IMaterialAttrib>> m_matmap;
        CRefObj<IMesh> m_spMesh;

        void ReadLine(const char *&pData, std::string &line, bool returnEmptyLines);
        void ParseVector3(const char *p, std::vector<Vector3> &data);
        void ParseFace(const char *p);
        void ParseMaterial(const char *pData);
    public:
        CObjParser();

        void ParseAscii(const wchar_t *pFilename, const char *pData);
    };

    CObjParser::CObjParser()
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
        bool flipWindingOrder = false;
        if (v0.cross(v1).dot(n) < 0)
        {
            flipWindingOrder = true;
        }
        flipWindingOrder = false;

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

    void CObjParser::ParseMaterial(const char *pData)
    {
        CRefObj<IMaterialAttrib> spMaterial;
        uint32 materialID = 0;
        while (true)
        {
            std::string line;
            ReadLine(pData, line, true);
            if (line.empty())
                break;
            if (line.substr(0, 6) == "newmtl")
            {
                std::string matname = line.substr(7);
                spMaterial = CreateMaterial();
                spMaterial->SetMaterialID(materialID);
                materialID++;
                m_matmap.insert(std::make_pair(matname, spMaterial));
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
                }
                else if (_strnicmp(p, "map_Kd", 6) == 0)
                {
                    p += 6;
                    while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"diffuseTexture", wfn, EShaderAccess::PixelShader);
                }
                else if (_strnicmp(p, "map_Ks", 6) == 0)
                {
                    p += 6;
                     while (isspace(*p))
                        p++;
                    std::string fn = m_Folder + std::string(p);
                    std::wstring wfn = str2wstr(fn);
                    spMaterial->SetTextureViaFilename(L"specularTexture", wfn, EShaderAccess::PixelShader);
                }
                else if (*p == 'N' && p[1] == 's')
                    spMaterial->SetScalar(L"specularExp", (float)atof(p + 3));
                else if (*p == 'K' && p[1] == 'a')
                {
                    Vector3 v;
                    p += 3;
                    v.x = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.y = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.z = (float)atof(p);
                    spMaterial->SetColor(L"ambientColor", v);
                }
                else if (*p == 'K' && p[1] == 's')
                {
                    Vector3 v;
                    p += 3;
                    v.x = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.y = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.z = (float)atof(p);
                    spMaterial->SetColor(L"specularColor", v);
                }
                else if (*p == 'K' && p[1] == 'd')
                {
                    Vector3 v;
                    p += 3;
                    v.x = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.y = (float)atof(p);
                    while (*p && !isspace(*p))
                        p++;
                    v.z = (float)atof(p);
                    spMaterial->SetColor(L"diffuseColor", v);
                }
                else if (*p == 'd')
                {
                    p += 2;
                    float transparency = (float)atof(p);
                    spMaterial->SetScalar(L"alpha", transparency);
                }
            }
        }
    }

    void CObjParser::ParseAscii(const wchar_t *pFilename, const char *pData)
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
                CRefObj<ISubMesh> spSubMesh;
                int materialID = (m_matmap.find(m_materialName) == m_matmap.end()) ? 0 : m_matmap[m_materialName]->GetMaterialID();
                Caustic::CreateSubMesh(m_Vertices, m_FaceVertIndices, materialID, &spSubMesh);
                m_spMesh->AddSubMesh(spSubMesh);
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
                ParseMaterial((const char*)spBuffer.get());
                std::vector<CRefObj<IMaterialAttrib>> materials;
                for (auto m : m_matmap)
                    materials.push_back(m.second);
                m_spMesh->SetMaterials(materials);
            }
            else if (line[0] == 'v' && line[1] == 'n')
                ParseVector3(line.c_str() + 2/* skip 'vn' */, m_Norms);
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
        //
        // Returns:
        // IMesh object
        //**********************************************************************
        CRefObj<IMesh> LoadObj(const wchar_t *pFilename)
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
            Caustic::CreateEmptyMesh(&parser.m_spMesh);
            parser.ParseAscii(pFilename, (const char*)spBuffer.get());
            return parser.m_spMesh;
        }
    };
};
