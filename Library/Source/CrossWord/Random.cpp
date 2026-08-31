#include "CrossWord/Random.h"
#include <time.h>

using namespace CrossWord;

Random::Random()
{
	this->generator.seed(this->randomDevice());
}

/*virtual*/ Random::~Random()
{
}

void Random::SetSeed(int seed)
{
	this->generator.seed(seed);
}

void Random::SetSeedUsingTime()
{
	this->generator.seed((unsigned int)time(nullptr));
}

int Random::InRange(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(this->generator);
}

void Random::MakeRandomPermutation(std::vector<int>& permutation, int size)
{
	permutation.resize(size);

	for (int i = 0; i < size; i++)
		permutation[i] = i;

	this->Shuffle(permutation.data(), (uint32_t)permutation.size());
}