#include <stdio.h>

#include "datatypes_and_constants.h"
#include "functions.h"

int main() {
	unsigned int chances[4][4] = {0,};
	unsigned char i, j;
	unsigned int iter;

	for(i=0; i<=3; ++i) {
		for(iter=0; iter<= REQUESTED_ITERATIONS; ++iter) {
			const unsigned int ENTRANCE = choose_exit(i)/4;
			chances[i][ENTRANCE]++;
			}

		for(j=0; j<=3; ++j) {
			const float ENTRY_CHANCE = chances[i][j] / (float)REQUESTED_ITERATIONS;
			printf("%1.2f\t", ENTRY_CHANCE);
		}
		printf("\n");
	}

	return 0;
}
