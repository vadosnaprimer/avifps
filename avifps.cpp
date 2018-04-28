#include <iostream>
#include <windows.h>
#include <vfw.h>

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		PAVIFILE pfile;
		AVIFILEINFOW afi;

		const char *filename = argv[i];

		AVIFileInit();
		AVIFileOpen(&pfile, filename, OF_SHARE_DENY_WRITE, 0L);
		pfile->Info(&afi, sizeof(afi));

		DWORD num = afi.dwRate;
		DWORD denom = afi.dwScale;

		std::cout.precision(std::numeric_limits<double>::max_digits10);

		std::cout
			<< "file:        " << filename          << std::endl
			<< "framerate:   " << (double)num/denom << std::endl
			<< "numerator:   " << num               << std::endl
			<< "denominator: " << denom             << std::endl
			<< std::endl;

		AVIFileRelease(pfile);
		AVIFileExit();
	}

	std::cin.get();

	return 0;
}
