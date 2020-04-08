//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Geometry\GeomDS\IKDTree.h"
#include "Base\Math\Vector.h"
#include <vector>
#include <functional>

namespace Caustic
{
    enum CutAxis
    {
        XAxis = 0,
        YAxis = 1,
        ZAxis = 2
    };

    //**********************************************************************
    // CKDTreeNode defines a single cutting plane in the tree
    //**********************************************************************
    struct CKDTreeNode
    {
        double m_cutPoint;                                          // Values < m_cutPoint are added to the left child (m_pLower), while >= are added to the right child (m_pUpper)
        CKDTreeNode *m_pLower;                                      // Left child (points < than m_cutPoint)
        CKDTreeNode *m_pUpper;                                      // Right child (points >= than m_cutPoint)
        std::vector<std::tuple<Caustic::Vector3,void*>> *m_pPoints; // List of points in this leaf node

        CKDTreeNode() :
            m_cutPoint(0.0f),
            m_pLower(nullptr),
            m_pUpper(nullptr)
        {
            m_pPoints = new std::vector<std::tuple<Caustic::Vector3, void*>>();
        }
    };
 
    //**********************************************************************
    // CKDTree defines a K-D tree used for storing/lookup of vertex data
    //**********************************************************************
    class CKDTree : public IKDTree, public CRefCount
    {
        CKDTreeNode *m_pChildren;       // Nodes in our tree
        int m_maxPoints;                // Maximum # of points allowed in any leaf node

        void AddPointRecursive(CKDTreeNode *pNode, CutAxis cutAxis, Caustic::Vector3 &vec, void *data);
        bool FindPointRecursive(CKDTreeNode *pNode, CutAxis cutAxis, Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data);
    public:
        //**********************************************************************
        CKDTree(int maxPoints) :
            m_pChildren(nullptr),
            m_maxPoints(maxPoints)
        {
        }

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IKDTree
        //**********************************************************************
        void AddPoint(Caustic::Vector3 &vec, void *data);
        bool FindPoint(Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data);
    };
    
    CAUSTICAPI void CreateKDTree(IKDTree **ppKDTree);
}
