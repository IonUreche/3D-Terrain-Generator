#include "stdafx.h"

#include "RNG.h"

float RNG::getFloat(float a, float b)
{
	std::uniform_real_distribution<float> dist(a, b);
	return dist(m_generator);
}

int RNG::getInt(int a, int b)
{
	std::uniform_int_distribution<int> dist(a, b);
	return dist(m_generator);
}

bool RNG::SetSeed()
{
	return m_setSeed;
}

bool RNG::m_setSeed = false;
std::default_random_engine RNG::m_generator;