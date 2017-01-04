CC=gcc
FLAGS=-g
SOURCES = HashGen.c Algorithms.c HashTable.c RNG.c medoids.c data.c List.c
HEADERS = HashGen.h HashTable.h RNG.h data.h List.h Algorithms.h
OBJECTS = HashGen.o HashTable.o RNG.o data.o List.o
LIBRARIES=-lm
medoids: ${SOURCES} ${HEADERS}
	${CC} ${FLAGS} -o $@ ${SOURCES} ${LIBRARIES}

lsh: ${SOURCES} ${HEADERS}
	${CC} ${FLAGS} -o $@ ${SOURCES} ${LIBRARIES}

HashGen.o: HashGen.c HashGen.h RNG.h
	${CC} ${FLAGS} -c HashGen.c

HashTable.o: HashTable.c HashTable.h HashGen.h
	${CC} ${FLAGS} -c HashTable.c

RNG.o: RNG.c RNG.h
	${CC} ${FLAGS} -c RNG.c

data.o: data.c data.h
	${CC} ${FLAGS} -c data.c

List.o: List.c List.h
	${CC} ${FLAGS} -c List.c

Algorithms.o: Algorithms.c Algorithms.h
	${CC} ${FLAGS} -c Algorithms.c

clean:
	rm -f *.o lsh