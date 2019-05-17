//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#define _USE_MATH_DEFINES
#include <cmath>
#include "Base\Math\Vector.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include "Rendering/Caustic/ICausticFactory.h"
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IMesh> spMesh;
        CreateEmptyMesh(&spMesh);
        return true;
    }

    static bool Test2()
    {
        CRefObj<IMeshConstructor> spMeshConstructor;
        Caustic::CreateMeshConstructor(&spMeshConstructor);
        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();
        spMeshConstructor->FaceOpen();
        Vector3 pos, norm;
        Vector2 uv;
        spMeshConstructor->VertexAdd(pos, norm, uv);
        spMeshConstructor->FaceClose();
        CRefObj<ISubMesh> spSubMesh;
        spMeshConstructor->SubMeshClose(&spSubMesh);
        CRefObj<IMesh> spMesh;
        spMeshConstructor->MeshClose(&spMesh);

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
        CRefObj<IMesh> spMesh;
        CreateSurfaceRevolution(pts, (uint32)pts.size(), numSubdivisions, 360.0f, &spMesh);
        uint32 numSubMeshes = spMesh->NumberSubMeshes();
        if (numSubMeshes != 1)
            return false;
        for (uint32 i = 0; i < numSubMeshes; i++)
        {
            CRefObj<ISubMesh> spSubMesh;
            spMesh->GetSubMesh(i, &spSubMesh);
            uint32 numEdges = spSubMesh->GetNumberEdges();
            uint32 numFaces = spSubMesh->GetNumberFaces();
            uint32 numVertices = spSubMesh->GetNumberVertices();
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
        CRefObj<IMesh> spMesh;
        CreateCube(&spMesh);
        return true;
    }

    static bool TestSphere()
    {
        CRefObj<IMesh> spMesh;
        CreateSphere(20, &spMesh);
        return true;
    }

    static bool TestTetrahedron()
    {
        CRefObj<IMesh> spMesh;
        CreateTetrahedron(&spMesh);
        return true;
    }

    static bool TestGrid()
    {
        CRefObj<IMesh> spMesh;
        Caustic::CreateGrid(20, &spMesh);
        return true;
    }

    static bool TestMaterial()
    {
        Vector3 ambient(0.0f, 0.0f, 0.0f);
        Vector3 diffuse(0.0f, 0.0f, 0.0f);
        Vector3 specular(0.0f, 0.0f, 0.0f);
        CRefObj<IMaterialAttrib> spMaterial;
		CRefObj<ICausticFactory> spFactory;
		CreateCausticFactory(&spFactory);
		spFactory->CreateMaterial(ambient, diffuse, specular, 1.0f, 1.0f, &spMaterial);
        return true;
    }

    static bool TestMaterialStore()
    {
        Vector3 ambient(0.0f, 0.0f, 0.0f);
        Vector3 diffuse(0.0f, 0.0f, 0.0f);
        Vector3 specular(0.0f, 0.0f, 0.0f);
        CRefObj<IMaterialAttrib> spMaterial;
		CRefObj<ICausticFactory> spFactory;
		CreateCausticFactory(&spFactory);
		spFactory->CreateMaterial(ambient, diffuse, specular, 1.0f, 1.0f, &spMaterial);
        wchar_t fn[MAX_PATH + 1];
        wchar_t dir[MAX_PATH + 1];
        GetTempPath(MAX_PATH, dir);
        GetTempFileName(dir, L"mtl", 0, fn);
        CRefObj<IStream> spStream;
        CT(SHCreateStreamOnFile(fn, STGM_WRITE, &spStream));
        spMaterial->Store(spStream.p);
        spStream = nullptr;
        CRefObj<IMaterialAttrib> spMaterialLoad;
		spFactory->CreateMaterial(&spMaterial);
		CRefObj<IStream> spStreamLoad;
        CT(SHCreateStreamOnFile(fn, STGM_READ, &spStreamLoad));
        spMaterialLoad->Load(spStreamLoad.p);
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
                printf("MathTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
