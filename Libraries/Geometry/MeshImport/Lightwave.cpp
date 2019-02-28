//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "MeshImport.h"
#include <memory>
#include <vector>

namespace Caustic
{
    class CLightWaveParser
    {
    protected:
        BYTE *m_pData;
        UINT m_byteCount;
        UINT m_offset;
    public:
        std::vector<Vector3> m_Verts;
        std::vector<int> m_Faces;

    public:
        struct ChunkDef;
        typedef void (CLightWaveParser::*ChunkFunc)(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize);
        struct ChunkDef
        {
            const char chunkID[5];
            CLightWaveParser::ChunkFunc func;
            ChunkDef *childNodes;
        };

        void ParseChunk(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize);
        void ParseFORM(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize);
        void ParsePNTS(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize);
        void ParseSRFS(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize);
        void ParsePOLS(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize);
        int ParseI4(BYTE *&pb);
        int ParseI2(BYTE *&pb);

        int m_FileSize;
        CLightWaveParser(BYTE *pbData, UINT byteCount);
        CRefObj<IMesh> ParseMesh();
    };
    
    CLightWaveParser::CLightWaveParser(BYTE *pbData, UINT byteCount)
    {
        m_pData = pbData;
        m_byteCount = byteCount;
        m_offset = 0;
    }
    
    int CLightWaveParser::ParseI4(BYTE *&pb)
    {
        int i = pb[0] << 24 | pb[1] << 16 | pb[2] << 8 | pb[3];
        pb += 4;
        return i;
    }

    int CLightWaveParser::ParseI2(BYTE *&pb)
    {
        int i = pb[0] << 8 | pb[1];
        pb += 2;
        return i;
    }

    void CLightWaveParser::ParsePNTS(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize)
    {
        int numVerts = chunkSize / 12;
        m_Verts.resize(numVerts);
        for (int i = 0; i < numVerts; i++)
        {
            int j = pb[0] << 24 | pb[1] << 16 | pb[2] << 8 | pb[3];
            float f = *(float*)&j;
            m_Verts[i].x = f;
            pb += 4;
            j = pb[0] << 24 | pb[1] << 16 | pb[2] << 8 | pb[3];
            f = *(float*)&j;
            m_Verts[i].y = f;
            pb += 4;
            j = pb[0] << 24 | pb[1] << 16 | pb[2] << 8 | pb[3];
            f = *(float*)&j;
            m_Verts[i].z = f;
            pb += 4;
        }
    }

    void CLightWaveParser::ParseSRFS(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize)
    {
    }

    void CLightWaveParser::ParsePOLS(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize)
    {
        while (chunkSize > 0)
        {
            int numverts = ParseI2(pb);
            _ASSERT(numverts == 3);
            int id0 = ParseI2(pb);
            int id1 = ParseI2(pb);
            int id2 = ParseI2(pb);
            int matid = ParseI2(pb);
            m_Faces.push_back(id0);
            m_Faces.push_back(id1);
            m_Faces.push_back(id2);
            chunkSize -= 10;
        }
    }

    CLightWaveParser::ChunkDef LWOBTable[] =
    {
        { "PNTS", &CLightWaveParser::ParsePNTS, nullptr },
        { "SRFS", &CLightWaveParser::ParseSRFS, nullptr },
        { "POLS", &CLightWaveParser::ParsePOLS, nullptr },
        { "    ", nullptr, nullptr }
    };

    CLightWaveParser::ChunkDef FormTable[] =
    {
        { "FORM", &CLightWaveParser::ParseFORM, LWOBTable },
        { "    ", nullptr, nullptr }
    };

    void CLightWaveParser::ParseFORM(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize)
    {
        pb += 4; // Skip LWOB
        ParseChunk(LWOBTable, pb, chunkSize - 4);
    }
    
    void CLightWaveParser::ParseChunk(ChunkDef *pDefTbl, BYTE *pb, UINT chunkSize)
    {
        UINT bytesLeft = chunkSize;
        while (bytesLeft)
        {
            int chunk = 0;
            BYTE *pbID = pb;
            pb += 4;
            UINT childChunkSize = ParseI4(pb);
            while (pDefTbl[chunk].chunkID[0] != ' ')
            {
                if (memcmp(pbID, pDefTbl[chunk].chunkID, 4) == 0)
                {
                    ((*this).*(pDefTbl[chunk].func))(pDefTbl[chunk].childNodes, pb, childChunkSize);
                    break;
                }
                chunk++;
            }
            pb += childChunkSize;
            bytesLeft -= childChunkSize + 8;
        }
    }

    namespace MeshImport
    {
        CRefObj<IMesh> LoadLightwave(const wchar_t *pFilename)
        {
            HANDLE h = CreateFile(pFilename, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
            DWORD dwSize = GetFileSize(h, nullptr);
            std::unique_ptr<BYTE[]> spBuffer(new BYTE[dwSize]);
            DWORD dwBytesRead;
            if (!ReadFile(h, spBuffer.get(), dwSize, &dwBytesRead, nullptr))
                CT(HRESULT_FROM_WIN32(GetLastError()));
            CloseHandle(h);
            CLightWaveParser parser(spBuffer.get(), dwSize);
            parser.ParseChunk(FormTable, spBuffer.get(), dwSize);
            CRefObj<Caustic::IMesh> spMesh;
            CreateEmptyMesh(&spMesh);
            CRefObj<Caustic::ISubMesh> spSubMesh;
            CreateSubMesh(parser.m_Verts, parser.m_Faces, 0, &spSubMesh);
            spMesh->AddSubMesh(spSubMesh.p);
            return spMesh;
        }
    };
};
