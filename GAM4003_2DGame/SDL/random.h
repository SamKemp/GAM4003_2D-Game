/*
File: random.h
Author: Peter Lager
Version: 2.01
Date Last Modified: 31-12-2006

Include file that provides a wrapper object to provide
a number of utility functions that use the pseudonumber
generator provide the C++.

To use the features in this facility you need to #include
this file and add the following line
using namespace random;

*/

#ifndef __RANDOM_H
#define __RANDOM_H

//------------------------  INCLUDES ----------------------------
#include <cstdlib>
#include <time.h>

using namespace std;

namespace random
{
	static class rand_dev
	{
	public:
		/*
		Constructor initialises random seed with the current system
		time to prevent repeatable series of numbers
		*/
		rand_dev()
		{
			srand((unsigned int)time(0));
		}

		// Returns an integer value in the range >=0 and <lt
		int number(int lt)
		{
			return rand() % lt;
		}

		// Returns an integer value in the range >=gte and <lt
		int number(int gte, int lt)
		{
			return gte + rand() % (lt - gte);
		}

		// Returns a float value in the range >=0.0 and <1.0
		float number()
		{
			return (float)rand() / ((float)RAND_MAX + 1.0f);
		}

		// Returns a float value in the range >=0.0 and <lt
		float number(float lt)
		{
			return lt * (float)rand() / ((float)RAND_MAX + 1.0f);
		}

		// Returns a float value in the range >=gte and <lt
		float number(float gte, float lt)
		{
			return gte + (lt - gte) * ((float)rand() / ((float)RAND_MAX + 1.0f));
		}

		/*
		Start the random numbers from a known seed to get a
		repeatable series of random numbers
		*/
		void seed(int s)
		{
			srand(s);
		}
	}rnd;
} // End of namespace

#endif // #ifndef __RANDOM_H