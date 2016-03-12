#pragma once
class Quaternion
{
public:

	Quaternion(double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0);
	~Quaternion();

	double length();

	void normalize();

	Quaternion conjugate();
	Quaternion mult(Quaternion &quat);

	double x, y, z, w;
};

