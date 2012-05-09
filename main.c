#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define ROOT 0


/*
 * Zmienne, parametry itp.
 * ----------------------------------------------------------------------------
 */

/**
 * Liczba iteracji.
 */
const unsigned int REQUESTED_ITERATIONS = 32000;

/**
 * Oznaczenia wjazdów.
 */
typedef enum T_ENTRANCE {N, W, S, E} Entrance;

/**
 * Pozycje wjazdów.
 */
const int offset[4] = {0, 4, 8, 12};

/**
 * Tablica prawdopodobienstw, wartości z treści zadania.
 * Zostaje nadpisana w funkcji load_data wartosciami z pliku params.txt
 */
float CHANCES[4][4] = { 		/* Original: */
		{0.1, 0.2, 0.5, 0.2}, 	/* {0.1, 0.2, 0.5, 0.2}, */
		{0.2, 0.1, 0.3, 0.4}, 	/* {0.2, 0.1, 0.3, 0.4}, */
		{0.5, 0.1, 0.1, 0.3}, 	/* {0.5, 0.1, 0.1, 0.3}, */
		{0.3, 0.4, 0.2, 0.1}  	/* {0.3, 0.4, 0.2, 0.1}, */
};

/**
 * Częstotliwosć pojawiania się nowego samochodu na kolejnych wjazdach.
 */
const int f[4] = {3, 3, 4, 2}; // Original: {3, 3, 4, 2};


/*
 * Nagłówki funkcji.
 * ----------------------------------------------------------------------------
 */
int choose_exit(const Entrance entrance_number);
int load_data(float array[4][4], char * filename);



/**
 * Main.
 * ----------------------------------------------------------------------------
 */
int main(int argc, char * argv[]) {

	int circle[16];
	int new_circle[16];

	unsigned char arrival[4];
	unsigned int arrival_cnt[4];
	unsigned int wait_cnt[4];
	unsigned int queue[4];
	unsigned int queue_accum[4];

	unsigned int total_arrival_cnt[4];
	unsigned int total_wait_cnt[4];
	unsigned int total_queue_accum[4];

	int i = 0;
	unsigned int iteration = 0;

	for(i = 0; i<= 15; ++i) {
		circle[i] = -1;
	}
	for(i=0; i<=3; i++) {
		arrival_cnt[i] = 0;
		wait_cnt[i] = 0;
		queue[i] = 0;
		queue_accum[i] = 0;
	}


	int myid, numprocs;

	/*** Inicjalizacja MPI ***/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* Odczytanie parametrów z pliku */
	if( myid == ROOT ) {
		printf("\n+--- Inicjalizacja\n");
		printf("| Numprocs:\t%d\n", numprocs);
		printf("| myid:\t\t%d\n", myid);

		if( !load_data(CHANCES, "params.txt") ) {
			printf("\n+--- Zamykam program\n");
			exit(-1);
		}
	}

	/* Wyslanie ich */
	for(i=0; i<=3; ++i) {
		MPI_Bcast(CHANCES[i], 4, MPI_FLOAT,
				ROOT, MPI_COMM_WORLD);
	}

	/*** Wlasciwy algorytm ***/
	srand((unsigned) time(NULL) * myid + myid);

	for(iteration = 0; iteration <= REQUESTED_ITERATIONS; iteration++) {
		unsigned int k;

		// Przyjeżdżają nowe samochody
		for(i=0; i<=3; ++i) {
			const float u = (rand()/(float)RAND_MAX);
			const float prob = 1.0f/(float)f[i];
			if( u <= prob) {
				arrival[i] = 1;
				arrival_cnt[i]++;
			} else {
				arrival[i] = 0;
			}
		}

		// Nastepuje iteracja, ruch na rondzie-----\n
		for(i=0; i<=15; ++i) {
			const int j = (i+1) % 16;
			if( (circle[i] == -1) || (circle[i] == j)) {
				new_circle[j] = -1;
			} else {
				new_circle[j] = circle[i];
			}
		}

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
				}
			} else {
				// Nic sie nie dzieje
			}

			if(arrival[i] > 0) {
				// Samochód, który właśnie nadjechał ląduje w kolejce
				wait_cnt[i]++;
				queue[i]++;
			}
		}
		for(i=0; i<=3; ++i) {
			queue_accum[i] += queue[i];
		}

	} // koniec głównej pętli
	/*** Koniec algorytmu ***/

	/* Zbieramy wyniki ze wszystkich maszyn */
	MPI_Reduce(arrival_cnt, total_arrival_cnt, 4, MPI_INT,
			MPI_SUM, ROOT, MPI_COMM_WORLD);
	MPI_Reduce(wait_cnt, total_wait_cnt, 4, MPI_INT,
				MPI_SUM, ROOT, MPI_COMM_WORLD);
	MPI_Reduce(queue_accum, total_queue_accum, 4, MPI_INT,
				MPI_SUM, ROOT, MPI_COMM_WORLD);

	if(myid == 0) {

		fprintf(stdout, "\n +--- Wyniki po %d iteracji na kazdej z %d maszyn:\n", REQUESTED_ITERATIONS, numprocs);
		printf("| %1s | %10s %10s %10s %10s %10s\n",
				"",
				"Nadjechalo",
				"Czekalo",
				"Kolejka",
				"Sr. kol.",
				"% Czekalo");
		for(i=0; i<=3; ++i) {
			const float AVG_QUEUE = (float)queue_accum[i]/(float)REQUESTED_ITERATIONS;
			const float WAITING_PERC = 100.0f * (float)wait_cnt[i]/(float)arrival_cnt[i];

			fprintf(stdout, "| %d | %10d %10d %10d %10.2f %10.2f\n",
					i,
					total_arrival_cnt[i],
					total_wait_cnt[i],
					total_queue_accum[i],
					AVG_QUEUE,
					WAITING_PERC);
		}
	}

	/*** Koniec ***/
	MPI_Finalize();
	return 0;
}

