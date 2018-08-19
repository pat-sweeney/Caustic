//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
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
    //! \brief Defines an interface for interacting with the KD tree
    //!
    //! IKDTree defines an interface for manipulating a KD Tree.
    //**********************************************************************
    struct IKDTree : public IRefCount
    {
        virtual void AddPoint(Caustic::Vector3 &vec, void *data) = 0;
        virtual bool FindPoint(Caustic::Vector3 &vec, std::function<bool(void *data)> comparator, void **data) = 0;
    };
}