avifps:
	g++ -std=c++11 avifps.cpp -o avifps.exe -lvfw32
	
clean:
	rm -f *.o *.exe