/**
 * Definicje pozostałych funkcji.
 * ----------------------------------------------------------------------------
 */

int load_data(float array[4][4], char * filename) {
	FILE * fp = fopen(filename, "r");
	if(!fp) {
		fprintf(stderr, "\n+---Nie udało się otworzyć pliku z parametrami!\n");
		return 0;
	}

	unsigned int i, j;
	unsigned int line_cnt = 0;
	while(!feof(fp)) {
		fscanf( fp,
				"%f %f %f %f\n",
				&array[line_cnt][0],
				&array[line_cnt][1],
				&array[line_cnt][2],
				&array[line_cnt][3] );
		++line_cnt;
	}

	float sums[4] = {0,};
	for(i=0; i<=3; ++i) {
		for(j=0; j<=3; ++j) {
			sums[i] += array[i][j];
		}
	}

	for(i=0;i<=3;++i) {
		if(sums[i] != 1.0) {
			fprintf( stderr,
					"\n+--- Tablica prawdopodobienstw ma bledy.\n"
					"| Suma prawdopodobieństw dla zjazdu nr %d wynosi %f.\n"
					"| Popraw plik params.txt tak, żeby suma prawdopodobienstw w kazdej linii wynosila 1.",
					i, sums[i]);
			MPI_Finalize();
			return 0;
		}
	}

	fprintf(stdout, "\n+--- Tablica prawdopodobieństw:\n");
	for(i=0;i<=3;++i) {
		printf("| %d | ", i);
		for(j=0; j<=3; ++j) {
			printf("%1.2f  ", array[i][j]);
		}
		printf("| Suma: %f\n", sums[i]);
	}

	return 1;
}

int choose_exit(const Entrance entrance_number) {
	float dist_n, dist_w, dist_s;
	Entrance exit_no;

	const float RANDOM = (float)rand() / (float)RAND_MAX;

	dist_n = CHANCES[entrance_number][0];
	dist_w = dist_n + CHANCES[entrance_number][1];
	dist_s = dist_w + CHANCES[entrance_number][2];

	if(RANDOM < dist_n) exit_no = N;
	else if(dist_n <= RANDOM && RANDOM < dist_w) exit_no = W;
	else if(dist_w <= RANDOM && RANDOM < dist_s) exit_no = S;
	else exit_no = E;

	return offset[exit_no];
}
