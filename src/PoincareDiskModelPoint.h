#pragma once
#include <array>
#include <string>

class HyperboloidModelPoint;

class PoincareDiskModelPoint
{
private:
	std::array<double, 2> m_point;

public:

	PoincareDiskModelPoint();
	PoincareDiskModelPoint(double x, double y);
	PoincareDiskModelPoint(const std::array<double, 2>& point);

	const std::array<double, 2>& GetPoint() const;
	std::array<double, 2>& GetPoint();
	const double& X() const;
	const double& Y() const;
	double& X();
	double& Y();

	bool operator== (const PoincareDiskModelPoint& anotherPoint) const;
	bool operator!= (const PoincareDiskModelPoint& anotherPoint) const;

	HyperboloidModelPoint ToHyperboloidModel() const;

	std::string ToString() const;
	friend std::ostream& operator<<(std::ostream& os, const  PoincareDiskModelPoint& point);

	bool IsIdeal() const;
};

double HyperbolicDistance(const PoincareDiskModelPoint& u, const PoincareDiskModelPoint& v);

void ReflectPoincareDiskModelPointAboutXAxis(PoincareDiskModelPoint& point);

template<size_t N> void ReflectPoincareDiskModelPointAboutXAxis(std::array<PoincareDiskModelPoint, N>& arrayOfPoints);

template<size_t N>
inline void ReflectPoincareDiskModelPointAboutXAxis(std::array<PoincareDiskModelPoint, N>& arrayOfPoints)
{
	for (size_t i{ 0 }; i < arrayOfPoints.size(); i++)
	{
		ReflectPoincareDiskModelPointAboutXAxis(arrayOfPoints.at(i));
	}
}
