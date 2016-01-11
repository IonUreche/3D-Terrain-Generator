#include "RNG.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

float RNG::GetRandFloat(float rangeMin, float rangeMax)
{
	if (!SetSeed())
	{
		srand((unsigned int)time(NULL));
		m_setSeed = true;
	}
	if (rangeMin > rangeMax) return -1.0f;
	float rZeroOne = ((float)rand() / (float) RAND_MAX);
	return rZeroOne * (rangeMax - rangeMin) + rangeMin;
}


bool RNG::SetSeed()
{
	return m_setSeed;
}

bool RNG::m_setSeed = false;

