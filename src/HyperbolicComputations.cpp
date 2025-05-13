#include "HyperbolicComputations.h"
#include "TilingSettings.h"
#include <iostream>
#include "Renderer.h"
#include <utility>

const Eigen::Matrix3d IdentityMatrix = Eigen::Matrix3d::Identity(3, 3);
const Eigen::Matrix3d LorentzianMetricTensor = Eigen::Matrix< double, 3, 1>(-1.0, 1.0, 1.0).asDiagonal();

double Cross2D(const Eigen::Vector2d& u, const Eigen::Vector2d& v)
{
	return u.x() * v.y() - u.y() * v.x();
}

std::array<float, 3> EigenVectorToArray(const Eigen::Vector3d& v)
{
	Eigen::Vector3f u = v.cast<float>();
	return { u.x(), u.y(), u.z() };
}

std::array<float, 2> EigenVectorToArray(const Eigen::Vector2d& v)
{
	Eigen::Vector2f u = v.cast<float>();
	return { u.x(), u.y()};
}

std::array<float, 9> EigenMatrixToArray(const Eigen::Matrix3d& m)
{
	Eigen::Matrix3f k = m.cast<float>();
	return { k(0,0),k(1,0),k(2,0),k(0,1),k(1,1),k(2,1),k(0,2),k(1,2),k(2,2) }; 
}

Eigen::Vector3d HyperboloidModelLineNormalVector(const PoincareDiskModelPoint& p, const PoincareDiskModelPoint& q)
{
	Eigen::Vector2d pPoint(p.X(), p.Y());
	Eigen::Vector2d qPoint(q.X(), q.Y());

	double lineConstantCoefficient = Cross2D(pPoint, qPoint);

	Eigen::Vector2d lineCrossLinearCoefficient = pPoint * (qPoint.dot(qPoint) + 1.0) - qPoint * (pPoint.dot(pPoint) + 1.0);
	//Eigen::Vector2d lineDotLinearCoefficient(lineCrossLinearCoefficient.y(), -lineCrossLinearCoefficient.x());

	Eigen::Vector3d normalVector(2.0 * lineConstantCoefficient, lineCrossLinearCoefficient.y(), -lineCrossLinearCoefficient.x());

	return normalVector;
}

Eigen::Matrix3d HyperboloidModelLineReflectionMatrix(const PoincareDiskModelPoint& p, const PoincareDiskModelPoint& q)
{
	auto normalVector = HyperboloidModelLineNormalVector(p, q);

	Eigen::Vector3d lorentzianCrossProduct(-normalVector.x(), normalVector.y(), normalVector.z());

	double lorentzianNormSquared = lorentzianCrossProduct.transpose() * LorentzianMetricTensor * lorentzianCrossProduct;

	Eigen::Vector3d lorentzianCrossProductNormalized = lorentzianCrossProduct / sqrt(lorentzianNormSquared);

	Eigen::Matrix3d reflectionMatrix = IdentityMatrix - 2.0 * lorentzianCrossProductNormalized * lorentzianCrossProductNormalized.transpose() * LorentzianMetricTensor;

	return reflectionMatrix;
}

double sgn(double x)
{
	if (x >= 0.0) return 1.0;

	return -1.0;
}

PoincareDiskModelPoint PoincareDiskLineReflectionOfPoint(const std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&>& line, const PoincareDiskModelPoint& point)
{
	std::pair<Eigen::Vector2d, Eigen::Vector2d> linePoints = { Eigen::Vector2d(line.first.X(), line.first.Y()), Eigen::Vector2d(line.second.X(), line.second.Y()) };

	double lineConstantCoefficient = Cross2D(linePoints.first, linePoints.second);

	Eigen::Vector2d lineCrossLinearCoefficient = linePoints.first * (linePoints.second.dot(linePoints.second) + 1.0) - linePoints.second * (linePoints.first.dot(linePoints.first) + 1.0);
	Eigen::Vector2d lineDotLinearCoefficient(lineCrossLinearCoefficient.y(), -lineCrossLinearCoefficient.x());

	Eigen::Vector2d vectorPoint(point.X(), point.Y());

	Eigen::Vector2d gradientAtPoint = 2.0 * vectorPoint * lineConstantCoefficient + lineDotLinearCoefficient;

	Eigen::Vector2d reflectionDirection = vectorPoint * (lineDotLinearCoefficient.dot(lineDotLinearCoefficient) - pow(2.0 * lineConstantCoefficient, 2.0)) - 2.0 * lineDotLinearCoefficient * (lineConstantCoefficient * (1.0 + vectorPoint.dot(vectorPoint)) + vectorPoint.dot(lineDotLinearCoefficient));

	Eigen::Vector2d reflectionPoint = reflectionDirection / gradientAtPoint.dot(gradientAtPoint);

	return PoincareDiskModelPoint(reflectionPoint.x(), reflectionPoint.y());
}

