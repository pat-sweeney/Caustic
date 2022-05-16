//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
export module Rendering.RenderGraph.RenderGraphDiagnostics;
export namespace Caustic
{
#ifdef TURN_ON_DIAGNOSTICS
    void SpewWarning(const wchar_t* msg, ...)
    {
        wchar_t buffer[1024];
        swprintf_s(buffer, __VA_ARGS__);
        OutputDebugString(buffer);
        OutputDebugString(L"\n");
    }
#else // TURN_ON_DIAGNOSTICS
    void SpewWarning(const wchar_t* msg, ...)
    {
    }
#endif // TURN_ON_DIAGNOSTICS
}