#version 330 core

layout(location = 0) in vec2 position;

const int PoincareDisk = 0;
const int BeltramiKleinDisk = 1;
const int PoincareHalfPlane = 2;
const int Gans = 3;
const int Band = 4;
const int AzimuthalEquidistant = 5;

const float GansScale = 3.0;

const mat3 LorentzianMetricTensor = mat3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
const vec3 LorentzianOriginPoint = vec3(1.0, 0.0, 0.0);

uniform vec2 u_wSize;
uniform int u_tilingImageModel;
uniform vec4 u_sourceImageTransformations;
uniform vec4 u_tilingImageTransformations;

out vec2  coord;

flat out mat3 sourceImageTransformations;
flat out mat3 tilingImageTransformations;

void LorentzianRotationMatrix(in float rotationAngle, out mat3 lorentzianRotationMatrix)
{
	float s = sin(rotationAngle);
	float c = cos(rotationAngle);

	lorentzianRotationMatrix[0] = vec3(1.0, 0.0, 0.0);
	lorentzianRotationMatrix[1] = vec3(0.0, c, s);
	lorentzianRotationMatrix[2] = vec3(0.0, -s, c);
}

void LorentzianTranslationMatrix(in vec3 translationVector, out mat3 lorentzianTranslationMatrix)
{
	float w = translationVector.x;

	lorentzianTranslationMatrix = outerProduct(translationVector + LorentzianOriginPoint, translationVector + LorentzianOriginPoint) / (w + 1.0) + LorentzianMetricTensor;
}

void main()
{

	mat3 sourceImageLocalRotation;
	mat3 sourceImageTranslation;
	mat3 sourceImageGlobalRotation;

	mat3 tilingImageLocalRotation;
	mat3 tilingImageTranslation;
	mat3 tilingImageGlobalRotation;

	vec3 sourceImageTranslationVector = vec3(sqrt(1.0 + dot(u_sourceImageTransformations.yz, u_sourceImageTransformations.yz)), u_sourceImageTransformations.yz);
	vec3 tilingImageTranslationVector = vec3(sqrt(1.0 + dot(u_tilingImageTransformations.yz, u_tilingImageTransformations.yz)), u_tilingImageTransformations.yz);

	LorentzianRotationMatrix(u_sourceImageTransformations.x, sourceImageLocalRotation);
	LorentzianTranslationMatrix(sourceImageTranslationVector, sourceImageTranslation);
	LorentzianRotationMatrix(u_sourceImageTransformations.w, sourceImageGlobalRotation);

	sourceImageTransformations = sourceImageGlobalRotation * sourceImageTranslation * sourceImageLocalRotation;

	LorentzianRotationMatrix(-u_tilingImageTransformations.x, tilingImageLocalRotation);
	LorentzianTranslationMatrix(-LorentzianMetricTensor * tilingImageTranslationVector, tilingImageTranslation);
	LorentzianRotationMatrix(-u_tilingImageTransformations.w, tilingImageGlobalRotation);

	tilingImageTransformations = tilingImageLocalRotation * tilingImageTranslation * tilingImageGlobalRotation;

	if(u_tilingImageModel == PoincareDisk || u_tilingImageModel == BeltramiKleinDisk)
	{
		if(u_wSize.x >= u_wSize.y)
		{
			coord = vec2(position.x * u_wSize.x/u_wSize.y, position.y);
		}
		else
		{
			coord = vec2(position.x, position.y * u_wSize.y/u_wSize.x);
		}
	}
	else if(u_tilingImageModel == PoincareHalfPlane)
	{
		coord = vec2(position.x * u_wSize.x/u_wSize.y, position.y + 1);
	}
	else if(u_tilingImageModel == Band)
	{
		coord = vec2(position.x * u_wSize.x/u_wSize.y, position.y);
	}
	else
	{
		coord = GansScale * vec2(position.x * u_wSize.x/u_wSize.y, position.y);
	}

	gl_Position = vec4(position.xy, 0.0, 1.0);
}