#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string FilePathsPrefixes::shadersFolderPath("res/shaders/");

const std::string FilePathsPrefixes::cyclesShadersFilesName("cyclesShader");
const std::string FilePathsPrefixes::tilingShardesFilesName("tilingShader");

const std::string vertexShaderExt = ".vert";
const std::string fragmentShaderExt = ".frag";

std::string LoadFileToString(const std::string& path)
{
    std::ifstream file;

    file.open(path.c_str());

    std::stringstream fileCotent;

    if (file.is_open())
    {
        fileCotent << file.rdbuf();

        file.close();
    }
    else
    {
        std::cout << "Error opening file: " << path << std::endl;
    }

    return fileCotent.str();
}

GLuint CompileShader(GLenum type, const std::string& src)
{
    GLuint shaderID = glCreateShader(type);

    const GLchar* csrc = src.c_str();

    glShaderSource(shaderID, 1, &(csrc), nullptr);
    glCompileShader(shaderID);

    GLint result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        GLint lengthOfLog = 0;

        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &lengthOfLog);

        std::vector<GLchar> message(lengthOfLog);

        glGetShaderInfoLog(shaderID, static_cast<GLsizei>(message.size()), &lengthOfLog, message.data());

        std::cout << "Filed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
        std::cout << "ERROR: " << message.data() << std::endl;

        glDeleteShader(shaderID);

        return 0;
    }

    return shaderID;
}

GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, fs);
    glDetachShader(program, vs);

    glDeleteShader(fs);
    glDeleteShader(vs);

    return program;
}

void Renderer::InitializeQuadBuffers()
{
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
}

void Renderer::InitializeCyclesShaderProgram(const GlobalUniforms& globaluniforms)
{
    std::string vsf, fsf;

    vsf = LoadFileToString(FilePathsPrefixes::shadersFolderPath + FilePathsPrefixes::cyclesShadersFilesName + vertexShaderExt);
    fsf = LoadFileToString(FilePathsPrefixes::shadersFolderPath + FilePathsPrefixes::cyclesShadersFilesName + fragmentShaderExt);

    cyclesShaderProgram = CreateShaderProgram(vsf, fsf);

    GLuint shaderProgram = cyclesShaderProgram;

    glUseProgram(shaderProgram);

    u_CyclesShader_WindowSizes = glGetUniformLocation(shaderProgram, "u_wSize");
    u_CyclesShader_A = glGetUniformLocation(shaderProgram, "u_A");
    u_CyclesShader_B = glGetUniformLocation(shaderProgram, "u_B");
    u_CyclesShader_C = glGetUniformLocation(shaderProgram, "u_C");
    u_CyclesShader_Color = glGetUniformLocation(shaderProgram, "u_color");
    u_CyclesShader_Thickness = glGetUniformLocation(shaderProgram, "u_thickness");
    u_CyclesShader_Model = glGetUniformLocation(shaderProgram, "u_model");

    glUniform2f(u_CyclesShader_WindowSizes, static_cast<GLfloat>(globaluniforms.windowSizes.at(0)), static_cast<GLfloat>(globaluniforms.windowSizes.at(1)));
}

void Renderer::InitializeTilingProgram(const GlobalUniforms& globaluniforms)
{
    std::string vsf, fsf;

    vsf = LoadFileToString(FilePathsPrefixes::shadersFolderPath + FilePathsPrefixes::tilingShardesFilesName + vertexShaderExt);
    fsf = LoadFileToString(FilePathsPrefixes::shadersFolderPath + FilePathsPrefixes::tilingShardesFilesName + fragmentShaderExt);

    tilingShaderProgram = CreateShaderProgram(vsf, fsf);

    GLuint shaderProgram = tilingShaderProgram;

    glUseProgram(shaderProgram);

    u_TilingShader_WindowSizes = glGetUniformLocation(shaderProgram, "u_wSize");
    u_TilingShader_SeparatingSides = glGetUniformLocation(shaderProgram, "u_separatingSides");
    u_TilingShader_SeparatingSidesNum = glGetUniformLocation(shaderProgram, "u_separatingSidesNum");
    u_TilingShader_SourceImageTransformations = glGetUniformLocation(shaderProgram, "u_sourceImageTransformations");
    u_TilingShader_TilingImageTransformations = glGetUniformLocation(shaderProgram, "u_tilingImageTransformations");
    u_TilingShader_SourceImageModel = glGetUniformLocation(shaderProgram, "u_sourceImageModel");
    u_TilingShader_TilingImageModel = glGetUniformLocation(shaderProgram, "u_tilingImageModel");
    u_TilingShader_aSideReflectionIsometryMatrix = glGetUniformLocation(shaderProgram, "u_aSideReflection");
    u_TilingShader_bSideReflectionIsometryMatrix = glGetUniformLocation(shaderProgram, "u_bSideReflection");
    u_TilingShader_cSideReflectionIsometryMatrix = glGetUniformLocation(shaderProgram, "u_cSideReflection");
    u_TilingShader_aSideNormalVector = glGetUniformLocation(shaderProgram, "u_aSideNormal");
    u_TilingShader_bSideNormalVector = glGetUniformLocation(shaderProgram, "u_bSideNormal");
    u_TilingShader_cSideNormalVector = glGetUniformLocation(shaderProgram, "u_cSideNormal");
    u_TilingShader_Texture0 = glGetUniformLocation(shaderProgram, "u_texture0");
    u_TilingShader_Texture1 = glGetUniformLocation(shaderProgram, "u_texture1");

    glUniform2f(u_TilingShader_WindowSizes, static_cast<GLfloat>(globaluniforms.windowSizes.at(0)), static_cast<GLfloat>(globaluniforms.windowSizes.at(1)));
}

