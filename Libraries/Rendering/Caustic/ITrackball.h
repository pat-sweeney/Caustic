//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"

//**********************************************************************
// File: ITrackball.h
// This file defines the published interface for talking to the track ball
// manipulator.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: ETrackballConstraint
    // Defines available constraints for the trackball object
    //
    // Constraint_None - Don't constrain rotations
    // Constraint_XAxis - Constrain rotations to be around X axis
    // Constraint_YAxis - Constrain rotations to be around Y axis
    // Constraint_ZAxis - Constrain rotations to be around Z axis
    //
    // Header:
    // [Link:Rendering/Caustic/ITrackball.h]
    //**********************************************************************
    enum ETrackballConstraint
    {
        Constraint_None,
        Constraint_XAxis,
        Constraint_YAxis,
        Constraint_ZAxis,
    };

    //**********************************************************************
    // Interface: ITrackball
    // Defines the interface to our trackball implementation
    //
    // Header:
    // [Link:Rendering/Caustic/ITrackball.h]
    //**********************************************************************
    struct ITrackball : public IRefCount
    {
        //**********************************************************************
        // Method: BeginTracking
        // Called to start tracking
        //
        // Parameters:
        // screenX - mouse's X position where tracking starts in pixels
        // screenY - mouse's Y position where tracking starts in pixels
        // screenW - width of the window in pixels
        // screenH - height of the window in pixels
        //**********************************************************************
        virtual void BeginTracking(int screenX, int screenY, int screenW, int screenH) = 0;

        //**********************************************************************
        // Method: UpdateTracking
        // Called when mouse moves during trackball tracking
        //
        // Parameters:
        // screenX - current mouse's X position
        // screenY - current mouse's Y position
        // constraint - current constraint
        // pMatrix - Resulting trackball matrix
        //**********************************************************************
        virtual bool UpdateTracking(int screenX, int screenY, ETrackballConstraint constraint, DirectX::XMMATRIX *pMatrix) = 0;

        //**********************************************************************
        // Method: EndTracking
        // Called when mouse is released during trackball tracking
        //**********************************************************************
        virtual void EndTracking() = 0;
    };
    
    //**********************************************************************
    // Function: CreateTrackball
    // Global function for creating a track ball. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Returns:
    // Returns the newly created trackball.
    //
    // Header:
    // [Link:Rendering/Caustic/ITrackball.h]
    //**********************************************************************
    CAUSTICAPI CRefObj<ITrackball> CreateTrackball();
}
