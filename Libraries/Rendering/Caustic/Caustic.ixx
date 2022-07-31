//**********************************************************************
// Copyright Patrick Sweeney 2015-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfcaptureengine.h>

export module Caustic.Base;
import Base.Core.Core;
import Base.Core.Error;
export namespace Caustic
{
    //**********************************************************************
    // Function: SystemStartup
    // This should only be called once when the application starts
    //**********************************************************************
    void SystemStartup()
    {
        CoInitializeEx(0, COINIT_MULTITHREADED);
        CT(MFStartup(MF_VERSION, MFSTARTUP_FULL));
    }

    //**********************************************************************
    // Function: SystemShutdown
    // This should only be called once when the application shutsdown
    //**********************************************************************
    void SystemShutdown()
    {
        MFShutdown();
        CoUninitialize();
    }
}
