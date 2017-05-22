/*
 * (mu+lambda)-ES_WP.h
 *
 *  Created on: 04 apr 2017
 *      Author: RedShy
 */

#ifndef mu_lambda_ES_WP
#define mu_lambda_ES_WP

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

int evolutionStrategy_WP(const std::vector<unsigned>& s1,
		const std::vector<unsigned>& s2, const size_t& s1l, const size_t& s2l,

		const std::vector<unsigned>& sig1, const std::vector<unsigned>& sig2,
		const size_t& sig1l, const size_t& sig2l,

		const size_t& p1, matching_schema<bool>& m, edit_distance& e,

		const unsigned max_generations, const unsigned mu,
		const unsigned lambda)
{

	unsigned generation = 0;

	ES_MatchingSchema startingMS(sig1, sig2);

	//Generate mu random individuals
	ES_MatchingSchema parents[mu];
	for (unsigned i = 0; i < mu; ++i)
	{
		startingMS.shuffle();

		//validate matching schema
		if (ES_isValid(startingMS))
		{
			startingMS.costValue = e.edit_distance_matching_schema_enhanced(s1,
					s2, s1l, s2l, startingMS.sigma1, startingMS.sigma2, sig1l,
					sig2l, m);
			parents[i] = startingMS;
		}
		else
		{
			//TODO: not valid, maybe mutate until is valid?
			//repeat iteration
			i--;
		}
	}

//	unsigned same = 0;
	while (generation <= max_generations)
	{
		//Generate lambda children. Only mutation, no recombination
		for (unsigned i = 0; i < lambda; i++)
		{
			//Choose random parent
			unsigned p = rand() % mu;

			//Produce child, in the case parents=1 (like this) just clone
			ES_MatchingSchema child = parents[p];



			//mutate child
			child.mutate();


//			child.costValue = e.edit_distance_matching_schema_enhanced(s1, s2,
//					s1l, s2l, child.sigma1, child.sigma2, sig1l, sig2l, m);
//			if (child.costValue < parents[p].costValue)
//			{
//				std::cout << "IMPROVED!";
//			}
//			else if (child.costValue > parents[p].costValue)
//			{
//				std::cout << "WORSE!\n";
//			}
//			else
//			{
//				std::cout << "SAME VALUE!\n";
//				same++;
//				std::cout << "BEFORE:\n";
//				m.print_matching_schema(parents[p].sigma1, parents[p].sigma2);
//				std::cout << "AFTER:\n";
//				m.print_matching_schema(child.sigma1, child.sigma2);
//			}
//			std::cout << "-------------------------------------\n";

			//validate child
			if (ES_isValid(child))
			{

				//select the worst parent, mu is always very very small like 5 or 10
				unsigned worstParentCostValue = parents[0].costValue;
				unsigned worstParent = 0;
				for (unsigned i = 1; i < mu; i++)
				{
					if (parents[i].costValue > worstParentCostValue)
					{
						worstParentCostValue = parents[i].costValue;
						worstParent = i;
					}
				}

				int newDistance =
						e.edit_distance_matching_schema_enhanced_with_diagonal(
								s1,
						s2, s1l, s2l, child.sigma1, child.sigma2, sig1l, sig2l,
						m, worstParentCostValue);

				if (newDistance != -1)
				{
					//The child is better than the worst parent, so he become a new parent
					child.costValue = newDistance;
					parents[worstParent] = child;
				}

//				else child discarded
			}
			else
			{
				//TODO: not valid, maybe mutate until is valid?
				//repeat iteration
				i--;
			}
		}
		generation++;
	}

	//TODO return best of all
	unsigned bestValue = parents[0].costValue;
	unsigned bestParent = 0;
	for (unsigned i = 1; i < mu; i++)
	{
		if (parents[i].costValue < bestValue)
		{
			bestValue = parents[i].costValue;
			bestParent = i;
		}
	}
//	std::cout << "SAME VALUE:" << same << "\n";
	std::cout << bestValue;
	return bestValue;

}

#endif
