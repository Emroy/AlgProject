CC=gcc
FLAGS=-g
SOURCES = HashGen.c HashTable.c RNG.c main.c distances.c
HEADERS = HashGen.h HashTable.h RNG.h distances.h
OBJECTS = HashGen.o HashTable.o RNG.o distances.o
LIBRARIES=-lm
lsh: ${SOURCES} ${HEADERS}
	${CC} ${FLAGS} -o $@ ${SOURCES} ${LIBRARIES}

HashGen.o: HashGen.c HashGen.h RNG.h
	${CC} ${FLAGS} -c HashGen.c

HashTable.o: HashTable.c HashTable.h HashGen.h
	${CC} ${FLAGS} -c HashTable.c

RNG.o: RNG.c RNG.h
	${CC} ${FLAGS} -c RNG.c

distances.o: distances.c distances.h
	${CC} ${FLAGS} -c distances.c

clean:
	rm -f *.o lsh