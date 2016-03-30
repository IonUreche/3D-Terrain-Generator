#pragma once
class PerlinNoise
{
public:

	static double noise(double x, double y, double z);
	static double OctavePerlin(double x, double y, double z, int octaves, double persistence);

private:

	static bool initialized;
	static void init(unsigned int seed);
	static double fade(double t);
	static double lerp(double t, double a, double b);
	static double grad(int hash, double x, double y, double z);
	static int permutation[256], p[512];
};

