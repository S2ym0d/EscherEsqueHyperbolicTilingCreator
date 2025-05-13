#pragma once
#include "FundamentalTriangle.h"
#include <utility>
#include <vector>

struct FundamentalTile
{
	FundamentalTriangle fundamentalTriangle;

	std::vector<std::pair<PoincareDiskModelPoint, PoincareDiskModelPoint>> polygon;
};
