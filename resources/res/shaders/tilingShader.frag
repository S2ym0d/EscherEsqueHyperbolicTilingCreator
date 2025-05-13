#version 330 core

#define EPS 0.0001

#define M_PI 3.1415926535897932384626433832795

#define MAXITER 200

in vec2 coord;

flat in mat3 sourceImageTransformations;
flat in mat3 tilingImageTransformations;

out vec4 color;

uniform vec2 u_wSize;
uniform bvec3 u_separatingSides;
uniform int u_separatingSidesNum;
uniform int u_sourceImageModel;
uniform int u_tilingImageModel;
uniform mat3 u_aSideReflection;
uniform mat3 u_bSideReflection;
uniform mat3 u_cSideReflection;
uniform vec3 u_aSideNormal;
uniform vec3 u_bSideNormal;
uniform vec3 u_cSideNormal;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

const int PoincareDisk = 0;
const int BeltramiKleinDisk = 1;
const int PoincareHalfPlane = 2;
const int Gans = 3;
const int Band = 4;
const int AzimuthalEquidistant = 5;

const int aIsometry = 0;
const int bIsometry = 1;
const int cIsometry = 2;

const float GansScale = 3.0;

void IsInFunadmentalTriangle(in vec3 hyperboloidPoint, out bool isInFunTriangle)
{
	isInFunTriangle = (dot(hyperboloidPoint, u_aSideNormal) >= -EPS) && (dot(hyperboloidPoint, u_bSideNormal) >= -EPS) && (dot(hyperboloidPoint, u_cSideNormal) >= -EPS);
}

void DoIsometry(in int isometryCode, inout vec3 vector)
{
	if(isometryCode == aIsometry)
	{
		vector = u_aSideReflection * vector;
	}
	else if(isometryCode == bIsometry)
	{
		vector = u_bSideReflection * vector;
	}
	else
	{
		vector = u_cSideReflection * vector;
	}
}

void AddIsometry(in int isometryCode, inout mat3 isometrySequence)
{
	if(isometryCode == aIsometry)
	{
		isometrySequence = isometrySequence * u_aSideReflection;
	}
	else if(isometryCode == bIsometry)
	{
		isometrySequence = isometrySequence * u_bSideReflection;
	}
	else
	{
		isometrySequence = isometrySequence * u_cSideReflection;
	}
}

void TilingModelPointToHemisphereModel(in int tilingModel, in vec2 tilingPoint, out vec3 hemispherePoint, out bool pointInDomain)
{
	pointInDomain = false;

	if(tilingModel == PoincareDisk)
	{
		if(length(tilingPoint) < 1.0)
		{
			pointInDomain = true;
		}

		float length2 = dot(tilingPoint, tilingPoint);

		hemispherePoint.yz = 2.0 * tilingPoint / (1.0 + length2);
		hemispherePoint.x = (1.0 - length2) / (1.0 + length2);
	}
	else if(tilingModel == BeltramiKleinDisk)
	{
		if(length(tilingPoint) < 1.0)
		{
			pointInDomain = true;
		}

		hemispherePoint.yz = tilingPoint;
		hemispherePoint.x = sqrt(1.0 - dot(tilingPoint, tilingPoint));
	}
	else if(tilingModel == PoincareHalfPlane)
	{
		if(tilingPoint.y > 0.0)
		{
			pointInDomain = true;
		}

		vec2 rescaledTilingPoint = 2.0 * tilingPoint;

		float hemisphereProjectionValue = 4.0 / (dot(rescaledTilingPoint, rescaledTilingPoint) + 4.0);

		hemispherePoint = vec3(rescaledTilingPoint.y * hemisphereProjectionValue, rescaledTilingPoint.x * hemisphereProjectionValue, 1.0 - 2.0 * hemisphereProjectionValue);
	}
	else if(tilingModel == Band)
	{
		pointInDomain = true;

		vec2 rescaledTilingPoint = M_PI * tilingPoint / 2.0;

		vec3 hemiCylinderModelPoint = vec3(cos(rescaledTilingPoint.y), sinh(rescaledTilingPoint.x), sin(rescaledTilingPoint.y));

		float hemisphereProjectionValue = 1.0 / cosh(rescaledTilingPoint.x);

		hemispherePoint = hemiCylinderModelPoint * hemisphereProjectionValue;
	}
}

