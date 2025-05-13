#pragma once
#include "OPENGLHeaderFiles.h"
#include <array>
#include "TilingSettings.h"
#include "HyperboloidModelPoint.h"
#include <tuple>
#include "Image.h"

constexpr std::array<float, 9> IdentityMatrixArray = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

struct GlobalUniforms
{
	std::array<int, 2> windowSizes = {800, 600};
	std::array<bool, 3> separatingSides = {true, true, true};
	std::tuple<float, HyperboloidModelPoint, float> sourceImageTransformations = {0.0f, HyperboloidModelPoint(), 0.0f};
	HyperbolicModel sourceImageModel = HyperbolicModel::PoincareDisk;
	std::tuple<float, HyperboloidModelPoint, float> tilingImageTransformations = { 0.0f, HyperboloidModelPoint(), 0.0f };
	HyperbolicModel tilingImageModel = HyperbolicModel::PoincareDisk;

	std::array<float, 9> aSideReflectionIsometryMatrix = IdentityMatrixArray;
	std::array<float, 9> bSideReflectionIsometryMatrix = IdentityMatrixArray;
	std::array<float, 9> cSideReflectionIsometryMatrix = IdentityMatrixArray;

	std::array<float, 3> aSideNormalVector = { 0.0f, 1.0f, 0.0 };
	std::array<float, 3> bSideNormalVector = { 0.0f, 1.0f, 0.0 };
	std::array<float, 3> cSideNormalVector = { 0.0f, 1.0f, 0.0 };
};

struct FilePathsPrefixes
{
	static const std::string shadersFolderPath;

	static const std::string cyclesShadersFilesName;
	static const std::string tilingShardesFilesName;
};

std::string LoadFileToString(const std::string& path);
GLuint CompileShader(GLenum type, const std::string& src);
GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

class Renderer
{
private:
	GLfloat quadVertices[8] = {
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f,
	};

	GLuint quadVAO = 0; 
	GLuint quadVBO = 0;

	GLuint cyclesShaderProgram = 0;
	GLuint tilingShaderProgram = 0;

	GLuint texture0 = 0;
	GLuint texture1 = 0;

	GLuint u_CyclesShader_WindowSizes = 0;
	GLuint u_CyclesShader_A = 0;
	GLuint u_CyclesShader_B = 0;
	GLuint u_CyclesShader_C = 0;
	GLuint u_CyclesShader_Color = 0;
	GLuint u_CyclesShader_Thickness = 0;
	GLuint u_CyclesShader_Model = 0;
						 
	GLuint u_TilingShader_WindowSizes = 0;
	GLuint u_TilingShader_SeparatingSides = 0;
	GLuint u_TilingShader_SeparatingSidesNum = 0;
	GLuint u_TilingShader_SourceImageTransformations = 0;
	GLuint u_TilingShader_TilingImageTransformations = 0;
	GLuint u_TilingShader_SourceImageModel = 0;
	GLuint u_TilingShader_TilingImageModel = 0;
	GLuint u_TilingShader_aSideReflectionIsometryMatrix = 0;
	GLuint u_TilingShader_bSideReflectionIsometryMatrix = 0;
	GLuint u_TilingShader_cSideReflectionIsometryMatrix = 0;
	GLuint u_TilingShader_aSideNormalVector = 0;
	GLuint u_TilingShader_bSideNormalVector = 0;
	GLuint u_TilingShader_cSideNormalVector = 0;
	GLuint u_TilingShader_Texture0 = 0;
	GLuint u_TilingShader_Texture1 = 0;
	

	void InitializeQuadBuffers();

	void InitializeCyclesShaderProgram(const GlobalUniforms& globaluniform);
	void InitializeTilingProgram(const GlobalUniforms& globaluniforms);

public:
	void InitializeShaders(const GlobalUniforms& globaluniforms);
	void ReloadShader(const GlobalUniforms& globaluniforms);

	void RenderCycle(const GlobalUniforms& globaluniforms, const float A, const std::array<float, 2>& B, const float C, const std::array<float, 4>& color, float thickness = 1.0) const;

	void RenderTiling(const GlobalUniforms& globaluniforms, const std::array<Image, 2>& images) const;

	~Renderer();
};

