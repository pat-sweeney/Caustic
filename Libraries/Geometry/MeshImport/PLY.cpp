//**********************************************************************
// Copyright Patrick Sweeney 2018-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <map>
#include <string>
#include <memory>
#include <vector>

module Geometry.MeshImport.PLY;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.Mesh;
import Geometry.Mesh.IMesh;
import Geometry.Mesh.MeshConstructor;
import Geometry.Mesh.IMeshConstructor;
import Geometry.MeshImport;
import Geometry.Mesh.MeshFuncs;
import Geometry.Mesh.RenderTypes;

namespace Caustic
{
    class CPlyParser
    {
    public:
        CRefObj<IMesh> m_spMesh;

        void ReadLine(const char *&pData, std::string &line, bool returnEmptyLines);
    public:
        CPlyParser();

        void ParseAscii(const wchar_t *pFilename, const char *pData);
    };

    CPlyParser::CPlyParser()
    {
    }

    void CPlyParser::ReadLine(const char *&pData, std::string &line, bool returnEmptyLines)
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

    void CPlyParser::ParseAscii(const wchar_t *pFilename, const char *pData)
    {
        //**********************************************************************
        // For now we take a very simplistic approach and assume all PLY files
        // contain vertices in order x/y/z/otherData and faces in the format
        // <vertexCount==3> index0 index1 index2
        //**********************************************************************

        //**********************************************************************
        // Parse header
        //**********************************************************************
        int numVerts = 0;
        int numFaces = 0;
        std::string line;
        while (*pData)
        {
            ReadLine(pData, line, false);
            if (line.substr(0, 14) == "element vertex")
            {
                numVerts = atoi(line.substr(15).c_str());
            }
            else if (line.substr(0, 12) == "element face")
            {
                numFaces = atoi(line.substr(13).c_str());
            }
            else if (line == "end_header")
                break;
        }

        //**********************************************************************
        // Parse the vertex list
        //**********************************************************************
        std::vector<CGeomVertex> vertices;
        vertices.reserve(numVerts);
        CGeomVertex v;
        ZeroMemory(&v, sizeof(v));
        for (int i = 0; i < numVerts; i++)
        {
            ReadLine(pData, line, false);
            const char *p = line.c_str();
            while (isspace(*p))
                p++;
            sscanf_s(p, "%f %f %f", &v.pos.x, &v.pos.y, &v.pos.z);
            v.index = i;
            vertices.push_back(v);
        }

        //**********************************************************************
        // Parse face list
        //**********************************************************************
        std::vector<int> faces;
        faces.reserve(numFaces);
        for (int i = 0; i < numFaces; i++)
        {
            ReadLine(pData, line, false);
            const char *p = line.c_str();
            while (isspace(*p))
                p++;
            int count, index0, index1, index2;
            sscanf_s(p, "%d %d %d %d", &count, &index0, &index1, &index2);
            faces.push_back(index0);
            faces.push_back(index1);
            faces.push_back(index2);
        }
        if (numFaces > 0)
        {
            m_spMesh = Caustic::CreateEmptyMesh();
            CRefObj<ISubMesh> spSubMesh = Caustic::CreateSubMesh(vertices, faces, 0);
            m_spMesh->AddSubMesh(spSubMesh);
        }
        m_spMesh->ComputeNormals();
    }

    namespace MeshImport
    {
        //**********************************************************************
        // Function: LoadPLY
        // Loads a mesh from a .ply file
        //
        // Parameters:
        // pFilename - path to .ply file
        //
        // Returns:
        // IMesh object
        //**********************************************************************
        CRefObj<IMesh> LoadPLY(const wchar_t *pFilename)
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
            CPlyParser parser;
            parser.m_spMesh = Caustic::CreateEmptyMesh();
            parser.ParseAscii(pFilename, (const char*)spBuffer.get());
            return parser.m_spMesh;
        }
    };
};