void Renderer::InitializeShaders(const GlobalUniforms& globaluniforms)
{
    InitializeQuadBuffers();

    //InitializeCyclesShaderProgram(globaluniforms);
    InitializeTilingProgram(globaluniforms);

    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);
}

void Renderer::ReloadShader(const GlobalUniforms& globaluniforms)
{
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture0);
    glDeleteProgram(tilingShaderProgram);
    glDeleteProgram(cyclesShaderProgram);

    InitializeCyclesShaderProgram(globaluniforms);
    InitializeTilingProgram(globaluniforms);

    glGenTextures(1, &texture0);
    glGenTextures(1, &texture1);
}

void Renderer::RenderCycle(const GlobalUniforms& globaluniforms, const float A, const std::array<float, 2>& B, const float C, const std::array<float, 4>& color, float thickness) const
{
    glBindVertexArray(quadVAO);
    glUseProgram(cyclesShaderProgram);

    glUniform1f(u_CyclesShader_A, A);
    glUniform2fv(u_CyclesShader_B, 1, B.data());
    glUniform1f(u_CyclesShader_C, A);
    glUniform4fv(u_CyclesShader_Color, 1, color.data());
    glUniform1f(u_CyclesShader_Thickness, thickness);
    glUniform1i(u_CyclesShader_Model, static_cast<GLint>(globaluniforms.tilingImageModel));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

Renderer::~Renderer()
{
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture0);

    glDeleteProgram(tilingShaderProgram);
    glDeleteProgram(cyclesShaderProgram);

    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::RenderTiling(const GlobalUniforms& globaluniforms, const std::array<Image, 2>& images) const
{
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images.at(0).Width(), images.at(0).Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, images.at(0).Data().data());

    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images.at(1).Width(), images.at(1).Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, images.at(1).Data().data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBindVertexArray(quadVAO);
    glUseProgram(tilingShaderProgram);

    glUniform1i(u_TilingShader_Texture0, 0);
    glUniform1i(u_TilingShader_Texture1, 1);

    glUniform3i(u_TilingShader_SeparatingSides, globaluniforms.separatingSides.at(0) ? 1 : 0, globaluniforms.separatingSides.at(1) ? 1 : 0, globaluniforms.separatingSides.at(2) ? 1 : 0);
    
    GLint separatingSidesNum = (globaluniforms.separatingSides.at(0) ? 1 : 0) + (globaluniforms.separatingSides.at(1) ? 1 : 0) + (globaluniforms.separatingSides.at(2) ? 1 : 0);
    
    glUniform1i(u_TilingShader_SeparatingSidesNum, separatingSidesNum);
    glUniform4f(u_TilingShader_SourceImageTransformations, std::get<0>(globaluniforms.sourceImageTransformations), static_cast<GLfloat>(std::get<1>(globaluniforms.sourceImageTransformations).X()), static_cast<GLfloat>(std::get<1>(globaluniforms.sourceImageTransformations).Y()), std::get<2>(globaluniforms.sourceImageTransformations));
    glUniform4f(u_TilingShader_TilingImageTransformations, std::get<0>(globaluniforms.tilingImageTransformations), static_cast<GLfloat>(std::get<1>(globaluniforms.tilingImageTransformations).X()), static_cast<GLfloat>(std::get<1>(globaluniforms.tilingImageTransformations).Y()), std::get<2>(globaluniforms.tilingImageTransformations));
    glUniform1i(u_TilingShader_SourceImageModel, static_cast<GLint>(globaluniforms.sourceImageModel));
    glUniform1i(u_TilingShader_TilingImageModel, static_cast<GLint>(globaluniforms.tilingImageModel));
    glUniformMatrix3fv(u_TilingShader_aSideReflectionIsometryMatrix, 1, GL_FALSE, globaluniforms.aSideReflectionIsometryMatrix.data());
    glUniformMatrix3fv(u_TilingShader_bSideReflectionIsometryMatrix, 1, GL_FALSE, globaluniforms.bSideReflectionIsometryMatrix.data());
    glUniformMatrix3fv(u_TilingShader_cSideReflectionIsometryMatrix, 1, GL_FALSE, globaluniforms.cSideReflectionIsometryMatrix.data());
    glUniform3fv(u_TilingShader_aSideNormalVector, 1, globaluniforms.aSideNormalVector.data());
    glUniform3fv(u_TilingShader_bSideNormalVector, 1, globaluniforms.bSideNormalVector.data());
    glUniform3fv(u_TilingShader_cSideNormalVector, 1, globaluniforms.cSideNormalVector.data());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