double PoincareDiskLengthOfSideFromCenter(double oppositeAngle, double adjacentAngle1, double adjacentAngle2)
{
	double rawValueTop = cos(oppositeAngle) + cos(adjacentAngle1 + adjacentAngle2);
	double rawValueBottom = cos(oppositeAngle) + cos(adjacentAngle1 - adjacentAngle2);

	double sideLength = sqrt(rawValueTop / rawValueBottom);

	return sideLength;
}

PoincareDiskModelPoint RVertexAngleBisectorIdealPoint(const PoincareDiskModelPoint& rVertex, const PoincareDiskModelPoint& qVertex)
{
	Eigen::Vector2d rPoint(rVertex.X(), rVertex.Y());
	Eigen::Vector2d qPoint(qVertex.X(), qVertex.Y());

	Eigen::Vector2d rqLineCrossLinearCoefficient = rPoint * (qPoint.dot(qPoint) + 1.0) - qPoint * (rPoint.dot(rPoint) + 1.0);
	Eigen::Vector2d rqLineDotLinearCoefficient(rqLineCrossLinearCoefficient.y(), -rqLineCrossLinearCoefficient.x());

	double rqLineConstantCoefficient = Cross2D(rPoint, qPoint);

	double rqLineDotLinearCoefficientNormSquared = rqLineDotLinearCoefficient.dot(rqLineDotLinearCoefficient);

	Eigen::Vector2d rqLineForwardIdealPoint = -2.0 * rqLineDotLinearCoefficient * rqLineConstantCoefficient / rqLineDotLinearCoefficientNormSquared 
		- rqLineCrossLinearCoefficient * sqrt(1.0 / rqLineDotLinearCoefficientNormSquared - 4.0 * pow(rqLineConstantCoefficient / rqLineDotLinearCoefficientNormSquared, 2.0));

	double rqLineForwardHalfPlaneIdealPoint = rqLineForwardIdealPoint.x() / (1.0 - rqLineForwardIdealPoint.y());

	double rPointHalfPlane = (1.0 + rPoint.y()) / (1.0 - rPoint.y());

	double mobiusTransformationCCoefficient = 1.0 / sqrt(pow(rPointHalfPlane, 2.0) + pow(rqLineForwardHalfPlaneIdealPoint, 2.0));

	double mobiusTransformationACoefficient = mobiusTransformationCCoefficient * rqLineForwardHalfPlaneIdealPoint;

	double rVertexAngleBisectorHalfPlaneIdealPoint = (mobiusTransformationACoefficient + 1.0) / mobiusTransformationCCoefficient;

	Eigen::Vector2d rVertexAngleBisectorIdealPoint = 
		Eigen::Vector2d(2 * rVertexAngleBisectorHalfPlaneIdealPoint, pow(rVertexAngleBisectorHalfPlaneIdealPoint, 2.0) - 1.0) / (pow(rVertexAngleBisectorHalfPlaneIdealPoint, 2.0) + 1.0);

	return PoincareDiskModelPoint(rVertexAngleBisectorIdealPoint.x(), rVertexAngleBisectorIdealPoint.y());
}

