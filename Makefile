# all: main.o uci.o perft.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o timecontrol.o hash.o net.o search.o
# 	gcc -Ofast main.o uci.o perft.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o timecontrol.o hash.o net.o search.o -o crabstar
# 	x86_64-w64-mingw32-g++ -Ofast  main.o uci.o perft.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o timecontrol.o hash.o net.o search.o -o crabstar.exe

# debug:  main.o uci.o perft.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o timecontrol.o hash.o net.o search.o
# 	gcc -g main.o uci.o perft.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o timecontrol.o hash.o net.o search.o -o crabstar
# 	x86_64-w64-mingw32-gcc -g  main.o uci.o perft.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o timecontrol.o hash.o net.o search.o -o crabstar.exe

# main.o:  uci.h perft.h constants.h move.h board.h bitboard.h bishop.h king.h knight.h pawn.h queen.h rook.h timecontrol.h hash.h net.h search.h

# net.o: net.h bitboard.h board.h constants.h

# search.o: search.h board.h move.h constants.h timecontrol.h hash.h net.h

# uci.o: uci.h move.h board.h constants.h timecontrol.h hash.h

# move.o: move.h bitboard.h board.h

# perft.o: perft.h board.h move.h constants.h

# board.o: board.h move.h bitboard.h constants.h pawn.h knight.h bishop.h rook.h queen.h king.h timecontrol.h

# bitboard.o: bitboard.h

# bishop.o: bishop.h move.h bitboard.h constants.h

# king.o: king.h move.h bitboard.h constants.h

# knight.o: knight.h move.h bitboard.h constants.h

# pawn.o: pawn.h move.h bitboard.h constants.h

# queen.o: queen.h move.h bitboard.h constants.h

# rook.o: rook.h move.h bitboard.h constants.h

# timecontrol.o: timecontrol.h perft.h

# hash.o: hash.h bitboard.h board.h

# clean:
# 	del -f *.o *.exe
# 	rm -f *.o *.exe

all: main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o hash.o
	gcc -Ofast main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o hash.o -o crabstar
	x86_64-w64-mingw32-g++ -Ofast  main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o hash.o  net.o search.o random.o game.o -o crabstar.exe

debug:  main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o hash.o
	gcc -g main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o hash.o -o crabstar
	x86_64-w64-mingw32-g++ -g main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o hash.o -o crabstar.exe

main.o: constants.h move.h board.h bitboard.h bishop.h king.h knight.h pawn.h queen.h rook.h net.h search.h game.h hash.h

game.o: game.h net.h search.h board.h constants.h

random.o: random.h

net.o: net.h bitboard.h board.h constants.h random.h

search.o: search.h board.h move.h constants.h net.h hash.h

move.o: move.h bitboard.h board.h

board.o: board.h move.h bitboard.h constants.h pawn.h knight.h bishop.h rook.h queen.h king.h hash.h

bitboard.o: bitboard.h

bishop.o: bishop.h move.h bitboard.h constants.h

king.o: king.h move.h bitboard.h constants.h

knight.o: knight.h move.h bitboard.h constants.h

pawn.o: pawn.h move.h bitboard.h constants.h

queen.o: queen.h move.h bitboard.h constants.h

rook.o: rook.h move.h bitboard.h constants.h

hash.h: hash.h constants.h

clean:
	del -f *.o *.exe
	rm -f *.o *.exe

# all: main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o 
# 	gcc -Ofast main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o -o crabstar
# 	x86_64-w64-mingw32-g++ -Ofast  main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o hash.o  net.o search.o random.o game.o -o crabstar.exe

# debug:  main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o 
# 	gcc -g main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o -o crabstar
# 	x86_64-w64-mingw32-g++ -g main.o move.o board.o bitboard.o bishop.o king.o knight.o pawn.o queen.o rook.o net.o search.o random.o game.o -o crabstar.exe

# main.o: constants.h move.h board.h bitboard.h bishop.h king.h knight.h pawn.h queen.h rook.h net.h search.h game.h 

# game.o: game.h net.h search.h board.h constants.h

# random.o: random.h

# net.o: net.h bitboard.h board.h constants.h random.h

# search.o: search.h board.h move.h constants.h net.h

# move.o: move.h bitboard.h board.h

# board.o: board.h move.h bitboard.h constants.h pawn.h knight.h bishop.h rook.h queen.h king.h

# bitboard.o: bitboard.h

# bishop.o: bishop.h move.h bitboard.h constants.h

# king.o: king.h move.h bitboard.h constants.h

# knight.o: knight.h move.h bitboard.h constants.h

# pawn.o: pawn.h move.h bitboard.h constants.h

# queen.o: queen.h move.h bitboard.h constants.h

# rook.o: rook.h move.h bitboard.h constants.h


# clean:
# 	del -f *.o *.exe
# 	rm -f *.o *.exe