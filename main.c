#include <stdio.h>

#include "functions.c"

#define SIMPLE_DEBUG_TARGET stdout
#define FULL_DEBUG_TARGET stdout



/**
 * Lista możiliwych wjazdów na rondo
 */


int main() {
	srand(time(NULL));

	//fprintf(FULL_DEBUG_TARGET, "## Start ##\n");

	int circle[16];
	int new_circle[16];

	unsigned char arrival[4];
	unsigned int arrival_cnt[4];
	unsigned int wait_cnt[4];
	unsigned int queue[4];
	unsigned int queue_accum[4]= {0,};

	int i = 0;
	unsigned int iteration = 0;

	unsigned int requested_iterations = 32000;
	const int f[4] = {3, 3, 4, 2}; // Original: {3, 3, 4, 2};
	//fprintf(FULL_DEBUG_TARGET, "## Start ##\n");

	for(i = 0; i<= 15; ++i) {
		circle[i] = -1;
		//fprintf(FULL_DEBUG_TARGET, "## Start A##\n");
	} // endfor
	for(i=0; i<=3; i++) {
		arrival_cnt[i] = 0;
		wait_cnt[i] = 0;
		queue[i] = 0;
		queue_accum[i] = 0;
		//fprintf(FULL_DEBUG_TARGET, "%d %d %d %d \n", arrival_cnt[i], wait_cnt[i], queue[i], queue_accum[i]);
		//fprintf(FULL_DEBUG_TARGET, "## Start B %d ##\n", i);
	} // endfor


	fprintf(FULL_DEBUG_TARGET, "## Start ##\n");

	for(iteration = 0; iteration <= requested_iterations; iteration++) {
		unsigned int k;
		//printf("def SIMPLE_DEBUG_TARGET\n");
		for(k=0; k<=15; ++k) {
			if(circle[k] == -1) {
				fprintf(SIMPLE_DEBUG_TARGET, "[  ] ");
			} else {
				fprintf(SIMPLE_DEBUG_TARGET, "[%2d] ", circle[k]);
			}
		}
		fprintf(SIMPLE_DEBUG_TARGET, "\n");


		//fprintf(FULL_DEBUG_TARGET, "------------\nIteracja numer: %d\n", iteration);
		// Przyjeżdżają nowe samochody
		//fprintf(FULL_DEBUG_TARGET, "1) Przyjeżdżają nowe samochody\n");
		for(i=0; i<=3; ++i) {
			//printf("f[i] int %d float %f double %f\n", f[i], (float)f[i], (double)f[i]);
			const float u = (rand()/(float)RAND_MAX);
			const float prob = 1.0f/(float)f[i];
			if( u <= prob) {
				arrival[i] = 1;
				arrival_cnt[i]++;
			} else {
				arrival[i] = 0;
			} // endif
			//fprintf(FULL_DEBUG_TARGET, "\t- na wjeździe numer %d przyjazd samochodu %d \t| u[%f] f[i][%f] 1/f[i][%f]\n", i, arrival[i], u, (float)f[i], prob);
		} // endfor

		// Nastepuje iteracja, ruch na rondzie
		//fprintf(FULL_DEBUG_TARGET, "2) Ruch na rondzie\n");
		for(i=0; i<=15; ++i) {
			const int j = (i+1) % 16;
			//fprintf(FULL_DEBUG_TARGET, "\t- sektor[%2d]=%2d ", i, circle[i]);
			if( (circle[i] == -1) || (circle[i] == j)) {
				new_circle[j] = -1;

			} else {
				new_circle[j] = circle[i];
			} // endif
			//fprintf(FULL_DEBUG_TARGET, "\t| nowy_sektor[%2d]=%2d\n", j, new_circle[j]);
		} // endfor

		for(i=0; i<=15; ++i) circle[i] = new_circle[i];

		// Samochody wjeżdżają na rondo
		//fprintf(FULL_DEBUG_TARGET, "3) Samochody wjeżdżają na rondo\n");
		for(i=0; i<=3; ++i) {
			//fprintf(FULL_DEBUG_TARGET, "\t- wjazd numer %d: ", i);
			if(circle[offset[i]] == -1) {
				//fprintf(FULL_DEBUG_TARGET, "| JEST miejsce na wjazd ");
				// Jest miejsce na wjazd samochodu
				if(queue[i] > 0) {
					// Czekający samochod wjezdza
					queue[i]--;
					circle[offset[i]] = choose_exit(i);
					//fprintf(PRINT_TARGET, "\t kolejka %d \t cel %d", queue[i], circle[offset[i]]);
					//fprintf(FULL_DEBUG_TARGET, "| wjechał KOL | kolejka %d", queue[i]);
				} else if(arrival[i] > 0) {
					// Samochod ktory nadjechal wjeżdża na rondo
					arrival[i] = 0;
					circle[offset[i]] = choose_exit(i);
					//fprintf(FULL_DEBUG_TARGET, "| wjechał TAK | kolejka %d", queue[i]);
				} // endif
				//fprintf(FULL_DEBUG_TARGET, "| cel %d\n", circle[offset[i]]);

			} else {
				//fprintf(FULL_DEBUG_TARGET, "| BRAK miejsca na wjazd ");
				//fprintf(FULL_DEBUG_TARGET, "| wjechał NIE | kolejka %d\n", queue[i]);
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

	fprintf(SIMPLE_DEBUG_TARGET, "-----\nWyniki po %d iteracji:\n", requested_iterations);
	for(i=0; i<=3; ++i) {
		const float AVG_QUEUE = (float)queue_accum[i]/(float)requested_iterations;
		const float WAITING_PERC = 100.0f * (float)wait_cnt[i]/(float)arrival_cnt[i];
		fprintf(SIMPLE_DEBUG_TARGET, "%d) Średnia dl. kolejki: %f, Czekało: %f%\n", i, AVG_QUEUE, WAITING_PERC);
	}



	return 0;
}
