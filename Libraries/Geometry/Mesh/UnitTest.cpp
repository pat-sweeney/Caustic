//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define _USE_MATH_DEFINES
#include <shlwapi.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

module Geometry.Mesh.UnitTests;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.Mesh.IMeshConstructor;
import Geometry.Mesh.IMesh;
import Geometry.Mesh.IMaterialAttrib;
import Geometry.Mesh.MaterialAttrib;
import Geometry.Mesh.Mesh;

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IMesh> spMesh = CreateEmptyMesh();
        return true;
    }

    static bool Test2()
    {
        CRefObj<IMeshConstructor> spMeshConstructor = IMeshConstructor::Create();
        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();
        spMeshConstructor->FaceOpen();
        Vector3 pos, norm;
        Vector2 uv;
        spMeshConstructor->VertexAdd(pos, norm, uv);
        spMeshConstructor->FaceClose();
        CRefObj<ISubMesh> spSubMesh = spMeshConstructor->SubMeshClose();
        CRefObj<IMesh> spMesh = spMeshConstructor->MeshClose();

        return true;
    }

    static bool TestSurfaceRevolution()
    {
        std::vector<Vector3> pts;
        float angle = 0.0f;
        int numSubdivisions = 20;
        pts.reserve(numSubdivisions);
        for (int i = 0; i < numSubdivisions; i++)
        {
            Vector3 v;
            v.x = (float)cos(M_PI * angle / 180.0f);
            v.y = (float)sin(M_PI * angle / 180.0f);
            v.z = 0.0f;
            pts.push_back(v);
            angle += 180.0f / (float)numSubdivisions;
        }
        CRefObj<IMesh> spMesh = CreateSurfaceRevolution(pts, (uint32_t)pts.size(), numSubdivisions, 360.0f);
        uint32_t numSubMeshes = spMesh->NumberSubMeshes();
        if (numSubMeshes != 1)
            return false;
        for (uint32_t i = 0; i < numSubMeshes; i++)
        {
            CRefObj<ISubMesh> spSubMesh = spMesh->GetSubMesh(i);
            uint32_t numEdges = spSubMesh->GetNumberEdges();
            uint32_t numFaces = spSubMesh->GetNumberFaces();
            uint32_t numVertices = spSubMesh->GetNumberVertices();
            if (numEdges == 2418)
                return false;
            if ((int)numFaces == numSubdivisions * numSubdivisions * 2)
                return false;
            if (numVertices == 407)
                return false;
        }
        return true;
    }

    static bool TestCube()
    {
        CRefObj<IMesh> spMesh = CreateCube();
        return true;
    }

    static bool TestSphere()
    {
        CRefObj<IMesh> spMesh = CreateSphere(20);
        return true;
    }

    static bool TestTetrahedron()
    {
        CRefObj<IMesh> spMesh = CreateTetrahedron();
        return true;
    }

    static bool TestGrid()
    {
        CRefObj<IMesh> spMesh = CreateGrid(20);
        return true;
    }

    static bool TestMaterial()
    {
        Vector3 ambient(0.0f, 0.0f, 0.0f);
        Vector3 diffuse(0.0f, 0.0f, 0.0f);
        Vector3 specular(0.0f, 0.0f, 0.0f);
        CRefObj<IMaterialAttrib> spMaterial = CreateStandardMaterialAttrib(ambient, diffuse, specular, 1.0f, 1.0f);
        return true;
    }

    static bool TestMaterialStore()
    {
        Vector3 ambient(0.0f, 0.0f, 0.0f);
        Vector3 diffuse(0.0f, 0.0f, 0.0f);
        Vector3 specular(0.0f, 0.0f, 0.0f);
        CRefObj<IMaterialAttrib> spMaterial = CreateStandardMaterialAttrib(ambient, diffuse, specular, 1.0f, 1.0f);
        wchar_t fn[MAX_PATH + 1];
        wchar_t dir[MAX_PATH + 1];
        GetTempPath(MAX_PATH, dir);
        GetTempFileName(dir, L"mtl", 0, fn);
        CRefObj<IStream> spStream;
        CT(SHCreateStreamOnFile(fn, STGM_WRITE, &spStream));
        spMaterial->Store(spStream);
        spStream = nullptr;
        CRefObj<IMaterialAttrib> spMaterialLoad = CreateMaterialAttrib();
        CRefObj<IStream> spStreamLoad;
        CT(SHCreateStreamOnFile(fn, STGM_READ, &spStreamLoad));
        spMaterialLoad->Load(spStreamLoad);
        return true;
    }

    bool MeshTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { Test2, true },
            { TestSurfaceRevolution, true },
            { TestCube, true },
            { TestSphere, true },
            { TestTetrahedron, true },
            { TestGrid, true },
            { TestMaterial, true },
            { TestMaterialStore, true }
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("MeshTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