std::pair<PoincareDiskModelPoint, bool> LineLineIntersection(const std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&>& line1, const std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&>& line2)
{
	std::pair<Eigen::Vector2d, Eigen::Vector2d> line1Points = { Eigen::Vector2d(line1.first.X(), line1.first.Y()), Eigen::Vector2d(line1.second.X(), line1.second.Y()) };
	std::pair<Eigen::Vector2d, Eigen::Vector2d> line2Points = { Eigen::Vector2d(line2.first.X(), line2.first.Y()), Eigen::Vector2d(line2.second.X(), line2.second.Y()) };

	double line1ConstantCoefficient = Cross2D(line1Points.first, line1Points.second);
	double line2ConstantCoefficient = Cross2D(line2Points.first, line2Points.second);

	Eigen::Vector2d line1CrossLinearCoefficient = line1Points.first * (line1Points.second.dot(line1Points.second) + 1.0) - line1Points.second * (line1Points.first.dot(line1Points.first) + 1.0);
	Eigen::Vector2d line1DotLinearCoefficient(line1CrossLinearCoefficient.y(), -line1CrossLinearCoefficient.x());


	Eigen::Vector2d line2CrossLinearCoefficient = line2Points.first * (line2Points.second.dot(line2Points.second) + 1.0) - line2Points.second * (line2Points.first.dot(line2Points.first) + 1.0);
	Eigen::Vector2d line2DotLinearCoefficient(line2CrossLinearCoefficient.y(), -line2CrossLinearCoefficient.x());

	double intersectionHomogeneousPart = Cross2D(line1DotLinearCoefficient, line2DotLinearCoefficient);

	Eigen::Vector2d intersectionDirection = 2.0 * (line1ConstantCoefficient * line2CrossLinearCoefficient - line2ConstantCoefficient * line1CrossLinearCoefficient);

	double intersectionValue = pow(intersectionHomogeneousPart, 2.0) - intersectionDirection.dot(intersectionDirection);

	if (intersectionValue < 0.0)
	{
		return { PoincareDiskModelPoint(), false };
	}

	double denominator = intersectionHomogeneousPart + sgn(intersectionHomogeneousPart) * sqrt(intersectionValue);

	if (denominator == 0.0)
	{
		return { PoincareDiskModelPoint(), false };
	}

	Eigen::Vector2d intersectionPoint = intersectionDirection / denominator;

	return { PoincareDiskModelPoint(intersectionPoint.x(), intersectionPoint.y()), true };
}

PoincareDiskModelPoint ProjectionOntoLine(const std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&>& line, const PoincareDiskModelPoint& point)
{
	auto reflectedPoint = PoincareDiskLineReflectionOfPoint(line, point);

	std::pair<Eigen::Vector2d, Eigen::Vector2d> linePoints = { Eigen::Vector2d(line.first.X(), line.first.Y()), Eigen::Vector2d(line.second.X(), line.second.Y()) };

	double lineConstantCoefficient = Cross2D(linePoints.first, linePoints.second);

	Eigen::Vector2d lineCrossLinearCoefficient = linePoints.first * (linePoints.second.dot(linePoints.second) + 1.0) - linePoints.second * (linePoints.first.dot(linePoints.first) + 1.0);
	Eigen::Vector2d lineDotLinearCoefficient(lineCrossLinearCoefficient.y(), -lineCrossLinearCoefficient.x());

	Eigen::Vector2d vectorPoint(point.X(), point.Y());
	Eigen::Vector2d vectorReflectedPoint(reflectedPoint.X(), reflectedPoint.Y());

	double lineFunctionValueAtPoint = lineConstantCoefficient * (vectorPoint.dot(vectorPoint) + 1.0) + vectorPoint.dot(lineDotLinearCoefficient);

	double scaleFactorBCoefficient = 1.0 - 2.0 * vectorPoint.dot(vectorReflectedPoint) + vectorPoint.dot(vectorPoint) * vectorReflectedPoint.dot(vectorReflectedPoint);

	double scaleFactorCCoefficient = lineDotLinearCoefficient.dot(lineDotLinearCoefficient) - pow(2.0 * lineConstantCoefficient, 2.0);

	double scaleFactorTopRawValue = scaleFactorBCoefficient * (pow(scaleFactorCCoefficient, 2.0) + 8.0 * scaleFactorCCoefficient * lineConstantCoefficient * lineFunctionValueAtPoint + pow(4.0 * lineConstantCoefficient * lineFunctionValueAtPoint, 2.0));

	double scaleFactorBottomRawValue = scaleFactorCCoefficient * (scaleFactorCCoefficient + 4.0 * lineConstantCoefficient * lineFunctionValueAtPoint);

	double scaleFactor = (sqrt(scaleFactorTopRawValue / scaleFactorBottomRawValue) - (1.0 - vectorPoint.dot(vectorPoint))) / 2.0;

	double tangentCircleEuclideanRadius = sqrt(scaleFactor * (scaleFactor + 1.0 - vectorPoint.dot(vectorPoint)) / pow(scaleFactor + 1.0, 2.0));

	Eigen::Vector2d tangentCircleEuclideanCenter = vectorPoint / (scaleFactor + 1.0);

	Eigen::Vector2d tangentPointDirection = 2.0 * tangentCircleEuclideanCenter * lineConstantCoefficient + lineDotLinearCoefficient;

	tangentPointDirection.normalize();

	double tangentPointDirectionSign = -sgn(lineConstantCoefficient * (tangentCircleEuclideanCenter.dot(tangentCircleEuclideanCenter) + 1.0) + tangentCircleEuclideanCenter.dot(lineDotLinearCoefficient));

	Eigen::Vector2d tangentPoint = tangentCircleEuclideanCenter + tangentPointDirectionSign * tangentCircleEuclideanRadius * tangentPointDirection;

	return PoincareDiskModelPoint(tangentPoint.x(), tangentPoint.y());
}

