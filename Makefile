all: crabstar.exe

debug: crabstarDebug.exe

crabstar.exe: crabstar.o
	g++ -oFast crabstar.cpp -o crabstar
	x86_64-w64-mingw32-gcc -oFast crabstar.cpp -o crabstar.exe

crabstarDebug.exe: crabstar.o
	g++ crabstar.cpp -o crabstarDebug
	x86_64-w64-mingw32-gcc crabstar.cpp -o crabstarDebug.exe

crabstar.o: bitboard.h

.PHONY: clean
clean:
		rm -f *.exe *.o *~ a.out crabstar crabstarDebug 