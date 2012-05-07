#ifndef __DATATYPES_AND_CONSTANTS_H__
#define __DATATYPES_AND_CONSTANTS_H__

/**
 * Number of iterations
 */
const unsigned int REQUESTED_ITERATIONS = 32000;

/**
 * Entrance name-to-number
 */
typedef enum T_ENTRANCE {N, W, S, E} Entrance;

/**
 * Entrances' sector number
 */
const int offset[4] = {0, 4, 8, 12};
const float CHANCES[4][4] = { /* Original: */
		{0.1, 0.2, 0.5, 0.2}, /* {0.1, 0.2, 0.5, 0.2}, */
		{0.2, 0.1, 0.3, 0.4}, /* {0.2, 0.1, 0.3, 0.4}, */
		{0.5, 0.1, 0.1, 0.3}, /* {0.5, 0.1, 0.1, 0.3}, */
		{0.3, 0.4, 0.2, 0.1}  /* {0.3, 0.4, 0.2, 0.1}, */
};

#endif
