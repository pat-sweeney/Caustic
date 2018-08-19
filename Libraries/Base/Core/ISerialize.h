//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Core.h"
#include "IRefCount.h"
#include <atlbase.h>

namespace Caustic
{
    //**********************************************************************
    //! \brief Defines an interface for serializing out our objects
    //!
    //! ISerialize defines an interface for serializing out objects to some
    //! stream. 
    //**********************************************************************
    struct ISerialize : public IRefCount
    {
        virtual void Load(IStream *pStream) = 0;
        virtual void Store(IStream *pStream) = 0;
    };
}
