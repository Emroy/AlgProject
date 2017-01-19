CC=gcc
FLAGS=-g
SOURCES = HashGen.c Clustering.c HashTable.c clusteringRecommendation.c data.c List.c Ratings.c recommendations.c RNG.c
HEADERS = Clustering.h clusteringRecommendation.h data.h HashGen.h HashTable.h List.h Ratings.h RNG.h
OBJECTS = Clustering.o clusteringRecommendation.o data.o HashGen.o HashTable.o List.o Ratings.o RNG.o recommendations.o
LIBRARIES = -lm

recommendations: ${SOURCES} ${HEADERS} ${OBJECTS}
	${CC} ${FLAGS} -o recommendations ${OBJECTS} ${LIBRARIES}

Clustering.o: Clustering.h HashTable.h List.h data.h Ratings.h Clustering.c
	${CC} ${FLAGS} -c Clustering.c

clusteringRecommendation.o: clusteringRecommendation.h clusteringRecommendation.c
	${CC} ${FLAGS} -c clusteringRecommendation.c

data.o: data.h Ratings.h data.c
	${CC} ${FLAGS} -c data.c

HashGen.o: HashGen.h RNG.h data.h HashGen.c
	${CC} ${FLAGS} -c HashGen.c

HashTable.o: HashTable.h HashGen.h HashTable.c
	${CC} ${FLAGS} -c HashTable.c

List.o: List.h List.c
	${CC} ${FLAGS} -c List.c

Ratings.o: Ratings.h List.h Ratings.c
	${CC} ${FLAGS} -c Ratings.c

RNG.o: RNG.h RNG.c
	${CC} ${FLAGS} -c RNG.c -lm

recommendations.o: Ratings.h Clustering.h recommendations.c
	${CC} ${FLAGS} -c recommendations.c

clean:
	rm *.o
	rm recommendations