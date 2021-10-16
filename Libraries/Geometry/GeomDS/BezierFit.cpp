//**********************************************************************
// This code is based on:
// An algorithm for automatically fitting digitzed curves
// by Philip J. Schneider
// Graphic Gems
//**********************************************************************
#include "stdafx.h"
#include "BezierFit.h"
#include "Base\Math\Matrix.h"
import Base.Math.Basis;

namespace Caustic
{
	CBezierFit::CBezierFit()
	{
	}

	void CBezierFit::ComputeTangentVectors(std::vector<Vector2>& points, int startPt, int endPt, float tolerance,
		Vector2 *tangentDir0, Vector2 *tangentDir1)
	{
		// Compute approximate tangents at endpoints
		int numPts = (int)points.size();
		for (int i = startPt; i <= endPt; i++)
		{
			auto delta = points[i] - points[0];
			if (delta.Length() < tolerance)
				continue;
			*tangentDir0 = delta.Normalize();
			break;
		}
		for (int i = numPts - 1; i >= startPt; i--)
		{
			auto delta = points[i] - points[numPts - 1];
			if (delta.Length() < tolerance)
				continue;
			*tangentDir1 = delta.Normalize();
			break;
		}
	}
	
	void CBezierFit::ComputeParameterization(std::vector<float>& parameterization, std::vector<Vector2>& points, int startPt, int endPt)
	{
		float totalLength = 0.0f;
		for (int i = startPt + 1; i <= endPt; i++)
		{
			auto delta = points[i] - points[i - 1];
			totalLength += delta.Length();
		}
		for (int i = startPt + 1; i <= endPt; i++)
		{
			auto delta = points[i] - points[i - 1];
			float t = delta.Length() / totalLength;
			parameterization.push_back(t);
		}
	}

	// Here we will compute 4 control points that match our points in a least squares sense.
	// The start and end control points will match the start and end points in out input points.
	// Given two tangent vectors (tangentDir0 & tangentDir1) we want to find V1 and V2 such that
	//    V1=Alpha1 * T1 + V0
	//    V2=Alpha2 * T2 + V3
	// We want to minimize:
	//    S=Sum{Data[i] - Q(Ui)}^2
	// For complete derivation see Graphics Gems page 618
	float CBezierFit::FitBezierSegment(std::vector<Vector2>& points, int startPt, int endPt,
		Vector2& tangentDir0, Vector2& tangentDir1,
		std::vector<float> &parameterization, Vector2* controlPoints, int *ptOfMaxDeviation)
	{
		float Ai1SqSum = 0.0f;
		float Ai2SqSum = 0.0f;
		float Ai1Ai2Sum = 0.0f;
		float X1Sum = 0.0f;
		float X2Sum = 0.0f;
		for (int i = startPt; i <= endPt; i++)
		{
			float B0 = BersteinBasis(0, parameterization[i]);
			float B1 = BersteinBasis(1, parameterization[i]);
			float B2 = BersteinBasis(2, parameterization[i]);
			float B3 = BersteinBasis(3, parameterization[i]);
			Vector2 Ai1 = tangentDir0 * B1;
			Vector2 Ai2 = tangentDir1 * B2;
			Ai1SqSum += Ai1.Dot(Ai1);
			Ai1Ai2Sum += Ai1.Dot(Ai2);
			Ai2SqSum += Ai2.Dot(Ai2);
			auto t0 = points[startPt] * B0;
			auto t1 = points[startPt] * B1;
			auto t2 = points[endPt] * B2;
			auto t3 = points[endPt] * B3;
			auto tsum = t0 + t1 + t2 + t3;
			X1Sum = (points[i] - tsum).Dot(Ai1);
			X2Sum = (points[i] - tsum).Dot(Ai2);
		}

		// Now using Cramer's Rule solve for alpha1 and alpha2
		Matrix2x2 mat;
		mat.v[0][0] = Ai1SqSum; mat.v[0][1] = Ai1Ai2Sum;
		mat.v[1][0] = Ai1Ai2Sum; mat.v[1][1] = Ai2SqSum;
		float denominator = mat.Determinant();
		mat.v[0][0] = X1Sum; mat.v[0][1] = X2Sum;
		mat.v[1][0] = Ai1Ai2Sum; mat.v[1][1] = Ai2SqSum;
		float numerator = mat.Determinant();
		float alpha1 = numerator / denominator;
		
		mat.v[0][0] = Ai1SqSum; mat.v[0][1] = Ai1Ai2Sum;
		mat.v[1][0] = X1Sum; mat.v[1][1] = X2Sum;
		numerator = mat.Determinant();
		float alpha2 = numerator / denominator;

		Vector2 tv0 = tangentDir0 * alpha1;
		Vector2 tv1 = tangentDir1 * alpha2;
		controlPoints[0] = points[startPt] + tv0;
		controlPoints[1] = points[endPt] + tv1;
		controlPoints[2] = points[endPt];

		// Compute error
		float totalError = 0.0f;
		float maxDist = 0.0f;
		for (int i = startPt; i <= endPt; i++)
		{
			float B0 = BersteinBasis(0, parameterization[i]);
			float B1 = BersteinBasis(1, parameterization[i]);
			float B2 = BersteinBasis(2, parameterization[i]);
			float B3 = BersteinBasis(3, parameterization[i]);
			Vector2 V0 = points[startPt] * B0;
			Vector2 V1 = controlPoints[0] * B1;
			Vector2 V2 = controlPoints[1] * B2;
			Vector2 V3 = controlPoints[2] * B3;
			Vector2 delta = (V0 + V1 + V2 + V3) - points[i];
			float dist = delta.Length();
			if (dist > maxDist)
			{
				maxDist = dist;
				*ptOfMaxDeviation = i;
			}
			totalError += dist;
		}
		return (float)sqrt(totalError / (endPt - startPt + 1));
	}

