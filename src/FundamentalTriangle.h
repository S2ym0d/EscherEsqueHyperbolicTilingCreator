#pragma once
#include <array>
#include <tuple>
#include "PoincareDiskModelPoint.h"

struct FundamentalTriangle
{
	std::array<long long, 3> pqrValues = { -1,-1,-1 };

	std::array<PoincareDiskModelPoint, 3> pqrVertices = { PoincareDiskModelPoint(),PoincareDiskModelPoint() ,PoincareDiskModelPoint() };

	PoincareDiskModelPoint incenter = PoincareDiskModelPoint();

	PoincareDiskModelPoint aSideIntouchPoint = PoincareDiskModelPoint();
	PoincareDiskModelPoint bSideIntouchPoint = PoincareDiskModelPoint();
	PoincareDiskModelPoint cSideIntouchPoint = PoincareDiskModelPoint();

	PoincareDiskModelPoint pVertexWythoffProjection = PoincareDiskModelPoint();
	PoincareDiskModelPoint qVertexWythoffProjection = PoincareDiskModelPoint();
	PoincareDiskModelPoint rVertexWythoffProjection = PoincareDiskModelPoint();

	std::array<PoincareDiskModelPoint, 2> aSideIntouchPointWythoffProjections = { PoincareDiskModelPoint() ,PoincareDiskModelPoint() };
	std::array<PoincareDiskModelPoint, 2> bSideIntouchPointWythoffProjections = { PoincareDiskModelPoint() ,PoincareDiskModelPoint() };
	std::array<PoincareDiskModelPoint, 2> cSideIntouchPointWythoffProjections = { PoincareDiskModelPoint() ,PoincareDiskModelPoint() };

	std::array<PoincareDiskModelPoint, 3> incenterWythoffProjections = { PoincareDiskModelPoint() ,PoincareDiskModelPoint(),PoincareDiskModelPoint() };
};

