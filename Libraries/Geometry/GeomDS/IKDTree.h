//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include <vector>
#include <functional>

namespace Caustic
{
    //**********************************************************************
    // Interface: IKDTree
    // Defines an interface for manipulating a KD Tree.
    //**********************************************************************
    struct IKDTree : public IRefCount
    {
		//**********************************************************************
		// Method: AddPoint
		// Adds a point to our KD tree
		//
		// Parameters:
		// vec - point to add to tree
		// data - user data to associate with the point
		//**********************************************************************
		virtual void AddPoint(Caustic::Vector3 &vec, void *data) = 0;

		//**********************************************************************
		// Method: FindPoint
		// Checks if the specified point is in the tree
		//
		// Parameters
		// vec - point to find
		// comparator - user defined comparison function
		// data - user data associated with found point
		//
		// Returns:
		// True if point is found in tree. False otherwise.
		//**********************************************************************
		virtual bool FindPoint(Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data) = 0;

        //**********************************************************************
        // Method: FindNearest
        //
        // Parameters
        // vec - query point
        // radius - radius around query point in which to search
        // comparator - user defined comparison function
        // points - Returns list of found points
        //
        // Returns:
        // True if points are found in tree. False otherwise.
        //**********************************************************************
        virtual void FindNearest(Caustic::Vector3 &vec, float radius, 
            std::function<bool(void *data)> comparator, std::vector<std::tuple<Caustic::Vector3, void*>> &points) = 0;
    };
}