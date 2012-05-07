#ifndef __FUNCTIONS_C__
#define __FUNCTIONS_C__

#include <stdlib.h>
#include <time.h>

#include "datatypes_and_constants.h"

/**
 * Funkcja losująca docelowy wyjazd samochodu w zależności od wjazdu, na którym samochód się pojawi.
 */
int choose_exit(const Entrance entrance_number) {
	float dist_n, dist_w, dist_s;
	Entrance exit_no;

	extern const float CHANCES[4][4];

	const float RANDOM = (float)rand() / (float)RAND_MAX;

	dist_n = CHANCES[entrance_number][0];
	dist_w = dist_n + CHANCES[entrance_number][1];
	dist_s = dist_w + CHANCES[entrance_number][2];

	if(RANDOM < dist_n) exit_no = N;
	else if(dist_n <= RANDOM && RANDOM < dist_w) exit_no = W;
	else if(dist_w <= RANDOM && RANDOM < dist_s) exit_no = S;
	else exit_no = E;
	//fprintf(FULL_DEBUG_TARGET, "| Zrodlo[%d] Cel[%d] rand[%f]", offset[entrance_number], offset[exit_no], random);

	return offset[exit_no];
}

#endif
