#include "HyperboloidModelPoint.h"
#include "PoincareDiskModelPoint.h"

HyperboloidModelPoint::HyperboloidModelPoint() : HyperboloidModelPoint(0.0, 0.0)
{
}

HyperboloidModelPoint::HyperboloidModelPoint(double x, double y) : m_point({sqrt(1.0 + x*x + y*y),x,y})
{
}

HyperboloidModelPoint::HyperboloidModelPoint(const std::array<double, 2>& point) : HyperboloidModelPoint(point.at(0), point.at(1))
{
}

HyperboloidModelPoint::HyperboloidModelPoint(const std::array<double, 3>& point) : HyperboloidModelPoint(point.at(1), point.at(2))
{
}

const std::array<double, 3>& HyperboloidModelPoint::GetPoint() const
{
	return m_point;
}

std::array<double, 3>& HyperboloidModelPoint::GetPoint()
{
	return m_point;
}

const double& HyperboloidModelPoint::X() const
{
	return m_point.at(1);
}

const double& HyperboloidModelPoint::Y() const
{
	return m_point.at(2);
}

const double& HyperboloidModelPoint::W() const
{
	return m_point.at(0);
}

double& HyperboloidModelPoint::X()
{
	return m_point.at(1);
}

double& HyperboloidModelPoint::Y()
{
	return m_point.at(2);
}

double& HyperboloidModelPoint::W()
{
	return m_point.at(0);
}

bool HyperboloidModelPoint::operator==(const HyperboloidModelPoint& anotherPoint) const
{
	return (m_point.at(2) == anotherPoint.m_point.at(2)) && (m_point.at(1) == anotherPoint.m_point.at(1));
}

bool HyperboloidModelPoint::operator!=(const HyperboloidModelPoint& anotherPoint) const
{
	return !((*this) == anotherPoint);
}

PoincareDiskModelPoint HyperboloidModelPoint::ToPoincareDiskModel() const
{
	return PoincareDiskModelPoint(X() / (1.0 + W()), Y() / (1.0 + W()));
}

std::string HyperboloidModelPoint::ToString() const
{
	std::string stringHyperbolicPoint = "(";

	stringHyperbolicPoint += std::to_string(m_point.at(0)) + ", ";
	stringHyperbolicPoint += std::to_string(m_point.at(1)) + ", ";
	stringHyperbolicPoint += std::to_string(m_point.at(2)) + ")";
	stringHyperbolicPoint += "_H";

	return stringHyperbolicPoint;
}

double HyperbolicDistance(const HyperboloidModelPoint& u, const HyperboloidModelPoint& v)
{
	return acosh(u.W() * v.W() - u.X() * v.X() - u.Y() * v.Y());
}
