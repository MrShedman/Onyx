#include "MathHelper.hpp"
#include <random>

float randomize(float min, float max)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}
