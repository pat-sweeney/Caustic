//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include <atlbase.h>
#include "Geometry\Mesh\Mesh.h"

namespace Caustic
{
    namespace MeshImport
    {
        CRefObj<IMesh> LoadLightwave(const wchar_t *pFilename);
        CRefObj<IMesh> LoadObj(const wchar_t *pFilename);
        CRefObj<IMesh> LoadPLY(const wchar_t *pFilename);
    }
}
