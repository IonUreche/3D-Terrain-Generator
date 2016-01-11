#pragma once

class RNG
{
public:

	static float GetRandFloat(float rangeMin = 0.0f, float rangeMax = 1.0f);

private:

	static bool m_setSeed;

	static bool SetSeed();
};