void TilingModelPointToHyperboloidModel(in int tilingModel, in vec2 tilingPoint, out vec3 hyperboloidPoint, out bool pointInDomain)
{
	if(tilingModel == PoincareDisk || tilingModel == BeltramiKleinDisk || tilingModel == PoincareHalfPlane || tilingModel == Band)
	{
		vec3 hemispherePoint;

		TilingModelPointToHemisphereModel(tilingModel, tilingPoint, hemispherePoint, pointInDomain);

		if(hemispherePoint.x < EPS)
		{
			pointInDomain = false;
		}

		if(pointInDomain)
		{
			hyperboloidPoint.yz = hemispherePoint.yz / hemispherePoint.x;
			hyperboloidPoint.x = 1.0 / hemispherePoint.x;
		}
	}
	else
	{
		pointInDomain = true;
		vec2 tilingPointCopy = tilingPoint;

		if(tilingModel == AzimuthalEquidistant)
		{
			float pointLength = length(tilingPointCopy);

			if(pointLength > EPS)
			{
				tilingPointCopy = tilingPointCopy * sinh(pointLength) / pointLength;
			}
		}

		hyperboloidPoint.yz = tilingPointCopy;
		hyperboloidPoint.x = sqrt(1.0 + dot(tilingPointCopy, tilingPointCopy));
	}
}

void HemisphereModelPointToImageModel(in int imageModel, in vec3 hemispherePoint, out vec2 imageModelPoint)
{
	if(imageModel == PoincareDisk)
	{
		imageModelPoint = hemispherePoint.yz / (1.0 + hemispherePoint.x);
	}
	else if(imageModel == BeltramiKleinDisk)
	{
		imageModelPoint = hemispherePoint.yz;
	}
	else if(imageModel == PoincareDisk)
	{
		imageModelPoint.x = hemispherePoint.y / (1.0 - hemispherePoint.z);
		imageModelPoint.y = hemispherePoint.x / (1.0 - hemispherePoint.z);
	}
	else if(imageModel == Band)
	{
		float rescalingValue = 2.0 / M_PI;

		float hemiCylinderProjectionValue = 1.0 / sqrt(1.0 - hemispherePoint.y * hemispherePoint.y);

		vec3 hemiCylinderModelPoint = hemispherePoint * hemiCylinderProjectionValue;

		imageModelPoint = vec2(asinh(hemiCylinderModelPoint.y), asin(hemiCylinderModelPoint.z)) * rescalingValue;
	}
}

void HyperboloidModelPointToImageModel(in int imageModel, in vec3 hyperboloidPoint, out vec2 imageModelPoint)
{
	if(imageModel == PoincareDisk || imageModel == BeltramiKleinDisk || imageModel == PoincareHalfPlane || imageModel == Band)
	{
		vec3 hemispherePoint = vec3(1.0, hyperboloidPoint.yz) / hyperboloidPoint.x;

		HemisphereModelPointToImageModel(imageModel, hemispherePoint, imageModelPoint);
	}
	else
	{
		imageModelPoint = hyperboloidPoint.yz;

		if(imageModel == AzimuthalEquidistant)
		{
			float pointLength = length(imageModelPoint);

			if(pointLength > EPS)
			{
				imageModelPoint = imageModelPoint * asinh(pointLength) / pointLength;
			}
		}
	}
}

void CalculateFundamentalCoordinates3Separators(in vec3 hyperboloidPoint, out vec3 fundamentalCoordinates, out int oddeven)
{
	fundamentalCoordinates = hyperboloidPoint;
	oddeven = 0;

	for (int i = 0; i < MAXITER; i++)
	{
		bool isInTriangle = true;

		IsInFunadmentalTriangle(fundamentalCoordinates, isInTriangle);

		if (isInTriangle) break;

		if(dot(fundamentalCoordinates, u_aSideNormal) < -EPS)
		{
			DoIsometry(aIsometry, fundamentalCoordinates);
			oddeven = 1 - oddeven;
		}
		else if(dot(fundamentalCoordinates, u_bSideNormal) < -EPS)
		{
			DoIsometry(bIsometry, fundamentalCoordinates);
			oddeven = 1 - oddeven;
		}
		else if(dot(fundamentalCoordinates, u_cSideNormal) < -EPS)
		{
			DoIsometry(cIsometry, fundamentalCoordinates);
			oddeven = 1 - oddeven;
		}
	}
}

