#pragma once
class PerlinNoise
{
public:

	static double noise(double x, double y, double z);
	static double OctavePerlin(double x, double y, double z, int octaves, double persistence);
	static double noise2D(double xin, double yin);
private:
	
	static void init(unsigned int seed);
	static double fade(double t);
	static double lerp(double t, double a, double b);
	static double grad(int hash, double x, double y, double z);
	static int fastfloor(double x);
	static double dot(int g[], double x, double y);
	static double dot(int g[], double x, double y, double z);
	static double mix(double a, double b, double t);

	static bool initialized;
	static int perm[256], p[512], grad3[12][3];
};

