/*
 * (1+1)-ES_RA.h
 *
 *  Created on: 04 apr 2017
 *      Author: RedShy
 */
#ifndef ES_ONE_ONE_SRS
#define ES_ONE_ONE_SRS

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <queue>
#include "ES_MatchingSchema.h"
#include "EditDistance.h"
#include "MatchingSchema.h"

int evolutionStrategy_one_one_srs(const std::vector<unsigned>& s1,
		const std::vector<unsigned>& s2, const size_t& s1l, const size_t& s2l,

		const std::vector<unsigned>& sig1, const std::vector<unsigned>& sig2,
		const size_t& sig1l, const size_t& sig2l,

		const size_t& p1, const size_t& p2, matching_schema<bool>& m,
		edit_distance& e,

		const unsigned max_generations, const unsigned maxAttempts)
{
	clock_t start = clock();
	long double msElapsed = 0;

	//Initialize stuff for the mutator swap2-E
	const unsigned * const blocksig1 = initializeBlocksSwap2E(sig1, p1);
	const unsigned * const blocksig2 = initializeBlocksSwap2E(sig2, p2);

	//Just for printing, no other purpose
	unsigned totalGeneration = 0;

	ES_MatchingSchema parent(sig1, sig2);

	//Random start, delete shuffle for starting with a given matching schema
	parent.shuffle();

	parent.costValue = e.edit_distance_matching_schema_enhanced(s1, s2, s1l,
			s2l, parent.sigma1, parent.sigma2, sig1l, sig2l, m);

	ES_MatchingSchema best = parent;

	clock_t timeElapsed1 = clock() - start;
	msElapsed = timeElapsed1 / CLOCKS_PER_MS;
	std::cout << msElapsed << " " << best.costValue << "\n";

	unsigned attempts = 0;
	while (attempts < maxAttempts)
	{
		unsigned generation = 0;

		while (generation <= max_generations)
		{
			//Produce child
			ES_MatchingSchema child = parent;

			//mutate child
			child.swap2_enhanced(blocksig1, blocksig2);

			int newDistance =
					e.edit_distance_matching_schema_enhanced_with_diagonal(s1,
							s2, s1l, s2l, child.sigma1, child.sigma2, sig1l,
							sig2l, m, parent.costValue);

			if (newDistance != -1)
			{
				//The child is better than its father, so he become new parent
				parent = child;
				parent.costValue = newDistance;

				if (parent.costValue < best.costValue)
				{
					best.costValue = parent.costValue;

					clock_t timeElapsed = clock() - start;
					msElapsed = timeElapsed / CLOCKS_PER_MS;
					std::cout << msElapsed << " " << best.costValue << "\n";
				}
			}

			generation++;

			//For printing stuff
			totalGeneration++;
		}

		//Random restart
		parent.shuffle();
		parent.costValue = e.edit_distance_matching_schema_enhanced(s1, s2, s1l,
				s2l, parent.sigma1, parent.sigma2, sig1l, sig2l, m);

		attempts++;
	}

	delete[] blocksig1;
	delete[] blocksig2;

	clock_t timeElapsed = clock() - start;
	msElapsed = timeElapsed / CLOCKS_PER_MS;
	std::cout << msElapsed << " " << best.costValue << "\n";

//	std::cout<<best.costValue;
	return best.costValue;
}

#endif /* SRC__1_1__ES_H_ */