FundamentalTriangle PVertexCenteredTriangle(double pAngle, double qAngle, double rAngle)
{
	FundamentalTriangle fundamentalTriangle;

	fundamentalTriangle.pqrValues = { -1, -1, -1 };

	fundamentalTriangle.pqrVertices.at(0) = PoincareDiskModelPoint(0.0, 0.0);

	double aSideLength = PoincareDiskLengthOfSideFromCenter(qAngle, pAngle, rAngle);

	double bSideLength = PoincareDiskLengthOfSideFromCenter(rAngle, pAngle, qAngle);

	fundamentalTriangle.pqrVertices.at(1) = PoincareDiskModelPoint(bSideLength * sin(pAngle), -bSideLength * cos(pAngle));

	fundamentalTriangle.pqrVertices.at(2) = PoincareDiskModelPoint(0.0, -aSideLength);

	auto rVertexBisectorIdealPoint = RVertexAngleBisectorIdealPoint(fundamentalTriangle.pqrVertices.at(2), fundamentalTriangle.pqrVertices.at(1));

	PoincareDiskModelPoint pVertexBisectorIdealPoint(sin(pAngle / 2.0), -cos(pAngle / 2.0));

	fundamentalTriangle.incenter = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(0) , pVertexBisectorIdealPoint }, { fundamentalTriangle.pqrVertices.at(2), rVertexBisectorIdealPoint }).first;

	std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&> aSide = { fundamentalTriangle.pqrVertices.at(0),  fundamentalTriangle.pqrVertices.at(2) };
	std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&> bSide = { fundamentalTriangle.pqrVertices.at(1),  fundamentalTriangle.pqrVertices.at(2) };
	std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&> cSide = { fundamentalTriangle.pqrVertices.at(0),  fundamentalTriangle.pqrVertices.at(1) };

	fundamentalTriangle.aSideIntouchPoint = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(1) , fundamentalTriangle.incenter }, aSide).first;
	fundamentalTriangle.bSideIntouchPoint = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(0) , fundamentalTriangle.incenter }, bSide).first;
	fundamentalTriangle.cSideIntouchPoint = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(2) , fundamentalTriangle.incenter }, cSide).first;

	fundamentalTriangle.pVertexWythoffProjection = ProjectionOntoLine(bSide, fundamentalTriangle.pqrVertices.at(0));
	fundamentalTriangle.qVertexWythoffProjection = ProjectionOntoLine(aSide, fundamentalTriangle.pqrVertices.at(1));
	fundamentalTriangle.rVertexWythoffProjection = ProjectionOntoLine(cSide, fundamentalTriangle.pqrVertices.at(2));

	fundamentalTriangle.aSideIntouchPointWythoffProjections = { ProjectionOntoLine(bSide, fundamentalTriangle.aSideIntouchPoint), ProjectionOntoLine(cSide, fundamentalTriangle.aSideIntouchPoint) };
	fundamentalTriangle.bSideIntouchPointWythoffProjections = { ProjectionOntoLine(aSide, fundamentalTriangle.bSideIntouchPoint), ProjectionOntoLine(cSide, fundamentalTriangle.bSideIntouchPoint) };
	fundamentalTriangle.cSideIntouchPointWythoffProjections = { ProjectionOntoLine(aSide, fundamentalTriangle.cSideIntouchPoint), ProjectionOntoLine(bSide, fundamentalTriangle.cSideIntouchPoint) };

	fundamentalTriangle.incenterWythoffProjections = { ProjectionOntoLine(aSide, fundamentalTriangle.incenter),ProjectionOntoLine(bSide, fundamentalTriangle.incenter),ProjectionOntoLine(cSide, fundamentalTriangle.incenter) };

	return fundamentalTriangle;
}

