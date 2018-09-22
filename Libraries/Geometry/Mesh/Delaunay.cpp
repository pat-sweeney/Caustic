//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Base\Math\Helper.h"
#include <map>
#include "Delaunay.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Adds a new point to our current Delaunay triangulation using
    //! the Bowyer-Watson algorithm.
    //**********************************************************************
    void CDelaunay2::AddPoint(Vector2 &pt)
    {
        std::vector<int> badTriangles;
        std::map<std::tuple<int, int>, int> edges;
        for (int i = 0; i < triangles.size(); i++)
        {
            if (!triangles[i].inuse)
                continue;
            int i0 = triangles[i].i0;
            int i1 = triangles[i].i1;
            int i2 = triangles[i].i2;
            if (PointInTriangleCircumcircle(points[triangles[i].i0], points[triangles[i].i1], points[triangles[i].i2], pt))
            {
                std::tuple<int, int> t = std::make_tuple(i0, i1);
                if (edges.find(t) == edges.end())
                    edges[t] = 0;
                else
                    edges[t] = edges[t] + 1;
                t = std::make_tuple(i1, i2);
                if (edges.find(t) == edges.end())
                    edges[t] = 0;
                else
                    edges[t] = edges[t] + 1;
                t = std::make_tuple(i2, i0);
                if (edges.find(t) == edges.end())
                    edges[t] = 0;
                else
                    edges[t] = edges[t] + 1;
                triangles[i].inuse = false;
                badTriangles.push_back(i);
            }
            points.push_back(pt);
            for (auto edge : edges)
            {
                if (edge.second > 1)
                {
                    Triangle tri;
                    tri.i0 = std::get<0>(edge.first);
                    tri.i1 = std::get<1>(edge.first);
                    tri.i2 = (int)points.size() - 1;
                    tri.inuse = true;
                    triangles.push_back(tri);
                }
            }
        }
        // Find unshared edges in bad triangles
    }

    CAUSTICAPI void CreateDelaunay2(IDelaunay2 **ppDelaunay)
    {
        std::unique_ptr<CDelaunay2> spDelaunay(new CDelaunay2());
        *ppDelaunay = spDelaunay.release();
        (*ppDelaunay)->AddRef();
    }
}
