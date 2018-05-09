avifps:
	g++ -std=c++11 avifps.cpp -o avifps.exe -lvfw32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
	
clean:
	rm -f *.o *.exe