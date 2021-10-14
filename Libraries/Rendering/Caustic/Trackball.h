//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#define NOMINMAX 
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
#include "Caustic.h"
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "CausticFactory.h"

namespace Caustic
{
    //**********************************************************************
    // Class: CTrackball
    // Implementation of ITrackball
    //
    // Members:
    // <uint32> m_centerX - Center of screen's X coordinate
    // <uint32> m_centerY - Center of screen's Y coordinate
    // <uint32> m_width - Width of screen
    // <uint32> m_height - Height of screen
    // <uint32> m_radius - Radius of trackball
    // <uint32> m_startX - X Position of mouse when drag was started
    // <uint32> m_startY - Y Position of mouse when drag was started
    // <Vector3> m_startPos - Position of m_startX,m_startY on trackball surface
    //
    // Header:
    // [Link:Rendering/Caustic/Trackball.h]
    //**********************************************************************
    class CTrackball : public ITrackball, public CRefCount
    {
        uint32 m_centerX; // Center of screen's X coordinate
        uint32 m_centerY; // Center of screen's Y coordinate
        uint32 m_width; // Width of screen
        uint32 m_height; // Height of screen
        uint32 m_radius; // Radius of trackball
        uint32 m_startX; // X Position of mouse when drag was started
        uint32 m_startY; // Y Position of mouse when drag was started
        Vector3 m_startPos; // Position of m_startX,m_startY on trackball surface
        
        void ComputeSurfacePosition(int screenX, int screenY, Vector3 *pPos);
    public:
        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }
        
        //**********************************************************************
        // ITrackball
        //**********************************************************************
        virtual void BeginTracking(int screenX, int screenY, int screenW, int screenH) override;
        virtual bool UpdateTracking(int screenX, int screenY, ETrackballConstraint constraint, DirectX::XMMATRIX *pMatrix) override;
        virtual void EndTracking() override;
    };
};