void CalculateFundamentalCoordinates2Separators(in vec3 hyperboloidPoint, in vec3 nonSepNormal, in int nonSepCode, in vec3 sep1Normal, in int sep1Code, in vec3 sep2Normal, in int sep2Code, out vec3 fundamentalCoordinates, out int oddeven)
{
	fundamentalCoordinates = hyperboloidPoint;
	vec3 fundamentalTriangleCoordinates = fundamentalCoordinates;

	bool nonSepLast = false;

	oddeven = 0;
	
	for (int i = 0; i < MAXITER; i++)
	{
		bool isInTriangle = true;

		IsInFunadmentalTriangle(fundamentalTriangleCoordinates, isInTriangle);

		if (isInTriangle) break;

		if(dot(fundamentalTriangleCoordinates, nonSepNormal) < -EPS)
		{
			nonSepLast = true;

			DoIsometry(nonSepCode, fundamentalTriangleCoordinates);
		}
		else if(dot(fundamentalTriangleCoordinates, sep1Normal) < -EPS)
		{
			DoIsometry(sep1Code, fundamentalTriangleCoordinates);

			if(nonSepLast)
			{
				DoIsometry(nonSepCode, fundamentalTriangleCoordinates);
			}

			nonSepLast = false;

			fundamentalCoordinates = fundamentalTriangleCoordinates;

			oddeven = 1 - oddeven;
		}
		else if(dot(fundamentalTriangleCoordinates, sep2Normal) < -EPS)
		{
			DoIsometry(sep2Code, fundamentalTriangleCoordinates);

			if(nonSepLast)
			{
				DoIsometry(nonSepCode, fundamentalTriangleCoordinates);
			}

			nonSepLast = false;

			fundamentalCoordinates = fundamentalTriangleCoordinates;

			oddeven = 1 - oddeven;
		}
	}
}

void CalculateFundamentalCoordinates1Separators(in vec3 hyperboloidPoint, in vec3 nonSep1Normal, in int nonSep1Code, in vec3 nonSep2Normal, in int nonSep2Code, in vec3 sepNormal, in int sepCode, out vec3 fundamentalCoordinates, out int oddeven)
{
	fundamentalCoordinates = hyperboloidPoint;
	vec3 fundamentalTriangleCoordinates = fundamentalCoordinates;

	mat3 invertedNonSepSequence = mat3(1.0);

	oddeven = 0;

	for (int i = 0; i < MAXITER; i++)
	{
		bool isInTriangle = true;

		IsInFunadmentalTriangle(fundamentalTriangleCoordinates, isInTriangle);

		if (isInTriangle) break;

		if(dot(fundamentalTriangleCoordinates, nonSep1Normal) < -EPS)
		{
			AddIsometry(nonSep1Code, invertedNonSepSequence);

			DoIsometry(nonSep1Code, fundamentalTriangleCoordinates);
		}
		else if(dot(fundamentalTriangleCoordinates, nonSep2Normal) < -EPS)
		{
			AddIsometry(nonSep2Code, invertedNonSepSequence);

			DoIsometry(nonSep2Code, fundamentalTriangleCoordinates);
		}
		else if(dot(fundamentalTriangleCoordinates, sepNormal) < -EPS)
		{
			DoIsometry(sepCode, fundamentalTriangleCoordinates);

			fundamentalTriangleCoordinates = invertedNonSepSequence * fundamentalTriangleCoordinates;

			invertedNonSepSequence = mat3(1.0);

			fundamentalCoordinates = fundamentalTriangleCoordinates;

			oddeven = 1 - oddeven;
		}
	}
}

