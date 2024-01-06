//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define NOMINMAX 
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>

export module Rendering.Caustic.Trackball;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Vector;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ITrackball;

export namespace Caustic
{
    //**********************************************************************
    // Class: CTrackball
    // Implementation of ITrackball
    //
    // Members:
    // <uint32_t> m_centerX - Center of screen's X coordinate
    // <uint32_t> m_centerY - Center of screen's Y coordinate
    // <uint32_t> m_width - Width of screen
    // <uint32_t> m_height - Height of screen
    // <uint32_t> m_radius - Radius of trackball
    // <uint32_t> m_startX - X Position of mouse when drag was started
    // <uint32_t> m_startY - Y Position of mouse when drag was started
    // <Vector3> m_startPos - Position of m_startX,m_startY on trackball surface
    //
    // Module:
    // {Link:import Rendering.Caustic.Trackball;{Rendering/Caustic/Trackball.ixx}}
    //**********************************************************************
    class CTrackball : public ITrackball, public CRefCount
    {
        uint32_t m_centerX; // Center of screen's X coordinate
        uint32_t m_centerY; // Center of screen's Y coordinate
        uint32_t m_width; // Width of screen
        uint32_t m_height; // Height of screen
        uint32_t m_radius; // Radius of trackball
        uint32_t m_startX; // X Position of mouse when drag was started
        uint32_t m_startY; // Y Position of mouse when drag was started
        Vector3 m_startPos; // Position of m_startX,m_startY on trackball surface
        
        void ComputeSurfacePosition(int screenX, int screenY, Vector3 *pPos);
    public:
        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }
        
        //**********************************************************************
        // ITrackball
        //**********************************************************************
        virtual void BeginTracking(int screenX, int screenY, int screenW, int screenH) override;
        virtual bool UpdateTracking(int screenX, int screenY, ETrackballConstraint constraint, DirectX::XMMATRIX *pMatrix) override;
        virtual void EndTracking() override;
    };
};
