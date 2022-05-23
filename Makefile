.PHONY: all
all: crabstar.o
	g++ -oFast crabstar.cpp -o crabstar
	x86_64-w64-mingw32-g++ -oFast crabstar.cpp -o crabstar.exe

debug: crabstar.o
	g++ crabstar.cpp -o crabstar
	x86_64-w64-mingw32-g++ crabstar.cpp -o crabstar.exe

crabstar.o: bitboard.h

.PHONY: clean
clean:
		rm -f *.exe *.o *~ a.out crabstar crabstarDebug 