#pragma once
#include <array>
#include <string>
#include "HyperboloidModelPoint.h"

enum WythhoffSymbol
{
	Ipqr,
	pIqr,
	qIpr,
	rIpq,
	pqIr,
	prIq,
	qrIp,
	pqrI
};

enum TilingAlgorithm
{
	fast,
	tileInternallyConsistent
};

enum HyperbolicModel
{
	PoincareDisk = 0,
	BeltramiKleinDisk = 1,
	PoincareHalfPlane = 2,
	Gans = 3,
	Band = 4,
	AzimuthalEquidistant = 5
};

enum CenteringType
{
	vertexCentered,
	footOfAltitudeCentered
};

struct TilingSettings
{
	std::array<long long, 3> pqrValues = {7, 3, 2};
	std::array<bool, 3> separatingSide = { true, true ,true };

	bool oddEvenImageTiling = false;
	std::string evenImageFilePath = "";
	std::string oddImageFilePath = "";

	double sourceImageLocalRotation = 0.0;
	HyperboloidModelPoint sourceImageTranslation = HyperboloidModelPoint(0.0, 0.0);
	double sourceImageGlobalRotation = 0.0;
	HyperbolicModel sourceImageModel = HyperbolicModel::PoincareDisk;

	double tilingImageLocalRotation = 0.0;
	HyperboloidModelPoint tilingImageTranslation = HyperboloidModelPoint(0.0, 0.0);
	double tilingImageGlobalRotation = 0.0;
	HyperbolicModel tilingImageModel = HyperbolicModel::PoincareDisk;

	TilingAlgorithm tilingAlgorithm = TilingAlgorithm::fast;
	CenteringType centeringType = CenteringType::vertexCentered;
};

std::pair<TilingSettings, bool> LoadTilingSettings(const std::string& settingsFilePath);

void MakeTilingSettingsFile();
