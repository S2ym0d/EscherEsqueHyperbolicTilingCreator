#pragma once
#include "FundamentalTriangle.h"
#include <Eigen/Core>

struct TilingSettings;

struct GlobalUniforms;

constexpr double PI = 3.1415926535897931;

FundamentalTriangle PVertexCenteredTriangle(double pAngle, double qAngle, double rAngle);

FundamentalTriangle FootOfTheQVertexAltitudeCenteredTriangle(double pAngle, double qAngle, double rAngle);

std::array<float, 3> EigenVectorToArray(const Eigen::Vector3d& v);

std::array<float, 2> EigenVectorToArray(const Eigen::Vector2d& v);

std::array<float, 9> EigenMatrixToArray(const Eigen::Matrix3d& m);

Eigen::Vector3d HyperboloidModelLineNormalVector(const PoincareDiskModelPoint& p, const PoincareDiskModelPoint& q);

Eigen::Matrix3d HyperboloidModelLineReflectionMatrix(const PoincareDiskModelPoint& p, const PoincareDiskModelPoint& q);

std::pair<FundamentalTriangle, bool> GetFundamentalTriangle(const TilingSettings& ts);

std::pair<FundamentalTriangle, bool> FillGlobalUniforms(const TilingSettings& ts, GlobalUniforms& gu);