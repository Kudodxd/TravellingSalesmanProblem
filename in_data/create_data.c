#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CITIES 1000

typedef struct {
    int id;
    int x;
    int y;
} City;

void generateRandomCities(City cities[NUM_CITIES]) {
    srand(time(NULL));

    for (int i = 0; i < NUM_CITIES; ++i) {
        cities[i].id = i + 1;
        cities[i].x = rand() % 10000; // Adjust the range as needed
        cities[i].y = rand() % 10000; // Adjust the range as needed
    }
}

void printCitiesData(City cities[NUM_CITIES]) {
    printf("DIMENSION: %d\n", NUM_CITIES);
    printf("EDGE_WEIGHT_TYPE: EUC_2D\n");
    printf("NODE_COORD_SECTION\n");

    for (int i = 0; i < NUM_CITIES; ++i) {
        printf("%4d %d %d\n", cities[i].id, cities[i].x, cities[i].y);
    }

    printf("EOF\n");
}

int main() {
    City cities[NUM_CITIES];
    printf("NAME : m1000\nCOMMENT : 1000 cities\nTYPE : TSP\n");

    generateRandomCities(cities);
    printCitiesData(cities);

    return 0;
}
