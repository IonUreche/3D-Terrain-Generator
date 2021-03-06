#pragma once

class RNG
{
public:

	static double getDouble(double a = 0.0, double b = 1.0);
	static float getFloat(float a = 0.0f, float b = 1.0f);
	static int getInt(int a = 0, int b = INT_MAX);

private:

	static std::default_random_engine m_generator;

	static bool m_setSeed;

	static bool SetSeed();
};




