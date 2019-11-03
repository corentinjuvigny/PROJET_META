/*

Copyright (c) 2019-2020, Adrien BLASSIAU and Corentin JUVIGNY

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

/** @file annealing.h
 *
 * @brief Simulated annealing algorithm to construct a better solution.
 *
 */

#ifndef __ANNEALING__
#define __ANNEALING__


typedef struct _BestSolution BestSolution;

/**
 * \struct _BestSolution
 * \brief The best solution given by the simulated annealing algorithm .
 *
 * Use to store the best solution and his size.
 * - best_solution is a list of the name of the sensor on the best solution.
 * - size is the size of the list.
 */
struct _BestSolution {
  Queue*	best_solution;
  int 		size;
};

/**
 * This function applies a simulated annealing algorithm on the solution given
 * by the greedy algorithm.
 *
 * @param pf 				   The modify solution.
 * @param phi                  The phi parameter of the simulated annealing.
 * @param step                 The step of the simulated annealing.
 * @param T_initial            The initial temperature of the simulated
 *                             annealing.
 * @param nb_iterations        The number of iterations of the simulated
 *                             annealing.
 * @param progress		       If we want a progress bar.
 * @return The best solution we found
 */
BestSolution* simulated_annealing(TPointFile* pf, double phi, int step, double T_initial, int nb_iterations, int progress);

/**
 * This function reconstructs the solution.
 * @param pf All the data of the problem.
 * @param bs The beast solution.
 */
void reconstruct_solution(TPointFile* pf, BestSolution* bs);

#endif