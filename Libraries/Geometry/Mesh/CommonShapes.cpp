//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include "Mesh.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Caustic
{
    //**********************************************************************
    // CreateSurfaceRevolution is used to generate a solid by revolving the specified
    // polygon around the Y axis. All points are assumed to be relative to the world coordinate
    // origin (thus the point specifies the point's normal vector).
    // pts - List of points (should be increasing Y value)
    // npts - Number of points in 'pts'
    // subdivisions - Number of subdivisions to generate around the Y axis
    // maxAngle - Maximum number of degrees to rotate around Y axis
    // ppMesh - Newly generated mesh
    //**********************************************************************
    CAUSTICAPI void CreateSurfaceRevolution(std::vector<Vector3> &pts, uint32 npts, uint32 subdivisions, float maxAngle, IMesh **ppMesh)
    {
        CRefObj<IMeshConstructor> spMeshConstructor;

        Caustic::CreateMeshConstructor(&spMeshConstructor);
        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();

        _ASSERT(npts >= 3);

        // Check if our first and last vertices are on the Y axis.
        // If so then use them as the top/bottom vertex and remove
        // them from the list of points (since the top/bottom triangles
        // are handled as a triangle fan...otherwise we will end up
        // with degenerate triangles (since the top/bottom will be in
        // the point list we revolve as well as the _topVertex/_bottomVertex).
        // If the starting/ending vertices are not on the Y axis then
        // add new top/bottom vertices which are on the Y axis.
        Vector3 *origPts = &pts[0];
        Vector3 _topvertex(0.0F, pts[npts - 1].y, 0.0F);
        Vector3 _bottomvertex(0.0F, pts[0].y, 0.0F);
        Vector3 *top = &_topvertex;
        Vector3 *bottom = &_bottomvertex;
        uint32 endIndex = npts - 1;
        if (IsZero(pts[0].x) && IsZero(pts[0].z))
        {
            bottom = &pts[0];
            origPts++;
            npts--;
        }
        if (IsZero(pts[endIndex].x) && IsZero(pts[endIndex].z))
        {
            top = &pts[endIndex];
            npts--;
        }
        std::unique_ptr<Vector3[]> p1(new Vector3[npts]);
        std::unique_ptr<Vector3[]> p2(new Vector3[npts]);
        std::unique_ptr<Vector2[]> t1(new Vector2[npts]);
        std::unique_ptr<Vector2[]> t2(new Vector2[npts]);
        std::unique_ptr<Vector3[]> firstpt(new Vector3[npts]);
        std::unique_ptr<Vector2[]> firstuv(new Vector2[npts]);
        Vector3 *lp = p1.get();
        Vector3 *cp = p2.get();
        Vector2 *lt = t1.get();
        Vector2 *ct = t2.get();
        //
        // Assign first row of texture coordinates
        //
        float du = 1.0F / (float)(npts + 1);
        float dv = 1.0F / (float)(subdivisions + 1);
        for (uint32 i = 0; i < (uint32)npts; i++)
            lt[i] = Vector2(0.0f, du * float(i + 1));
        float v = 0.0F;
        float angle = 0.0F;
        float delta = maxAngle / (float)subdivisions;
        for (uint32 i = 0; i <= subdivisions; i++)
        {
            if (i == subdivisions)
                angle = 0.0F;
            //
            // Rotate point list
            //
            if (i == subdivisions)
            {
                cp = firstpt.get();
                ct = firstuv.get();
            }
            else
            {
                for (uint32 j = 0; j < npts; j++)
                {
                    cp[j].x = (origPts[j].x * (float)cos(M_PI * angle / 180.0) + origPts[j].z * (float)sin(M_PI * angle / 180.0));
                    cp[j].y = origPts[j].y;
                    cp[j].z = (origPts[j].z * (float)cos(M_PI * angle / 180.0) - origPts[j].x * (float)sin(M_PI * angle / 180.0));
                    ct[j] = Vector2(v, lt[j].y);
                }
                v += dv;
            }
            if (i > 0)
            {
                // Create bottom face
                spMeshConstructor->FaceOpen();

                // Compute normal vector
                Vector3 np = (lp[0] - *bottom).Normalize();
                Vector3 nq = (cp[0] - *bottom).Normalize();
                Vector3 normal = nq.cross(np).Normalize();

                // Add face vertices
                Vector2 zeroVec(0.0f, 0.0f);
                spMeshConstructor->VertexAdd(*bottom, normal, zeroVec);
                spMeshConstructor->VertexAdd(cp[0], normal, ct[0]);
                spMeshConstructor->VertexAdd(lp[0], normal, lt[0]);

                // Close bottom face
                spMeshConstructor->FaceClose();

                // Create face list
                for (uint32 j = 1; j < npts; j++)
                {
                    // Here we create two faces on our sphere:
                    //
                    //    lp[j]            cp[j]
                    //    X----------------X
                    //    |               .|
                    //    |           .    |
                    //    |       .        |
                    //    |   .            |
                    //    X----------------X
                    //    lp[j-1]          cp[j-1]
                    spMeshConstructor->FaceOpen();
                    np = (cp[j] - lp[j - 1]).Normalize();
                    nq = (cp[j - 1] - lp[j - 1]).Normalize();
                    normal = nq.cross(np).Normalize();
                    spMeshConstructor->VertexAdd(lp[j - 1], normal, lt[j - 1]);
                    spMeshConstructor->VertexAdd(cp[j - 1], normal, ct[j - 1]);
                    spMeshConstructor->VertexAdd(cp[j], normal, ct[j]);
                    spMeshConstructor->FaceClose();

                    spMeshConstructor->FaceOpen();
                    np = (lp[j] - lp[j - 1]).Normalize();
                    nq = (cp[j] - lp[j - 1]).Normalize();
                    normal = nq.cross(np).Normalize();
                    spMeshConstructor->VertexAdd(lp[j - 1], normal, lt[j - 1]);
                    spMeshConstructor->VertexAdd(cp[j], normal, ct[j]);
                    spMeshConstructor->VertexAdd(lp[j], normal, lt[j]);
                    spMeshConstructor->FaceClose();
                }

                // Create top face
                spMeshConstructor->FaceOpen();
                uint32 j = npts - 1;

                // Compute normal vector
                np = (cp[j] - *top).Normalize();
                nq = (lp[j] - *top).Normalize();
                normal = nq.cross(np).Normalize();

                // Add face vertices
                Vector2 oneVec(1.0f, 1.0f);
                spMeshConstructor->VertexAdd(*top, normal, oneVec);
                spMeshConstructor->VertexAdd(lp[j], normal, lt[j]);
                spMeshConstructor->VertexAdd(cp[j], normal, ct[j]);

                // Close top face
                spMeshConstructor->FaceClose();
            }
            else
            {
                memcpy((void *)firstpt.get(), (void *)cp, npts * sizeof(Vector3));
                memcpy((void *)firstuv.get(), (void *)ct, npts * sizeof(Vector2));
            }
            Vector3 *tmp = lp;
            lp = cp;
            cp = tmp;
            Vector2 *tmp2 = lt;
            lt = ct;
            ct = tmp2;
            angle += delta;
        }
        spMeshConstructor->SubMeshClose(nullptr);
        spMeshConstructor->MeshClose(ppMesh);
    }

    //**********************************************************************
    // Generates a sphere mesh.
    // 
    // Creates a mesh in the shape of a sphere. 
    // subdivisions - Number of divisions along azimuth and elevation
    // ppMesh - Returns the created mesh
    //**********************************************************************
    CAUSTICAPI void CreateSphere(uint32 subdivisions, IMesh **ppMesh)
    {
        std::vector<Vector3> pts(subdivisions + 2);
        pts[0] = Vector3(0.0F, -1.0F, 0.0F);
        float delta = 180.0F / float(subdivisions + 1);
        float angle = -90.0F + delta;
        uint32 i;
        for (i = 1; i <= subdivisions; i++)
        {
            pts[i] = Vector3((float)cos(M_PI * angle / 180.0), (float)sin(M_PI * angle / 180.0), 0.0F);
            angle += delta;
        }
        pts[i] = Vector3(0.0F, 1.0F, 0.0F);
        CreateSurfaceRevolution(pts, subdivisions + 2, subdivisions, 360.0f, ppMesh);
    }

    //**********************************************************************
    // Generates a tetrahedron mesh.
    // 
    // Creates a mesh in the shape of a tetrahedro. 
    // ppMesh - Returns the created mesh
    //**********************************************************************
    CAUSTICAPI void CreateTetrahedron(IMesh **ppMesh)
    {
        Vector3 vertPos[4] =
        {
            { +1.0f, +1.0f, +1.0f },
            { -1.0f, -1.0f, +1.0f },
            { -1.0f, +1.0f, -1.0f },
            { +1.0f, -1.0f, -1.0f }
        };
        int vertFaces[4][3] =
        {
            { 2, 1, 0 },
            { 3, 2, 0 },
            { 1, 3, 0 },
            { 2, 3, 1 }
        };
        CRefObj<IMeshConstructor> spMeshConstructor;
        CreateMeshConstructor(&spMeshConstructor);
        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();
        for (int i = 0; i < (int)(sizeof(vertFaces) / sizeof(vertFaces[0])); i++)
        {
            spMeshConstructor->FaceOpen();
            Vector3 np = (vertPos[vertFaces[i][1]] - vertPos[vertFaces[i][0]]).Normalize();
            Vector3 nq = (vertPos[vertFaces[i][2]] - vertPos[vertFaces[i][0]]).Normalize();
            Vector3 normal = nq.cross(np).Normalize();
            Vector2 zeroVec(0.0f, 0.0f);
            spMeshConstructor->VertexAdd(vertPos[vertFaces[i][0]], normal, zeroVec);
            spMeshConstructor->VertexAdd(vertPos[vertFaces[i][1]], normal, zeroVec);
            spMeshConstructor->VertexAdd(vertPos[vertFaces[i][2]], normal, zeroVec);
            spMeshConstructor->FaceClose();
        }
        spMeshConstructor->SubMeshClose(nullptr);
        spMeshConstructor->MeshClose(ppMesh);
    }

    //**********************************************************************
    // Generates a grid mesh
    // 
    // Generates a grid mesh of size -1..+1,-1..+1 with texture coordinates
    // spanning the entire mesh from 0..1
    //subdivisions Number of times to subdivide the grid
    // ppMesh - Returns the created mesh
    //**********************************************************************
    CAUSTICAPI void CreateGrid(uint32 subdivisions, IMesh **ppMesh)
    {
        CRefObj<IMeshConstructor> spMeshConstructor;
        Caustic::CreateMeshConstructor(&spMeshConstructor);

        spMeshConstructor->MeshOpen();
        spMeshConstructor->SubMeshOpen();
        float dx = 2.0f / subdivisions;
        float dy = 2.0f / subdivisions;
        float cy = -1.0f;
        float dv = 1.0f / subdivisions;
        float cv = 0.0f;
        Vector3 normal(0.0f, 1.0f, 0.0f);
        for (uint32 y = 0; y < subdivisions; y++)
        {
            float tcy = cy;
            float cx = -1.0f;
            float du = 1.0f / subdivisions;
            float cu = 0.0f;
            for (uint32 x = 0; x < subdivisions; x++)
            {
                spMeshConstructor->FaceOpen();
                Vector3 vpos;
                Vector2 vuv;
                spMeshConstructor->VertexAdd(vpos = Vector3(cx, tcy, 0.0f), normal, vuv = Vector2(cu, cv));
                spMeshConstructor->VertexAdd(vpos = Vector3(cx + dx, tcy, 0.0f), normal, vuv = Vector2(cu + du, cv));
                spMeshConstructor->VertexAdd(vpos = Vector3(cx + dx, tcy + dy, 0.0f), normal, vuv = Vector2(cu + du, cv + dv));
                spMeshConstructor->FaceClose();
                spMeshConstructor->FaceOpen();
                spMeshConstructor->VertexAdd(vpos = Vector3(cx, tcy, 0.0f), normal, vuv = Vector2(cu, cv));
                spMeshConstructor->VertexAdd(vpos = Vector3(cx + dx, tcy + dy, 0.0f), normal, vuv = Vector2(cu + du, cv + dv));
                spMeshConstructor->VertexAdd(vpos = Vector3(cx, tcy + dy, 0.0f), normal, vuv = Vector2(cu, cv + dv));
                spMeshConstructor->FaceClose();
                cx += dx;
                cu += du;
            }
            cy += dy;
            cv += dv;
        }
        CRefObj<ISubMesh> spSubMesh;
        spMeshConstructor->SubMeshClose(&spSubMesh);
        spSubMesh->SetMeshFlags(EMeshFlags::TwoSided);
        spMeshConstructor->MeshClose(ppMesh);
    }

    //**********************************************************************
    // Returns a cube mesh
    // ppMesh - Returns the newly created mesh object
    //**********************************************************************
    CAUSTICAPI void CreateCube(IMesh **ppMesh)
    {
        std::vector<Vector3> vpos =
        {
            // Front face
            { -1.0f, -1.0f, -1.0f },
        { -1.0f, +1.0f, -1.0f },
        { +1.0f, +1.0f, -1.0f },
        { +1.0f, -1.0f, -1.0f },
        // Top face
        { -1.0f, +1.0f, -1.0f },
        { -1.0f, +1.0f, +1.0f },
        { +1.0f, +1.0f, +1.0f },
        { +1.0f, +1.0f, -1.0f },
        // Bottom face
        { -1.0f, -1.0f, +1.0f },
        { -1.0f, -1.0f, -1.0f },
        { +1.0f, -1.0f, -1.0f },
        { +1.0f, -1.0f, +1.0f },
        // Left face
        { -1.0f, -1.0f, +1.0f },
        { -1.0f, +1.0f, +1.0f },
        { -1.0f, +1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        // Right face
        { +1.0f, -1.0f, -1.0f },
        { +1.0f, +1.0f, -1.0f },
        { +1.0f, +1.0f, +1.0f },
        { +1.0f, -1.0f, +1.0f },
        // Back face
        { +1.0f, -1.0f, +1.0f },
        { +1.0f, +1.0f, +1.0f },
        { -1.0f, +1.0f, +1.0f },
        { -1.0f, -1.0f, +1.0f }
        };
        std::vector<Vector2> uvs =
        {
            // Front face
            { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        // Top face
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        // Bottom face
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        // Left face
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        // Right face
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        // Back face
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        };
        std::vector<Vector3> norm =
        {
            // Front face
            { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        // Top face
        { 0.0f, +1.0f, 0.0f },
        { 0.0f, +1.0f, 0.0f },
        { 0.0f, +1.0f, 0.0f },
        { 0.0f, +1.0f, 0.0f },
        // Bottom face
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        // Left face
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        // Right face
        { +1.0f, 0.0f, 0.0f },
        { +1.0f, 0.0f, 0.0f },
        { +1.0f, 0.0f, 0.0f },
        { +1.0f, 0.0f, 0.0f },
        // Back face
        { 0.0f, 0.0f, +1.0f },
        { 0.0f, 0.0f, +1.0f },
        { 0.0f, 0.0f, +1.0f },
        { 0.0f, 0.0f, +1.0f }
        };
        CRefObj<IMesh> spMesh;
        CreateEmptyMesh(&spMesh);
        std::vector<int> faces(36);
        for (int i = 0; i < 6; i++)
        {
            faces[6 * i + 0] = 4 * i + 3;
            faces[6 * i + 1] = 4 * i + 0;
            faces[6 * i + 2] = 4 * i + 1;
            faces[6 * i + 3] = 4 * i + 3;
            faces[6 * i + 4] = 4 * i + 1;
            faces[6 * i + 5] = 4 * i + 2;
        }
        CRefObj<ISubMesh> spSubMesh;
        CreateSubMesh(vpos, norm, uvs, faces, EVertexFlags(HasNormal | HasPosition | HasUV0), 0, &spSubMesh);
        spMesh->AddSubMesh(spSubMesh.p);
        *ppMesh = spMesh.p;
        (*ppMesh)->AddRef();
    }
}
