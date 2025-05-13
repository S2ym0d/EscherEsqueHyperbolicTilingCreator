#include "PoincareDiskModelPoint.h"
#include "HyperboloidModelPoint.h"
#include <iostream>

PoincareDiskModelPoint::PoincareDiskModelPoint() : PoincareDiskModelPoint(0.0, 0.0)
{
}

PoincareDiskModelPoint::PoincareDiskModelPoint(double x, double y) : m_point({x, y})
{
}

PoincareDiskModelPoint::PoincareDiskModelPoint(const std::array<double, 2>& point) : PoincareDiskModelPoint(point.at(0), point.at(1))
{
}

const std::array<double, 2>& PoincareDiskModelPoint::GetPoint() const
{
	return m_point;
}

std::array<double, 2>& PoincareDiskModelPoint::GetPoint()
{
	return m_point;
}

const double& PoincareDiskModelPoint::X() const
{
	return m_point.at(0);
}

const double& PoincareDiskModelPoint::Y() const
{
	return m_point.at(1);
}

double& PoincareDiskModelPoint::X()
{
	return m_point.at(0);
}

double& PoincareDiskModelPoint::Y()
{
	return m_point.at(1);
}

bool PoincareDiskModelPoint::operator==(const PoincareDiskModelPoint& anotherPoint) const
{
	return (m_point.at(0) == anotherPoint.m_point.at(0)) && (m_point.at(1) == anotherPoint.m_point.at(1));
}

bool PoincareDiskModelPoint::operator!=(const PoincareDiskModelPoint& anotherPoint) const
{
	return !((*this) == anotherPoint);
}

HyperboloidModelPoint PoincareDiskModelPoint::ToHyperboloidModel() const
{
	double normSquared = pow(X(), 2.0) + pow(Y(), 2.0);

	return HyperboloidModelPoint(2.0 * X() / (1.0 - normSquared), 2.0 * Y() / (1.0 - normSquared));
}

std::string PoincareDiskModelPoint::ToString() const
{
	return std::string("(") + std::to_string(X()) + ", " + std::to_string(Y()) + ")" + "_PD";
}

bool PoincareDiskModelPoint::IsIdeal() const
{
	double normSquared = pow(X(), 2.0) + pow(Y(), 2.0);

	if ((1.0 - normSquared) > 0.0) return false;

	return true;
}

double PoincareDiskIsometricInvariant(const PoincareDiskModelPoint& u, const PoincareDiskModelPoint& v)
{
	double normSquareduv = pow(u.X() - v.X(), 2.0) + pow(u.Y() - v.Y(), 2.0);
	double normSquaredu = pow(u.X(), 2.0) + pow(u.Y(), 2.0);
	double normSquaredv = pow(v.X(), 2.0) + pow(v.Y(), 2.0);

	return 2.0 * normSquareduv / ((1.0 - normSquaredu) * (1.0 - normSquaredv));
}

std::ostream& operator<<(std::ostream& os, const PoincareDiskModelPoint& point)
{
	os << point.ToString();

	return os;
}

double HyperbolicDistance(const PoincareDiskModelPoint& u, const PoincareDiskModelPoint& v)
{
	return acosh(1.0 + PoincareDiskIsometricInvariant(u, v));
}

void ReflectPoincareDiskModelPointAboutXAxis(PoincareDiskModelPoint& point)
{
	point.Y() = -point.Y();
}