	void CBezierFit::AdjustParameterization(std::vector<Vector2>& points, int startPt, int endPt, std::vector<float> &parameterization, Vector2 *controlPoints)
	{
		// So, the description in Graphics Gems for the Newton-Raphson step
		// is super confusing. We want to minimize the distance between a sample point
		// and its corresponding point on the curve. This is what Eq. 8 in the article
		// is referring to.
		// Thus we have:
		//     Q = is the point on the curve at parameter t (e.g. Q(t))
		//     Q1 = first derivative of Q(t)
		//     Q2 = second derivative of Q(t)
		// We want to solve:
		//     (Q-P) . Q1 = 0
		// That is we want the distance between the point on the curve and the sample
		// point to be perpendicular to the tangent at that point.
		// In the article it is not clear how the author gets Eq. 9
		// Looking at the sample code it isn't even clear that that equation is used at all.
		std::vector<float> updatedParameterization;
		updatedParameterization.resize(parameterization.size());
		for (int i = startPt; i <= endPt; i++)
		{
			float B0 = BersteinBasis(0, parameterization[i]);
			float B1 = BersteinBasis(1, parameterization[i]);
			float B2 = BersteinBasis(2, parameterization[i]);
			float B3 = BersteinBasis(3, parameterization[i]);
			Vector2 V0 = controlPoints[0] * B0;
			Vector2 V1 = controlPoints[1] * B1;
			Vector2 V2 = controlPoints[2] * B2;
			Vector2 V3 = controlPoints[3] * B3;
			Vector2 q = V0 + V1 + V2 + V3;

			Vector2 q1[3];
			q1[0].x = (controlPoints[1].x - controlPoints[0].x) * 3.0f;
			q1[0].y = (controlPoints[1].y - controlPoints[0].y) * 3.0f;
			q1[1].x = (controlPoints[2].x - controlPoints[1].x) * 3.0f;
			q1[1].y = (controlPoints[2].y - controlPoints[1].y) * 3.0f;
			q1[2].x = (controlPoints[3].x - controlPoints[2].x) * 3.0f;
			q1[2].y = (controlPoints[3].y - controlPoints[2].y) * 3.0f;

			Vector2 q2[2];
			q2[0].x = (q1[1].x - q1[0].x) * 2.0f;
			q2[0].y = (q1[1].y - q1[0].y) * 2.0f;
			q2[1].x = (q1[2].x - q1[1].x) * 2.0f;
			q2[1].y = (q1[2].y - q1[1].y) * 2.0f;

			float invu = 1 - parameterization[i];
			V0 = q1[0] * invu * invu;
			V1 = q1[1] * 2 * parameterization[i] * invu;
			V2 = q1[2] * parameterization[i] * parameterization[i];
			Vector2 q1u = V0 + V1 + V2;

			V0 = q2[0] * invu;
			V1 = q2[1] * parameterization[i];
			Vector2 q2u = V0 + V1;

			float numerator = (q.x - points[i].x) * q1u.x + (q.y - points[i].y) * q1u.y;
			float denominator = q1u.x * q1u.x + q1u.y * q1u.y + (q.x - points[i].x) * q2u.x + (q.y - points[i].y) * q2u.y;
			updatedParameterization[i] = parameterization[i] - numerator / denominator;
		}
		parameterization = updatedParameterization;
	}

