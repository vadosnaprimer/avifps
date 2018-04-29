//-------------------------------------------------------------------
//
//	avifps
//
//	Program to compute the least common framerate of AVI segments
//	that have different framerates. Uses a method to find the least
//	common multiple of _fractions_ described here:
//	https://eu.edugain.com/articles/6/LCM-of-Fractions/
//
//	Copyright (c) 2018 feos
//
//-------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <windows.h>
#include <vfw.h>

typedef unsigned long long u64;


//-------------------------------------------------------------------
//	gcd2 - greatest common divider for 2 integers
//-------------------------------------------------------------------

u64 gcd2(u64 a, u64 b)
{
	for (;;)
	{
		if (a == 0) return b;
		b %= a;
		if (b == 0) return a;
		a %= b;
	}
}


//-------------------------------------------------------------------
//	lcm2 - least common multiple of 2 integers
//-------------------------------------------------------------------

u64 lcm2(u64 a, u64 b)
{
	u64 temp = gcd2(a, b);
	return temp ? (a / temp * b) : 0;
}


//-------------------------------------------------------------------
//	lcmv - least common multiple of vector elements
//-------------------------------------------------------------------

u64 lcmv(std::vector<u64> &v)
{
	return std::accumulate(v.begin(), v.end(), (u64)1, lcm2);
}


//-------------------------------------------------------------------
//	main - extects avi filenames
//-------------------------------------------------------------------

int main(int argc, char** argv)
{
	if (argc == 1)								// no files provided, bail
	{
		std::cout << "Drag'n'drop AVI files";
		std::cin.get();							// make the console persist
		return 0;
	}

	std::vector<u64> nums;						// vector of numerators
	std::vector<u64> denoms;					// vector of denominators

	std::cout.precision(std::numeric_limits<double>::max_digits10);

	for (int i = 1; i < argc; i++)
	{
		PAVIFILE pfile;
		AVIFILEINFOW afi;

		const char *file = argv[i];

		AVIFileInit();
		AVIFileOpen(&pfile, file, OF_SHARE_DENY_WRITE, 0L);
		pfile->Info(&afi, sizeof(afi));

		u64 num = afi.dwRate;
		u64 denom = afi.dwScale;
		u64 gcd = gcd2(num, denom);

		num /= gcd;
		denom /= gcd;

		nums.push_back(num);
		denoms.push_back(denom);

		double fps = (double)num / denom;

		std::cout
			<< "filename:    " << file  << std::endl
			<< "framerate:   " << fps   << std::endl
			<< "numerator:   " << num   << std::endl
			<< "denominator: " << denom << std::endl
			<< std::endl;

		AVIFileRelease(pfile);
		AVIFileExit();

		if (argc == 2)							// just 1 file provided, bail
		{
			std::cin.get();						// make the console persist
			return 0;
		}
	}
	
	u64 denom = lcmv(denoms);					// least common denominator

	for (unsigned i = 0; i < nums.size(); i++)
		nums.at(i) *= (denom / denoms.at(i));	// cross-multiply numerators

	u64 num = lcmv(nums);						// least common numerator
	u64 gcd = gcd2(num, denom);					// reduce if possible
	num /= gcd;
	denom /= gcd;
	double fps = (double)num / denom;			// least common framerate

	std::cout
		<< "least common framerate:   " << fps   << std::endl
		<< "least common numerator:   " << num   << std::endl
		<< "least common denumerator: " << denom << std::endl
		<< std::endl;

	std::cin.get();								// make the console persist
	return 0;
}
