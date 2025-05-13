#pragma once
#include <array>
#include <string>

class PoincareDiskModelPoint;

class HyperboloidModelPoint
{
private:
	std::array<double, 3> m_point;

public:
	HyperboloidModelPoint();
	HyperboloidModelPoint(double x, double y);
	HyperboloidModelPoint(const std::array<double, 2>& point);
	HyperboloidModelPoint(const std::array<double, 3>& point);

	const std::array<double, 3>& GetPoint() const;
	std::array<double, 3>& GetPoint();
	const double& X() const;
	const double& Y() const;
	const double& W() const;
	double& X();
	double& Y();
	double& W();

	bool operator== (const HyperboloidModelPoint& anotherPoint) const;
	bool operator!= (const HyperboloidModelPoint& anotherPoint) const;

	PoincareDiskModelPoint ToPoincareDiskModel() const;

	std::string ToString() const;
};

double HyperbolicDistance(const HyperboloidModelPoint& u, const HyperboloidModelPoint& v);

