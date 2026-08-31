#pragma once

#include <random>
#include <vector>

namespace CrossWord
{
	class Random
	{
	public:
		Random();
		virtual ~Random();

		void SetSeed(int seed);
		void SetSeedUsingTime();
		int InRange(int min, int max);
		void MakeRandomPermutation(std::vector<int>& permutation, int size);

		template<typename T>
		void Shuffle(T* givenArray, uint32_t givenArraySize)
		{
			for (uint32_t i = 0; i < givenArraySize - 1; i++)
			{
				uint32_t j = this->InRange(int(i), int(givenArraySize - 1));
				if (i != j)
				{
					T temp = givenArray[i];
					givenArray[i] = givenArray[j];
					givenArray[j] = temp;
				}
			}
		}

	protected:
		std::random_device randomDevice;
		std::mt19937 generator;
	};
}