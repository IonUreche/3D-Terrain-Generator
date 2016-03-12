#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion(double _x, double _y, double _z, double _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Quaternion::~Quaternion()
{
}

double Quaternion::length()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

void Quaternion::normalize()
{
	double L = length();
	x /= L;
	y /= L;
	z /= L;
	w /= L;
}

Quaternion Quaternion::conjugate()
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::mult(Quaternion &quat)
{
	return Quaternion(
		w*quat.x + x*quat.w + y*quat.z - z*quat.y,
		w*quat.y - x*quat.z + y*quat.w + z*quat.x,
		w*quat.z + x*quat.y - y*quat.x + z*quat.w,
		w*quat.w - x*quat.x - y*quat.y - z*quat.z);
}