FundamentalTriangle FootOfTheQVertexAltitudeCenteredTriangle(double pAngle, double qAngle, double rAngle)
{
	double upperQAnglePart = atan(sin(qAngle) * cos(pAngle) / (cos(rAngle) + cos(pAngle) * cos(qAngle)));

	auto upperTriangle = PVertexCenteredTriangle(PI / 2.0, upperQAnglePart, pAngle);
	auto bottomTriangle = PVertexCenteredTriangle(PI / 2.0, qAngle - upperQAnglePart, rAngle);

	FundamentalTriangle fundamentalTriangle;

	fundamentalTriangle.pqrValues = { -1, -1, -1 };

	fundamentalTriangle.pqrVertices = { PoincareDiskModelPoint(0.0, -upperTriangle.pqrVertices.at(2).Y()), upperTriangle.pqrVertices.at(1), bottomTriangle.pqrVertices.at(2) };

	fundamentalTriangle.incenter = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(2), bottomTriangle.incenter }, { fundamentalTriangle.pqrVertices.at(0), PoincareDiskModelPoint(upperTriangle.incenter.X(), -upperTriangle.incenter.Y()) }).first;

	std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&> aSide = { fundamentalTriangle.pqrVertices.at(0),  fundamentalTriangle.pqrVertices.at(2) };
	std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&> bSide = { fundamentalTriangle.pqrVertices.at(1),  fundamentalTriangle.pqrVertices.at(2) };
	std::pair<const PoincareDiskModelPoint&, const PoincareDiskModelPoint&> cSide = { fundamentalTriangle.pqrVertices.at(0),  fundamentalTriangle.pqrVertices.at(1) };

	fundamentalTriangle.aSideIntouchPoint = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(1) , fundamentalTriangle.incenter }, aSide).first;
	fundamentalTriangle.bSideIntouchPoint = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(0) , fundamentalTriangle.incenter }, bSide).first;
	fundamentalTriangle.cSideIntouchPoint = LineLineIntersection({ fundamentalTriangle.pqrVertices.at(2) , fundamentalTriangle.incenter }, cSide).first;

	fundamentalTriangle.pVertexWythoffProjection = ProjectionOntoLine(bSide, fundamentalTriangle.pqrVertices.at(0));
	fundamentalTriangle.qVertexWythoffProjection = ProjectionOntoLine(aSide, fundamentalTriangle.pqrVertices.at(1));
	fundamentalTriangle.rVertexWythoffProjection = ProjectionOntoLine(cSide, fundamentalTriangle.pqrVertices.at(2));

	fundamentalTriangle.aSideIntouchPointWythoffProjections = { ProjectionOntoLine(bSide, fundamentalTriangle.aSideIntouchPoint), ProjectionOntoLine(cSide, fundamentalTriangle.aSideIntouchPoint) };
	fundamentalTriangle.bSideIntouchPointWythoffProjections = { ProjectionOntoLine(aSide, fundamentalTriangle.bSideIntouchPoint), ProjectionOntoLine(cSide, fundamentalTriangle.bSideIntouchPoint) };
	fundamentalTriangle.cSideIntouchPointWythoffProjections = { ProjectionOntoLine(aSide, fundamentalTriangle.cSideIntouchPoint), ProjectionOntoLine(bSide, fundamentalTriangle.cSideIntouchPoint) };

	fundamentalTriangle.incenterWythoffProjections = { ProjectionOntoLine(aSide, fundamentalTriangle.incenter),ProjectionOntoLine(bSide, fundamentalTriangle.incenter),ProjectionOntoLine(cSide, fundamentalTriangle.incenter) };

	return fundamentalTriangle;
}

bool IsSchwarzTriangleHyperbolic(long long p, long long q, long long r)
{
	long long pFinite = p == -1 ? 0 : 1;
	long long qFinite = q == -1 ? 0 : 1;
	long long rFinite = r == -1 ? 0 : 1;

	p = p == -1 ? 1 : p;
	q = q == -1 ? 1 : q;
	r = r == -1 ? 1 : r;

	if (p * r * qFinite + q * r * pFinite + p * q * rFinite < p * q * r)
	{
		return true;
	}

	return false;
}

