//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
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
		// data - user data to associate with the point. This data is saved in
        // the tree along with the point. During calls to FindPoint this data
        // will be returned to the client.
		//**********************************************************************
		virtual void AddPoint(Caustic::Vector3 &vec, void *data) = 0;

		//**********************************************************************
		// Method: FindPoint
		// Checks if the specified point is in the tree
		//
		// Parameters:
		// vec - point to find
		// comparator - user defined comparison function
		// data - user data associated with found point
		//
		// Returns:
		// True if point is found in tree. False otherwise.
		//**********************************************************************
		virtual bool FindPoint(Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data) = 0;
    };
}