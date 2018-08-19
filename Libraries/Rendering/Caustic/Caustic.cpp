//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Rendering\Caustic\Caustic.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>

namespace Caustic
{
    //**********************************************************************
    //! \brief CausticSetup is used to initialize global settings in the caustic
    //! renderer. All clients must call this function.
    //**********************************************************************
    CAUSTICAPI void CausticSetup()
    {
        CoInitializeEx(0, COINIT_MULTITHREADED);
        CT(MFStartup(MF_VERSION, MFSTARTUP_FULL));
    }

    //**********************************************************************
    //! \brief CausticShutdown is the opposite of CausticSetup(). All clients
    //! must call this function before exiting.
    //**********************************************************************
    CAUSTICAPI void CausticShutdown()
    {
        CT(MFShutdown());
        CoUninitialize();
    }
}