void ReflectTriangleAboutXAxis(FundamentalTriangle& rft)
{
	ReflectPoincareDiskModelPointAboutXAxis(rft.pqrVertices);

	ReflectPoincareDiskModelPointAboutXAxis(rft.incenter);

	ReflectPoincareDiskModelPointAboutXAxis(rft.aSideIntouchPoint);
	ReflectPoincareDiskModelPointAboutXAxis(rft.bSideIntouchPoint);
	ReflectPoincareDiskModelPointAboutXAxis(rft.cSideIntouchPoint);

	ReflectPoincareDiskModelPointAboutXAxis(rft.pVertexWythoffProjection);
	ReflectPoincareDiskModelPointAboutXAxis(rft.qVertexWythoffProjection);
	ReflectPoincareDiskModelPointAboutXAxis(rft.rVertexWythoffProjection);

	ReflectPoincareDiskModelPointAboutXAxis(rft.aSideIntouchPointWythoffProjections);
	ReflectPoincareDiskModelPointAboutXAxis(rft.bSideIntouchPointWythoffProjections);
	ReflectPoincareDiskModelPointAboutXAxis(rft.cSideIntouchPointWythoffProjections);

	ReflectPoincareDiskModelPointAboutXAxis(rft.incenterWythoffProjections);

	std::swap(rft.pqrVertices.at(0), rft.pqrVertices.at(2));

	std::swap(rft.bSideIntouchPoint, rft.cSideIntouchPoint);

	std::swap(rft.pVertexWythoffProjection, rft.rVertexWythoffProjection);

	std::swap(rft.bSideIntouchPointWythoffProjections, rft.cSideIntouchPointWythoffProjections);

	std::swap(rft.incenterWythoffProjections.at(1), rft.incenterWythoffProjections.at(2));
}

std::pair<FundamentalTriangle, bool> GetFundamentalTriangle(const TilingSettings& ts)
{
	long long p = ts.pqrValues.at(0);
	long long q = ts.pqrValues.at(1);
	long long r = ts.pqrValues.at(2);

	if (!IsSchwarzTriangleHyperbolic(p, q, r))
	{
		std::cout << "Schwarz triangle (" << p << ", " << q << ", " << r << ") is not hyperbolic." << std::endl;
		return { FundamentalTriangle(), false };
	}

	if (ts.centeringType == vertexCentered && (p > 50 || p == -1))
	{
		std::cout << "Value p = " << p << " is to big for vertex centering" << std::endl;
		return { FundamentalTriangle(), false };
	}

	double pAngle = p == -1 ? 0.0 : PI / static_cast<double>(p);
	double qAngle = q == -1 ? 0.0 : PI / static_cast<double>(q);
	double rAngle = r == -1 ? 0.0 : PI / static_cast<double>(r);

	if (ts.centeringType == vertexCentered || p == 2)
	{
		auto ft = PVertexCenteredTriangle(pAngle, qAngle, rAngle);

		ft.pqrValues = { p,q,r };

		return { ft , true };
	}

	if (r == 2)
	{
		auto xAxisReflectedFundamentalTriangle = PVertexCenteredTriangle(rAngle, qAngle, pAngle);

		ReflectTriangleAboutXAxis(xAxisReflectedFundamentalTriangle);

		xAxisReflectedFundamentalTriangle.pqrValues = { p,q,r };

		return { xAxisReflectedFundamentalTriangle , true };
	}

	auto ft = FootOfTheQVertexAltitudeCenteredTriangle(pAngle, qAngle, rAngle);

	ft.pqrValues = { p,q,r };

	return { ft , true };
}

std::pair<FundamentalTriangle, bool> FillGlobalUniforms(const TilingSettings& ts, GlobalUniforms& gu)
{
	auto fundamentalTriangle = GetFundamentalTriangle(ts);

	if (!fundamentalTriangle.second)
	{
		return { FundamentalTriangle(), false};
	}

	const auto& ft = fundamentalTriangle.first;

	const auto& pqrVertices = ft.pqrVertices;

	gu.aSideNormalVector = EigenVectorToArray(HyperboloidModelLineNormalVector(pqrVertices.at(0), pqrVertices.at(2)));
	gu.aSideReflectionIsometryMatrix = EigenMatrixToArray(HyperboloidModelLineReflectionMatrix(pqrVertices.at(0), pqrVertices.at(2)));

	gu.bSideNormalVector = EigenVectorToArray(HyperboloidModelLineNormalVector(pqrVertices.at(2), pqrVertices.at(1)));
	gu.bSideReflectionIsometryMatrix = EigenMatrixToArray(HyperboloidModelLineReflectionMatrix(pqrVertices.at(2), pqrVertices.at(1)));

	gu.cSideNormalVector = EigenVectorToArray(HyperboloidModelLineNormalVector(pqrVertices.at(1), pqrVertices.at(0)));
	gu.cSideReflectionIsometryMatrix = EigenMatrixToArray(HyperboloidModelLineReflectionMatrix(pqrVertices.at(1), pqrVertices.at(0)));

	return { ft,true };
}

