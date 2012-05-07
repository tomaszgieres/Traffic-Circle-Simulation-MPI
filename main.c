#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

#define SIMPLE_DEBUG_TARGET stdout
#define FULL_DEBUG_TARGET stdout

// requested iterations
// choose exit
// offset

/**
 * Lista możiliwych wjazdów na rondo
 */


int main() {
	srand(time(NULL));

	int circle[16];
	int new_circle[16];

	unsigned char arrival[4];
	unsigned int arrival_cnt[4];
	unsigned int wait_cnt[4];
	unsigned int queue[4];
	unsigned int queue_accum[4]= {0,};

	int i = 0;
	unsigned int iteration = 0;


	const int f[4] = {3, 3, 4, 2}; // Original: {3, 3, 4, 2};

	for(i = 0; i<= 15; ++i) {
		circle[i] = -1;
	} // endfor
	for(i=0; i<=3; i++) {
		arrival_cnt[i] = 0;
		wait_cnt[i] = 0;
		queue[i] = 0;
		queue_accum[i] = 0;
	} // endfor


	fprintf(FULL_DEBUG_TARGET, "## Start ##\n");

	for(iteration = 0; iteration <= REQUESTED_ITERATIONS; iteration++) {
		unsigned int k;
		for(k=0; k<=15; ++k) {
			if(circle[k] == -1) {
				fprintf(SIMPLE_DEBUG_TARGET, "[  ] ");
			} else {
				fprintf(SIMPLE_DEBUG_TARGET, "[%2d] ", circle[k]);
			}
		}
		fprintf(SIMPLE_DEBUG_TARGET, "\n");


		// Przyjeżdżają nowe samochody
		for(i=0; i<=3; ++i) {
			const float u = (rand()/(float)RAND_MAX);
			const float prob = 1.0f/(float)f[i];
			if( u <= prob) {
				arrival[i] = 1;
				arrival_cnt[i]++;
			} else {
				arrival[i] = 0;
			} // endif
		} // endfor

		// Nastepuje iteracja, ruch na rondzie
		for(i=0; i<=15; ++i) {
			const int j = (i+1) % 16;
			if( (circle[i] == -1) || (circle[i] == j)) {
				new_circle[j] = -1;

			} else {
				new_circle[j] = circle[i];
			} // endif
		} // endfor

		for(i=0; i<=15; ++i) circle[i] = new_circle[i];

		// Samochody wjeżdżają na rondo
		for(i=0; i<=3; ++i) {
			if(circle[offset[i]] == -1) {
				// Jest miejsce na wjazd samochodu
				if(queue[i] > 0) {
					// Czekający samochod wjezdza
					queue[i]--;
					circle[offset[i]] = choose_exit(i);
				} else if(arrival[i] > 0) {
					// Samochod ktory nadjechal wjeżdża na rondo
					arrival[i] = 0;
					circle[offset[i]] = choose_exit(i);
				} // endif

			} else {
				// Nic sie nie dzieje
			} // endif

			if(arrival[i] > 0) {
				// Samochód, który właśnie nadjechał ląduje w kolejce
				wait_cnt[i]++;
				queue[i]++;
			} // endif
		} // endfor
		for(i=0; i<=3; ++i) {
			queue_accum[i] += queue[i];
		} // endfor
	} // endfor iteration


	fprintf(SIMPLE_DEBUG_TARGET, "arrival_cnt : ");
	for(i=0; i<=3; ++i) fprintf(SIMPLE_DEBUG_TARGET, "%4d ", arrival_cnt[i]);
	fprintf(SIMPLE_DEBUG_TARGET, "\nwait_cnt :\t");
	for(i=0; i<=3; ++i) fprintf(SIMPLE_DEBUG_TARGET, "%4d ", wait_cnt[i]);
	fprintf(SIMPLE_DEBUG_TARGET, "\nqueue_accum : ");
	for(i=0; i<=3; ++i) fprintf(SIMPLE_DEBUG_TARGET, "%4d ", queue_accum[i]);
	fprintf(SIMPLE_DEBUG_TARGET, "\n");

	fprintf(SIMPLE_DEBUG_TARGET, "-----\nWyniki po %d iteracji:\n", REQUESTED_ITERATIONS);
	for(i=0; i<=3; ++i) {
		const float AVG_QUEUE = (float)queue_accum[i]/(float)REQUESTED_ITERATIONS;
		const float WAITING_PERC = 100.0f * (float)wait_cnt[i]/(float)arrival_cnt[i];
		//fprintf(SIMPLE_DEBUG_TARGET, "%d) Średnia dl. kolejki: %f, Czekało: %f%\n", i, AVG_QUEUE, WAITING_PERC);
	}



	return 0;
}