void CalculateFundamentalCoordinates(in vec3 hyperboloidPoint, out vec3 fundamentalCoordinates, out int oddeven)
{
	if(u_separatingSidesNum == 3)
	{
		CalculateFundamentalCoordinates3Separators(hyperboloidPoint, fundamentalCoordinates, oddeven);
	}
	else if(u_separatingSidesNum == 2)
	{
		vec3 nonSepNormal = vec3(0.0);
		int nonSepCode = 0;

		vec3 sep1Normal = vec3(0.0);
		int sep1Code = 0;

		vec3 sep2Normal = vec3(0.0);
		int sep2Code = 0;

		if(u_separatingSides.x == false)
		{
			nonSepNormal = u_aSideNormal;
			nonSepCode = aIsometry;

			sep1Normal = u_bSideNormal;
			sep1Code = bIsometry;

			sep2Normal = u_cSideNormal;
			sep2Code = cIsometry;
		}
		else if(u_separatingSides.y == false)
		{
			nonSepNormal = u_bSideNormal;
			nonSepCode = bIsometry;

			sep1Normal = u_aSideNormal;
			sep1Code = aIsometry;

			sep2Normal = u_cSideNormal;
			sep2Code = cIsometry;
		}
		else
		{
			nonSepNormal = u_cSideNormal;
			nonSepCode = cIsometry;

			sep1Normal = u_aSideNormal;
			sep1Code = aIsometry;

			sep2Normal = u_bSideNormal;
			sep2Code = bIsometry;
		}

		CalculateFundamentalCoordinates2Separators(hyperboloidPoint, nonSepNormal, nonSepCode, sep1Normal, sep1Code, sep2Normal, sep2Code, fundamentalCoordinates, oddeven);
	}
	else
	{
		vec3 nonSep1Normal = vec3(0.0);
		int nonSep1Code = 0;

		vec3 nonSep2Normal = vec3(0.0);
		int nonSep2Code = 0;

		vec3 sepNormal = vec3(0.0);
		int sepCode = 0;

		if(u_separatingSides.x)
		{
			sepNormal = u_aSideNormal;
			sepCode = aIsometry;

			nonSep1Normal = u_bSideNormal;
			nonSep1Code = bIsometry;

			nonSep2Normal = u_cSideNormal;
			nonSep2Code = cIsometry;
		}
		else if(u_separatingSides.y)
		{
			sepNormal = u_bSideNormal;
			sepCode = bIsometry;

			nonSep1Normal = u_aSideNormal;
			nonSep1Code = aIsometry;

			nonSep2Normal = u_cSideNormal;
			nonSep2Code = cIsometry;
		}
		else
		{
			sepNormal = u_cSideNormal;
			sepCode = cIsometry;

			nonSep1Normal = u_aSideNormal;
			nonSep1Code = aIsometry;

			nonSep2Normal = u_bSideNormal;
			nonSep2Code = bIsometry;
		}

		CalculateFundamentalCoordinates1Separators(hyperboloidPoint, nonSep1Normal, nonSep1Code, nonSep2Normal, nonSep2Code, sepNormal, sepCode, fundamentalCoordinates, oddeven);
	}
}

void SourceImageModelPointToTextureCoord(in vec2 sourceImageModelPoint, out vec2 textureCoord)
{
	if(u_sourceImageModel == PoincareDisk || u_sourceImageModel == BeltramiKleinDisk)
	{
		if(u_wSize.x >= u_wSize.y)
		{
			textureCoord = vec2(sourceImageModelPoint.x * u_wSize.y/u_wSize.x, sourceImageModelPoint.y);
		}
		else
		{
			textureCoord = vec2(sourceImageModelPoint.x, sourceImageModelPoint.y * u_wSize.x/u_wSize.y);
		}
	}
	else if(u_sourceImageModel == PoincareHalfPlane)
	{
		textureCoord = vec2(sourceImageModelPoint.x * u_wSize.y/u_wSize.x, sourceImageModelPoint.y / 2.0);
	}
	else if(u_sourceImageModel == Band)
	{
		textureCoord = vec2(sourceImageModelPoint.x * u_wSize.y/u_wSize.x, sourceImageModelPoint.y);
	}
	else
	{
		textureCoord = vec2(sourceImageModelPoint.x * u_wSize.y/u_wSize.x, sourceImageModelPoint.y) / GansScale;
	}

	textureCoord = (textureCoord + vec2(1.0, 1.0)) / 2.0;
}

void main()
{
	vec3 hyperboloidPoint;
	bool pointInDomain = false;

	TilingModelPointToHyperboloidModel(u_tilingImageModel, coord, hyperboloidPoint, pointInDomain);

	if(pointInDomain == false)
	{
		discard;
	}

	hyperboloidPoint = tilingImageTransformations * hyperboloidPoint;

	int oddeven;
	vec3 fundamentalCoordinates; 

	CalculateFundamentalCoordinates(hyperboloidPoint, fundamentalCoordinates, oddeven);

	fundamentalCoordinates = sourceImageTransformations * fundamentalCoordinates;

	vec2 sourceImageModelPoint;

	HyperboloidModelPointToImageModel(u_sourceImageModel, fundamentalCoordinates, sourceImageModelPoint);

	vec2 textureCoord;

	SourceImageModelPointToTextureCoord(sourceImageModelPoint, textureCoord);

	if(oddeven == 0)
	{
		color = texture(u_texture0, textureCoord);
	}
	else
	{
		color = texture(u_texture1, textureCoord);
	}
}