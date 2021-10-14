//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Geometry\GeomDS\KDTree.h"
#include <memory>
#include <tuple>
#include <algorithm>
#include <functional>

namespace Caustic
{
    //**********************************************************************
    // Given pNode recursively walk the tree until pNode is a leaf. Add the specified
    // point to that leaf.
    //**********************************************************************
    void CKDTree::AddPointRecursive(CKDTreeNode *pNode, CutAxis cutAxis, Caustic::Vector3 &vec, void *data)
    {
        // See if this node contains children nodes. If it does then recurse
        if (pNode->m_pLower != nullptr)
        {
            _ASSERT(pNode->m_pLower != nullptr);
            _ASSERT(pNode->m_pUpper != nullptr);
            float value;
            switch (cutAxis)
            {
            case CutAxis::XAxis:
                value = vec.x;
                break;
            case CutAxis::YAxis:
                value = vec.y;
                break;
            case CutAxis::ZAxis:
                value = vec.z;
                break;
            default:
                value = 0.0f;
                _ASSERT(!"This should never happen");
                break;
            }
			int nCut = cutAxis + 1;
			if (nCut > 2)
				nCut = 0;
            if (value < pNode->m_cutPoint)
                AddPointRecursive(pNode->m_pLower, (CutAxis)nCut, vec, data);
            else
                AddPointRecursive(pNode->m_pUpper, (CutAxis)nCut, vec, data);
        }
        else
        {
            // Check if the leaf node has too many points
            _ASSERT(pNode->m_pPoints != nullptr);
            if (pNode->m_pPoints->size() >= m_maxPoints)
            {
                CKDTreeNode *pLower = new CKDTreeNode();
                CKDTreeNode *pUpper = new CKDTreeNode();

                // Determine our split point. This requires sorting the data to determine the median
                std::vector<int> ordering;
                int numElems = (int)pNode->m_pPoints->size();
                ordering.resize(numElems);
                for (int i = 0; i < numElems; i++)
                    ordering[i] = i;
#if 0
                wchar_t buf[1024];
                swprintf_s(buf, L"Unsorted data:\n");
                OutputDebugString(buf);
                for (int i = 0; i < numElems; i++)
                {
                    swprintf_s(buf, L"%f %f %f\n",
                        std::get<0>((*pNode->m_pPoints)[i]).x,
                        std::get<0>((*pNode->m_pPoints)[i]).y,
                        std::get<0>((*pNode->m_pPoints)[i]).z);
                    OutputDebugString(buf);
                }
#endif
                std::sort(ordering.begin(), ordering.end(), [pNode, cutAxis](int a, int b)
                { 
                    double va, vb;
                    switch (cutAxis)
                    {
                    case CutAxis::XAxis:
                        va = std::get<0>((*pNode->m_pPoints)[a]).x;
                        vb = std::get<0>((*pNode->m_pPoints)[b]).x;
                        break;
                    case CutAxis::YAxis:
                        va = std::get<0>((*pNode->m_pPoints)[a]).y;
                        vb = std::get<0>((*pNode->m_pPoints)[b]).y;
                        break;
                    case CutAxis::ZAxis:
                        va = std::get<0>((*pNode->m_pPoints)[a]).z;
                        vb = std::get<0>((*pNode->m_pPoints)[b]).z;
                        break;
                    default:
                        _ASSERT(false);
                        va = vb = 0.0;
                        break;
                    }
                    return va < vb;
                });
#if 0
                swprintf_s(buf, L"Sorted data:\n");
                OutputDebugString(buf);
                for (int i = 0; i < numElems; i++)
                {
                    swprintf_s(buf, L"%f %f %f  %d=>%d\n",
                        std::get<0>((*pNode->m_pPoints)[ordering[i]]).x,
                        std::get<0>((*pNode->m_pPoints)[ordering[i]]).y,
                        std::get<0>((*pNode->m_pPoints)[ordering[i]]).z,
                        i,ordering[i]);
                    OutputDebugString(buf);
                }
#endif
                int cutIndex = numElems / 2;
                switch (cutAxis)
                {
                case CutAxis::XAxis:
                    pNode->m_cutPoint = std::get<0>((*pNode->m_pPoints)[ordering[cutIndex]]).x;
                    break;
                case CutAxis::YAxis:
                    pNode->m_cutPoint = std::get<0>((*pNode->m_pPoints)[ordering[cutIndex]]).y;
                    break;
                case CutAxis::ZAxis:
                    pNode->m_cutPoint = std::get<0>((*pNode->m_pPoints)[ordering[cutIndex]]).z;
                    break;
                }

                for (size_t i = 0; i < numElems; i++)
                {
                    std::vector<std::tuple<Caustic::Vector3, void*>> *ptArray = (i < cutIndex) ? pLower->m_pPoints : pUpper->m_pPoints;
                    ptArray->push_back((*pNode->m_pPoints)[ordering[i]]);
                }
                pNode->m_pLower = pLower;
                pNode->m_pUpper = pUpper;
				delete pNode->m_pPoints;
                pNode->m_pPoints = nullptr;
                AddPointRecursive(pNode, cutAxis, vec, data);
            }
            else
            {
				pNode->m_pPoints->push_back(std::make_tuple(vec, data));
            }
        }
    }

    //**********************************************************************
    void CKDTree::AddPoint(Caustic::Vector3 &vec, void *data)
    {
        if (m_pChildren == nullptr)
        {
            m_pChildren = new CKDTreeNode();
        }
        AddPointRecursive(m_pChildren, CutAxis::XAxis, vec, data);
    }

    //**********************************************************************
    bool CKDTree::FindPointRecursive(CKDTreeNode *pNode, CutAxis cutAxis, Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data)
    {
        float v;
        switch (cutAxis)
        {
		case CutAxis::XAxis:
            v = vec.x;
            break;
		case CutAxis::YAxis:
            v = vec.y;
            break;
		case CutAxis::ZAxis:
            v = vec.z;
            break;
        default:
            _ASSERT(false);
            v = 0.0f;
            break;
        }
        if (pNode->m_pUpper != nullptr)
        {
			int nCut = cutAxis + 1;
			if (nCut > 2)
				nCut = 0;
            if (v < pNode->m_cutPoint)
                return FindPointRecursive(pNode->m_pLower, (CutAxis)cutAxis, vec, comparator, data);
            else
                return FindPointRecursive(pNode->m_pUpper, (CutAxis)cutAxis, vec, comparator, data);
        }
        else
        {
			size_t nPoints = pNode->m_pPoints->size();
            for (size_t i = 0; i < nPoints; i++)
            {
                if (std::get<0>((*pNode->m_pPoints)[i]).IsEq(vec))
                {
                    bool match = true;
                    if (comparator != nullptr)
                    {
                        match = false;
                        if (comparator(std::get<1>((*pNode->m_pPoints)[i])))
                            match = true;
                    }
                    if (match)
                    {
                        if (data != nullptr)
                            *data = std::get<1>((*pNode->m_pPoints)[i]);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    //**********************************************************************
    bool CKDTree::FindPoint(Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data)
    {
        if (m_pChildren)
        {
            return FindPointRecursive(m_pChildren, CutAxis::XAxis, vec, comparator, data);
        }
		return false;
    }

    //**********************************************************************
    CRefObj<IKDTree> CreateKDTree()
    {
        return CRefObj<IKDTree>(new Caustic::CKDTree(50));
    }
}
