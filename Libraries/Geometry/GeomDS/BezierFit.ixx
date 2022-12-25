//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Geometry.GeomDS.BezierFit;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.GeomDS.IPath2;
import Base.Core.IRefCount;

export namespace Caustic
{
    // Given a set of points, this class will fit a series of cubic Beziers
    // to those points. The algorithm used is based on:
    //        An Algorithm For Automatically Fitting Digitized Curves
    //        Philip J. Schneider
    //        Graphics Gems
    class CBezierFit
    {
        CRefObj<IPath2> path;

        void ComputeTangentVectors(std::vector<Vector2>& points, int startPt, int endPt, float tolerance,
            Vector2* tangentDir0, Vector2* tangentDir1);
        float FitBezierSegment(std::vector<Vector2>& points, int startPt, int endPt, Vector2& tangentDir0, Vector2& tangentDir1, std::vector<float>& parameterization, Vector2* controlPoints, int *maxPt);
        void FitCubicBezier(std::vector<Vector2>& points, int startPt, int endPt, Vector2 &tangentDir0, Vector2 &tangentDir1, float tolerance);
        void ComputeParameterization(std::vector<float>& parameterization, std::vector<Vector2>& points, int startPt, int endPt);
        void AdjustParameterization(std::vector<Vector2>& points, int startPt, int endPt, std::vector<float>& parameterization, Vector2* controlPoints);
    public:
        CBezierFit();
        CRefObj<IPath2> FitPoints(std::vector<Vector2>& points, float tolerance);
    };
}
