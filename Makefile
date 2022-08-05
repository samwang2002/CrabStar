CC = gcc
CFLAGS = -g #-Wall
LDFLAGS = -g
LDLIBS = -lm

#main: main.o bishop.o bitboard.o board.o game.o king.o knight.o move.o net.o pawn.o queen.o random.o rook.o search.o
main: main.o bishop.o bitboard.o board.o hash.o king.o knight.o move.o net.o pawn.o perft.o queen.o rook.o search.o timecontrol.o uci.o
main.o: main.c
bishop.o: bishop.c bishop.h
bitboard.o: bitboard.c bitboard.h
board.o: board.c board.h
#game.o: game.c game.h
king.o: king.c king.h
knight.o: knight.c knight.h
move.o: move.c move.h
net.o: net.c net.h
pawn.o: pawn.c pawn.h
queen.o: queen.c queen.h
#random.o: random.c random.h
rook.o: rook.c rook.h
search.o: search.c search.h
uci.o: uci.c uci.h
timecontrol.o: timecontrol.c timecontrol.h

.phony: clean
clean:
	rm -f *.o main