	void CBezierFit::FitCubicBezier(std::vector<Vector2>& points, int startPt, int endPt,
		Vector2& tangentDir0, Vector2& tangentDir1, float tolerance)
	{
		if (endPt - startPt + 1 == 2)
		{
			path->AppendPathElems(EPathElem::LineTo, points[endPt].x, points[endPt].y, EPathElem::Null);
			return;
		}
		std::vector<float> parameterization;
		ComputeParameterization(parameterization, points, startPt, endPt);
		Vector2 controlPoints[3];
		int maxPt;
		float error = FitBezierSegment(points, startPt, endPt,
			tangentDir0, tangentDir1, parameterization, controlPoints, &maxPt);
		if (error < tolerance)
		{
			path->AddPathElem(EPathElem::CurveTo, &controlPoints[0], &controlPoints[1], &controlPoints[2]);
			return;
		}
		if (error < tolerance * tolerance)
		{
			// Attempt to refine our point parameterization via Newton-Raphson
			const int maxIterations = 5;
			for (int i = 0; i < maxIterations; i++)
			{
				AdjustParameterization(points, startPt, endPt, parameterization, controlPoints);
				error = FitBezierSegment(points, startPt, endPt, tangentDir0, tangentDir1, parameterization, controlPoints, &maxPt);
				if (error < tolerance)
				{
					path->AddPathElem(EPathElem::CurveTo, &controlPoints[0], &controlPoints[1], &controlPoints[2]);
					return;
				}
			}
		}
		Vector2 newTangentDir0;
		Vector2 newTangentDir1;
		ComputeTangentVectors(points, startPt, maxPt, tolerance, &newTangentDir0, &newTangentDir1);
		FitCubicBezier(points, startPt, maxPt, newTangentDir0, newTangentDir1, tolerance);
		ComputeTangentVectors(points, maxPt, endPt, tolerance, &newTangentDir0, &newTangentDir1);
		FitCubicBezier(points, maxPt, endPt, newTangentDir0, newTangentDir1, tolerance);
	}

	CRefObj<IPath2> CBezierFit::FitPoints(std::vector<Vector2>& points, float tolerance)
	{
		Vector2 tangentDir0;
		Vector2 tangentDir1;
		ComputeTangentVectors(points, 0, (int)points.size() - 1, tolerance, &tangentDir0, &tangentDir1);
		FitCubicBezier(points, 0, (int)points.size() - 1, tangentDir0, tangentDir1, tolerance);
		return path;
	}
}